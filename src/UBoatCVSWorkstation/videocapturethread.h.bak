#ifndef VIDEOCAPTURETHREAD_H
#define VIDEOCAPTURETHREAD_H

#include <QThread>
#include <QMutex>

#include <opencv2/opencv.hpp>


class VideoCaptureThread : public QThread
{
    Q_OBJECT
public:
    VideoCaptureThread();
    VideoCaptureThread(const std::string &pipeline, QObject *parent = nullptr);
    void stop();

signals:
    void frameReady(cv::Mat &frame);
    void captureError(const QString &error);
    void captureOpened();

protected:
    void run() override;

private:
    std::string pipeline;
    bool running;
    QMutex frameMutex;
};

#endif // VIDEOCAPTURETHREAD_H
