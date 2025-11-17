#include "videocapturethread.h"

VideoCaptureThread::VideoCaptureThread() {}

VideoCaptureThread::VideoCaptureThread(const std::string &pipeline, QObject *parent)
    : QThread(parent), pipeline(pipeline), running(false)
{
}

void VideoCaptureThread::stop()
{
    running = false;
    wait();
}

void VideoCaptureThread::run()
{
    running = true;
    cv::VideoCapture capture;

    try
    {
        capture.open(pipeline, cv::CAP_GSTREAMER);
        if (!capture.isOpened())
        {
            emit captureError("Не удалось открыть GStreamer pipeline");
            return;
        }
        emit captureOpened();

        cv::Mat frame;
        while (running && capture.isOpened())
        {
            if (capture.read(frame) && !frame.empty())
            {
                QMutexLocker locker(&frameMutex);
                emit frameReady(frame);
            } else
            {
                emit captureError("Не удалось прочитать кадр или получен пустой кадр");
                running = false;
            }
            msleep(33);
        }
    }
    catch (const std::exception &e)
    {
        emit captureError(QString("Исключение GStreamer: %1").arg(e.what()));
    }

    if (capture.isOpened())
    {
        capture.release();
    }
}


