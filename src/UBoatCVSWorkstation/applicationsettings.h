#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QString>
#include <string>

class ApplicationSettings
{
public:
    ApplicationSettings();
    QString getAppVersion();

    const int BORDER_SIZE = 10;
    const int PANEL_TOOL_SIZE = 100;
    const int PANEL_INFO_SIZE = 300;
    const int CAMERA_ID = 0;
    const int CAMERA_WIDTH = 640 /*640*/ /*1280*/;
    const int CAMERA_HEIGHT = 480 /*480*/ /*720*/;
    const int CAMERA_FPS = 30;
    const int VIDEO_TIMER_INTERVAL = 100;


    const double ALPHA = 0.5;
    const int SIGHT_SIZE = 50;
    const int SIGHT_TICK = 10;
    const int SIGHT_CROSS = 20;
    const int SIGHT_DELTA = 5;

    const int GRID_SMALL_SIZE = 15; // Размер маленьких рисочек
    const int GRID_BIG_SIZE = 30; // Размер больших рисочек

    const int GRID_V_DELTA = CAMERA_HEIGHT / 75; // Плотность вертикальных
    const int GRID_V_MAX = 5;

    const int GRID_H_DELTA = CAMERA_WIDTH / 100; // Плотность горизонтальных
    const int GRID_H_MAX = 5;

    const int XV0 = CAMERA_WIDTH / 5;
    const int YV0 = CAMERA_HEIGHT / 2 - GRID_V_DELTA * 2 * 10;

    const int XH0 = CAMERA_WIDTH / 2 - GRID_H_DELTA * 2 * 10;
    const int YH0 = CAMERA_HEIGHT / 10;

private:
    const std::string APP_VERSION = "1.0.1";
};

#endif // APPLICATIONSETTINGS_H
