#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>

#include "applicationsettings.h"

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
    void moveWindowToCenter();

private:
    Ui::MainWindow *ui;
    ApplicationSettings _appSet; // Уставки приложения
};
#endif // MAINWINDOW_H
