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
    const int CAMERA_WIDTH = 640 /*640*//*1280*/;
    const int CAMERA_HEIGHT = 480 /*480*//*720*/;
    const int CAMERA_FPS = 30;
    const int VIDEO_TIMER_INTERVAL = 100;

private:
    const std::string APP_VERSION = "1.0.1";
};

#endif // APPLICATIONSETTINGS_H
