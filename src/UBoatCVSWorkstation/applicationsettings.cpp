#include "applicationsettings.h"

ApplicationSettings::ApplicationSettings() {}

QString ApplicationSettings::getAppVersion()
{
    return QString(APP_VERSION.c_str());
}

void ApplicationSettings::load()
{
    _settingsFileName = QApplication::applicationDirPath() + QDir::separator() + "settings.ini";
    QSettings settings(_settingsFileName, QSettings::IniFormat);

    // Camera Settings
    settings.beginGroup("/CAMERA_SETTINGS");
    CAMERA_ID = settings.value("/ID", "0").toInt();
    CAMERA_WIDTH = settings.value("/WIDTH", "854").toInt();
    CAMERA_HEIGHT = settings.value("/HEIGHT", "480").toInt();
    CAMERA_FPS = settings.value("/FPS", "30").toInt();
    settings.endGroup();

    // Screen Setting
    settings.beginGroup("/SCREEN_SETTINGS");
    DRAW_SIGHT = settings.value("/DRAW_SIGHT", "0").toBool();
    DRAW_GRID = settings.value("/DRAW_GRID", "0").toBool();
    settings.endGroup();

    // General Setting
    settings.beginGroup("/GENERAL_SETTINGS");
    TEST_MODE = settings.value("/TEST_MODE", "0").toBool();
    settings.endGroup();
}

void ApplicationSettings::save()
{
    _settingsFileName = QApplication::applicationDirPath() + QDir::separator() + "settings.ini";
    QSettings settings(_settingsFileName, QSettings::IniFormat);

    settings.beginGroup("/CAMERA_SETTINGS");
    settings.setValue("/ID", CAMERA_ID);
    settings.setValue("/WIDTH", CAMERA_WIDTH);
    settings.setValue("/HEIGHT", CAMERA_HEIGHT);
    settings.setValue("/FPS", CAMERA_FPS);
    settings.endGroup();

    settings.beginGroup("/SCREEN_SETTINGS");
    settings.setValue("/DRAW_SIGHT", DRAW_SIGHT);
    settings.setValue("/DRAW_GRID", DRAW_GRID);
    settings.endGroup();

    settings.beginGroup("/GENERAL_SETTINGS");
    settings.setValue("/TEST_MODE", TEST_MODE);
    settings.endGroup();
}
