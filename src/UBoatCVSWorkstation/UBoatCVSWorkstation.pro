QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    applicationsettings.cpp \
    main.cpp \
    mainwindow.cpp \
    settingswindow.cpp \
    uboatmodel.cpp

HEADERS += \
    applicationcommon.h \
    applicationsettings.h \
    mainwindow.h \
    settingswindow.h \
    uboatmodel.h

FORMS += \
    mainwindow.ui \
    settingswindow.ui

INCLUDEPATH += c:\opencv-4.12.0-build\install\include
LIBS += -lwsock32
LIBS += -lws2_32
LIBS += -LC:\opencv-4.12.0-build\install\x64\vc17\lib

# Конфигурация OpenCV зависимостей
CONFIG(debug, debug|release) {
    LIBS += -lopencv_core4120d -lopencv_imgcodecs4120d -lopencv_highgui4120d \
        -lopencv_features2d4120d -lopencv_calib3d4120d -lopencv_videoio4120d \
        -lopencv_imgproc4120d -lopencv_ximgproc4120d
}
CONFIG(release, debug|release) {
    LIBS += -lopencv_core4120 -lopencv_imgcodecs4120 -lopencv_highgui4120 \
        -lopencv_features2d4120 -lopencv_calib3d4120 -lopencv_videoio4120 \
        -lopencv_imgproc4120 -lopencv_ximgproc4120
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBoatCVSLibrary/release/ -lUBoatCVSLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBoatCVSLibrary/debug/ -lUBoatCVSLibrary

INCLUDEPATH += $$PWD/../UBoatCVSLibrary
DEPENDPATH += $$PWD/../UBoatCVSLibrary

RESOURCES += \
    resources.qrc
