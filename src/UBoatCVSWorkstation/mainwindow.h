#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QMessageBox>
#include <QDebug>

#include "applicationsettings.h"
#include "applicationcommon.h"
#include "uboatmodel.h"

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
    //ConnectionStatus _cameraStatus;
    //ConnectionStatus _packetStatus;

private:
    Ui::MainWindow *ui;
    ApplicationSettings _appSet; // Уставки приложения
    UBoatModel *_model;

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
