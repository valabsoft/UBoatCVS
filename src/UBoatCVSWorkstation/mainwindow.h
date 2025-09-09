#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QMessageBox>

#include "applicationsettings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(ConnectionStatus CameraSatatus READ CameraSatatus WRITE setCameraSatatus NOTIFY cameraStatusChanged)
    Q_PROPERTY(ConnectionStatus PacketSatatus READ PacketSatatus WRITE setPacketSatatus NOTIFY packetStatusChanged)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum ConnectionStatus { ON, OFF, UNKNOWN };
    Q_ENUM(ConnectionStatus)

    void setGeometry();
    void moveWindowToCenter();

    void setCameraSatatus(ConnectionStatus cameraStatus);
    ConnectionStatus CameraSatatus() const;

    void setPacketSatatus(ConnectionStatus packetStatus);
    ConnectionStatus PacketSatatus() const;

private:
    ConnectionStatus _cameraStatus;
    ConnectionStatus _packetStatus;

private:
    Ui::MainWindow *ui;
    ApplicationSettings _appSet; // Уставки приложения

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
