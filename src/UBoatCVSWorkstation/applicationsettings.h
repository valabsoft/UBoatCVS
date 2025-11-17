#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QApplication>
#include <QSettings>
#include <QString>
#include <QDir>
#include <string>

class ApplicationSettings
{
public:
    ApplicationSettings();
    QString getAppVersion();
    void load();
    void save();

    int CAMERA_ID = 0;
    int CAMERA_WIDTH = 800 /*640*/ /*1280*/;
    int CAMERA_HEIGHT = 600 /*480*/ /*720*/;
    int CAMERA_FPS = 30;
    int VIDEO_TIMER_INTERVAL = 100;

    bool TEST_MODE = true;
    bool DRAW_SIGHT = true;
    bool DRAW_GRID = true;

    const int BORDER_SIZE = 10;
    const int PANEL_TOOL_SIZE = 100;
    const int PANEL_INFO_SIZE = 300;

    const double ALPHA = 0.5;

    const int SIGHT_SIZE = 50;
    const int SIGHT_TICK = 10;
    const int SIGHT_CROSS = 20;
    const int SIGHT_DELTA = 5;

    const int GRID_SMALL_SIZE = 15; // Размер маленьких рисочек
    const int GRID_BIG_SIZE = 30; // Размер больших рисочек
    int GRID_V_DELTA = CAMERA_HEIGHT / 100; // Плотность вертикальных
    const int GRID_V_MAX = 5;
    int GRID_H_DELTA = CAMERA_WIDTH / 100; // Плотность горизонтальных
    const int GRID_H_MAX = 5;

    int XV0 = CAMERA_WIDTH / 5;
    int YV0 = CAMERA_HEIGHT / 2 - GRID_V_DELTA * GRID_V_MAX / 2 * 10; // CAMERA_HEIGHT / 2 - GRID_V_DELTA * 2 * 10;

    int XH0 = CAMERA_WIDTH / 2 - GRID_H_DELTA * GRID_H_MAX / 2 * 10; // CAMERA_WIDTH / 2 - GRID_H_DELTA * 2 * 10;
    int YH0 = CAMERA_HEIGHT / 10;

    int PORT_VIDEO = 8010;
    int PORT_DATA = 6000;

    const int DATA_PACKET_SIZE = 28;

private:
    QString _settingsFileName;
    const std::string APP_VERSION = "1.0.1";
};

#endif // APPLICATIONSETTINGS_H
