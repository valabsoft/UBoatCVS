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
    const int CAMERA_WIDTH = 1280;
    const int CAMERA_HEIGHT = 720;
    const int PANEL_SIZE = 100;

private:
    const std::string APP_VERSION = "1.0.1";
};

#endif // APPLICATIONSETTINGS_H
