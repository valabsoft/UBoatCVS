QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# GStreamer paths
win32 {
    GSTREAMER_DIR = "C:/Program Files/gstreamer/1.0/msvc_x86_64"

    # Include paths
    INCLUDEPATH += $${GSTREAMER_DIR}/include/gstreamer-1.0 \
                   $${GSTREAMER_DIR}/lib/gstreamer-1.0/include \
                   $${GSTREAMER_DIR}/include/glib-2.0 \
                   $${GSTREAMER_DIR}/lib/glib-2.0/include

    # Library path
    LIBS += -L$${GSTREAMER_DIR}/lib

    # GStreamer libraries
    LIBS += -lgstreamer-1.0 -lgstbase-1.0 -lgstapp-1.0 -lgstvideo-1.0

    # Dependency libraries
    LIBS += -lglib-2.0 -lgobject-2.0 -lgmodule-2.0 -lgio-2.0 -lgthread-2.0 -lintl
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
