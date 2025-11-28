#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>

extern "C" {
#include <gst/gst.h>
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    //if (argc < 2) {
    //    qCritical() << "Usage: VideoStreamer <video_file> [port]";
    //    qCritical() << "Example: VideoStreamer C:/test.mp4 5000";
    //    return 1;
    //}

    QString videoPath = "C:\\temp\\VID.mp4"; //argv[1];
    int port = (argc > 2) ? QString(argv[2]).toInt() : 5000;

    // Проверка файла
    QFileInfo fileInfo(videoPath);
    if (!fileInfo.exists()) {
        qCritical() << "File not found:" << videoPath;
        return 1;
    }

    qDebug() << "Streaming file:" << videoPath;
    qDebug() << "File size:" << fileInfo.size() << "bytes";

    // Инициализация GStreamer
    gst_init(nullptr, nullptr);

    // Простой тестовый пайплайн
    QString pipelineStr = QString(
                              "videotestsrc pattern=snow ! "
                              "videoconvert ! x264enc ! rtph264pay ! "
                              "udpsink host=127.0.0.1 port=%1"
                              ).arg(port);

    qDebug() << "Using test pipeline:" << pipelineStr;

    GError *error = nullptr;
    GstElement *pipeline = gst_parse_launch(pipelineStr.toUtf8().constData(), &error);

    if (error) {
        qCritical() << "Failed to create pipeline:" << error->message;
        g_error_free(error);
        return 1;
    }

    qDebug() << "Pipeline created, starting...";

    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    if (ret == GST_STATE_CHANGE_FAILURE) {
        qCritical() << "Failed to start pipeline";

        // Проверка состояния
        GstState state, pending;
        gst_element_get_state(pipeline, &state, &pending, GST_CLOCK_TIME_NONE);
        qDebug() << "State:" << gst_element_state_get_name(state);
        qDebug() << "Pending:" << gst_element_state_get_name(pending);

        gst_object_unref(pipeline);
        return 1;
    }

    qDebug() << "=== TEST STREAM RUNNING ===";
    qDebug() << "Streaming test video to: udp://127.0.0.1:" << port;
    qDebug() << "Use: ffplay udp://127.0.0.1:" << port;
    qDebug() << "Press Enter to stop...";

    // Ожидание Enter
    QTextStream stream(stdin);
    stream.readLine();

    qDebug() << "Stopping...";
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    qDebug() << "Done";
    return 0;
}
