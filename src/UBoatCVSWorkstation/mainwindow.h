#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QImage>

#include "applicationsettings.h"
#include "applicationcommon.h"
#include "uboatmodel.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    

    void setGeometry();
    void setButtonIcons();
    void setStyle(Theme theme);
    void moveWindowToCenter();

    void setCameraSatatus(ConnectionStatus cameraStatus);
    void setPacketSatatus(ConnectionStatus packetStatus);

private:
    void onVideoTimer();
    QImage cvMatToQImage(const cv::Mat &mat);

private:
    Ui::MainWindow *ui;
    ApplicationSettings _appSet; // Уставки приложения
    UBoatModel *_model;
    QTimer *_videoTimer;
    cv::VideoCapture *_capture;

signals:
    void cameraStatusChanged(ConnectionStatus);
    void packetStatusChanged(ConnectionStatus);

private slots:
    void onCameraButtonClicked();
    void onPacketButtonClicked();
    void onCameraStatusChanged();
    void onPacketStatusChanged();
};
#endif // MAINWINDOW_H
