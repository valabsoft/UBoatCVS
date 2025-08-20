#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QString>
#include <string>

class ApplicationSettings
{
public:
    ApplicationSettings();
    QString getAppVersion();
private:
    const std::string APP_VERSION = "1.0.1";
};

#endif // APPLICATIONSETTINGS_H
