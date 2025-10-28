#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QCommandLineParser>

// Включаем заголовки GStreamer с extern "C"
#ifdef __cplusplus
extern "C" {
#endif

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#ifdef __cplusplus
}
#endif

class VideoStreamer : public QObject
{
    Q_OBJECT

public:
    VideoStreamer(QObject *parent = nullptr) : QObject(parent)
    {
        m_busTimer = new QTimer(this);
        connect(m_busTimer, &QTimer::timeout, this, &VideoStreamer::checkBusMessages);
    }

    ~VideoStreamer()
    {
        stopStreaming();
    }

    bool startStreaming(const QString &videoPath, int port = 8554)
    {
        gst_init(nullptr, nullptr);

        // Создание сервера
        m_server = gst_rtsp_server_new();
        gst_rtsp_server_set_service(m_server, QString::number(port).toUtf8().constData());

        // Настройка mount points
        GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(m_server);
        GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();

        // Простой пайплайн для видео
        QString pipelineStr = QString(
                                  "filesrc location=\"%1\" ! "
                                  "qtdemux ! queue ! h264parse ! rtph264pay name=pay0 pt=96"
                                  ).arg(videoPath);

        gst_rtsp_media_factory_set_launch(factory, pipelineStr.toUtf8().constData());
        gst_rtsp_media_factory_set_shared(factory, TRUE);

        gst_rtsp_mount_points_add_factory(mounts, "/test", factory);
        gst_object_unref(mounts);

        // Запуск сервера
        m_loop = g_main_loop_new(nullptr, FALSE);
        gst_rtsp_server_attach(m_server, nullptr);

        m_busTimer->start(100);

        qDebug() << "RTSP Video Streamer started on port" << port;
        qDebug() << "Stream URL: rtsp://localhost:" << port << "/test";
        qDebug() << "Use VLC or ffplay to view the stream";
        qDebug() << "Press Ctrl+C to stop";

        // Запуск главного цикла в отдельном таймере
        QTimer::singleShot(0, this, [this]() {
            if (m_loop) {
                g_main_loop_run(m_loop);
            }
        });

        return true;
    }

    void stopStreaming()
    {
        if (m_busTimer) {
            m_busTimer->stop();
        }

        if (m_loop) {
            g_main_loop_quit(m_loop);
            g_main_loop_unref(m_loop);
            m_loop = nullptr;
        }

        if (m_server) {
            gst_object_unref(m_server);
            m_server = nullptr;
        }

        qDebug() << "Streaming server stopped";
    }

private slots:
    void checkBusMessages()
    {
        // Проверка системных сообщений
    }

private:
    GstRTSPServer *m_server = nullptr;
    GMainLoop *m_loop = nullptr;
    QTimer *m_busTimer = nullptr;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Video Streamer");
    QCoreApplication::setApplicationVersion("1.0");

    // Парсинг аргументов командной строки
    QCommandLineParser parser;
    parser.setApplicationDescription("RTSP Video Streaming Server");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption videoFileOption(
        QStringList() << "f" << "file",
        "Video file to stream (MP4 with H.264)",
        "filename"
        );
    parser.addOption(videoFileOption);

    QCommandLineOption portOption(
        QStringList() << "p" << "port",
        "Port for RTSP stream (default: 8554)",
        "port",
        "8554"
        );
    parser.addOption(portOption);

    parser.process(app);

    QString videoPath = parser.value(videoFileOption);
    int port = parser.value(portOption).toInt();

    if (videoPath.isEmpty()) {
        qCritical() << "Error: Please specify video file with --file option";
        qCritical() << "Example: --file C:/video.mp4";
        parser.showHelp(1);
    }

    VideoStreamer streamer;

    // Остановка при выходе из приложения
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &streamer, &VideoStreamer::stopStreaming);

    if (!streamer.startStreaming(videoPath, port)) {
        qCritical() << "Failed to start streaming server";
        return 1;
    }

    return app.exec();
}
