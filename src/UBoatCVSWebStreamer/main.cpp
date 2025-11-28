#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QCommandLineParser>

extern "C" {
#include <gst/gst.h>
}

// Глобальные переменные для состояния
static GstElement *g_pipeline = nullptr;
static QTimer *g_busTimer = nullptr;

void stopStreaming()
{
    if (g_busTimer) {
        g_busTimer->stop();
        delete g_busTimer;
        g_busTimer = nullptr;
    }

    if (g_pipeline) {
        gst_element_set_state(g_pipeline, GST_STATE_NULL);
        gst_object_unref(g_pipeline);
        g_pipeline = nullptr;
        qDebug() << "Webcam streaming stopped";
    }
}

void checkBusMessages()
{
    if (!g_pipeline) return;

    GstBus *bus = gst_element_get_bus(g_pipeline);
    GstMessage *msg = gst_bus_pop(bus);

    while (msg) {
        switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
            qDebug() << "End of stream";
            stopStreaming();
            QCoreApplication::quit();
            break;

        case GST_MESSAGE_ERROR: {
            GError *error;
            gchar *debug;
            gst_message_parse_error(msg, &error, &debug);
            qCritical() << "Error:" << error->message;
            if (debug) qDebug() << "Debug info:" << debug;
            g_error_free(error);
            g_free(debug);
            stopStreaming();
            QCoreApplication::quit();
            break;
        }

        case GST_MESSAGE_WARNING: {
            GError *warning;
            gchar *debug;
            gst_message_parse_warning(msg, &warning, &debug);
            qWarning() << "Warning:" << warning->message;
            g_error_free(warning);
            g_free(debug);
            break;
        }

        default:
            break;
        }

        gst_message_unref(msg);
        msg = gst_bus_pop(bus);
    }

    gst_object_unref(bus);
}

QString createWebcamPipeline(const QString &host, int port, int webcamIndex)
{
    QString pipeline;

#ifdef Q_OS_WIN
    // Windows - используем ksvideosrc или dshow
    pipeline = QString(
                   "ksvideosrc device-index=%1 ! "
                   "videoconvert ! video/x-raw,format=I420 ! "
                   "x264enc speed-preset=ultrafast tune=zerolatency bitrate=500 ! "
                   "rtph264pay ! udpsink host=%2 port=%3 sync=false"
                   ).arg(webcamIndex).arg(host).arg(port);

#elif defined(Q_OS_LINUX)
    // Linux - используем v4l2src
    pipeline = QString(
                   "v4l2src device=/dev/video%1 ! "
                   "videoconvert ! video/x-raw,format=I420 ! "
                   "x264enc speed-preset=ultrafast tune=zerolatency bitrate=500 ! "
                   "rtph264pay ! udpsink host=%2 port=%3 sync=false"
                   ).arg(webcamIndex).arg(host).arg(port);

#else
    // Универсальный пайплайн
    pipeline = QString(
                   "autovideosrc ! "
                   "videoconvert ! video/x-raw,format=I420 ! "
                   "x264enc speed-preset=ultrafast tune=zerolatency bitrate=500 ! "
                   "rtph264pay ! udpsink host=%2 port=%3 sync=false"
                   ).arg(host).arg(port);
#endif

    return pipeline;
}

bool startStreaming(const QString &host = "127.0.0.1", int port = 5000, int webcamIndex = 0)
{
    // Инициализация GStreamer
    gst_init(nullptr, nullptr);

    // Создание пайплайна для вебкамеры
    QString pipelineStr = createWebcamPipeline(host, port, webcamIndex);
    qDebug() << "Using pipeline:" << pipelineStr;

    GError *error = nullptr;
    g_pipeline = gst_parse_launch(pipelineStr.toUtf8().constData(), &error);

    if (!g_pipeline) {
        if (error) {
            qCritical() << "Failed to create pipeline:" << error->message;
            g_error_free(error);
        }
        return false;
    }

    // Запуск пайплайна
    GstStateChangeReturn ret = gst_element_set_state(g_pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        qCritical() << "Failed to start pipeline";
        gst_object_unref(g_pipeline);
        g_pipeline = nullptr;
        return false;
    }

    // Создание таймера для проверки сообщений
    g_busTimer = new QTimer();
    QObject::connect(g_busTimer, &QTimer::timeout, &checkBusMessages);
    g_busTimer->start(100);

    qDebug() << "=== Webcam Streaming Started ===";
    qDebug() << "Webcam device:" << webcamIndex;
    qDebug() << "Stream: udp://" << host << ":" << port;
    qDebug() << "To view stream:";
    qDebug() << "  ffplay udp://" << host << ":" << port;
    qDebug() << "  vlc udp://@" << host << ":" << port;
    qDebug() << "Press Ctrl+C to stop";

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Webcam Streamer");
    QCoreApplication::setApplicationVersion("1.0");

    //QCommandLineParser parser;
    //parser.setApplicationDescription("Webcam UDP Streaming using GStreamer");
    //parser.addHelpOption();
    //parser.addVersionOption();

    //QCommandLineOption hostOption("h", "Host address", "host", "127.0.0.1");
    //parser.addOption(hostOption);

    //QCommandLineOption portOption("p", "UDP port", "port", "5000");
    //parser.addOption(portOption);

    //QCommandLineOption webcamOption("d", "Webcam device index", "device", "0");
    //parser.addOption(webcamOption);

    //parser.process(app);

    //QString host = parser.value(hostOption);
    //int port = parser.value(portOption).toInt();
    //int webcamIndex = parser.value(webcamOption).toInt();

    QString host = "127.0.0.1";
    int port = 1234;
    int webcamIndex = 0;

    // Остановка при выходе из приложения
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &stopStreaming);

    if (!startStreaming(host, port, webcamIndex)) {
        qCritical() << "Failed to start webcam streaming";
        return 1;
    }

    return app.exec();
}
