#-------------------------------------------------
#
# Project created by QtCreator 2014-02-23T16:44:29
#
#-------------------------------------------------

QT       += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Spectral-Explorer
VERSION = 1.6.0
#REVISION = $$system(git rev-parse HEAD)
#DEFINES += APP_REVISION=\\\"$$REVISION\\\"
TEMPLATE = app
TRANSLATIONS += translations/app_de.ts
CONFIG += c++11

unix {
    message ("unix")
    OPENCV_PATH = /usr/include/opencv2/
    LIBS     += -lopencv_core \
                -lopencv_highgui \
                -lopencv_imgproc
}
win32 {
    message ("win32")

    message ("opencv2")
    INCLUDEPATH += C:/opencv-2-4-13/install/include/
    LIBS += -LC:/opencv-2-4-13/install/x86/mingw/lib/
    Release {
        message ("release")
        LIBS += libopencv_core2413 \
                libopencv_highgui2413 \
                libopencv_imgproc2413
    }
    Debug {
        message ("debug")
        LIBS += libopencv_core2413 \
                libopencv_highgui2413 \
                libopencv_imgproc2413
    }
}

include(gui/gui.pri)
include(camera/camera.pri)
include(spectrum/spectrum.pri)
include(settings/settings.pri)

SOURCES += main.cpp \

DISTFILES += \
    CHANGELOG.md \
    README.md \

RESOURCES += \
    qdarkstyle/style.qrc \
    icons/icons.qrc \
    translations/translations.qrc

win32:RC_FILE += win_resources/resources.rc

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
