QT += core gui widgets multimedia testlib
CONFIG += c++11 console testcase
CONFIG -= app_bundle

TARGET = spectral_explorer_tests
TEMPLATE = app

INCLUDEPATH += .. ../gui ../camera ../spectrum ../settings

unix {
    LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
}
win32 {
    INCLUDEPATH += C:/opencv-2-4-13/install/include/
    LIBS += -LC:/opencv-2-4-13/install/x86/mingw/lib/
    LIBS += -lopencv_core2413 -lopencv_highgui2413 -lopencv_imgproc2413
}

SOURCES += \
    test_spectral_explorer.cpp \
    ../camera/camera.cpp \
    ../camera/camera_cv.cpp \
    ../camera/camera_detector.cpp \
    ../camera/camera_disposer.cpp \
    ../camera/camera_dummy.cpp \
    ../camera/camera_file.cpp \
    ../camera/capture_worker.cpp \
    ../gui/config_dialog.cpp \
    ../gui/graph_viewer.cpp \
    ../gui/opencv_viewer.cpp \
    ../gui/settings.cpp \
    ../gui/spectrum_selector.cpp \
    ../settings/setting.cpp \
    ../spectrum/intensity_configuration.cpp \
    ../spectrum/spectrum.cpp \
    ../spectrum/spectrum_config.cpp \
    ../spectrum/spectrum_data.cpp \
    ../spectrum/spectrum_disposer.cpp \
    ../spectrum/spectrum_drawing_strategy.cpp \
    ../spectrum/spectrum_intensity_colored_drawing.cpp \
    ../spectrum/spectrum_intensity_drawing.cpp \
    ../spectrum/spectrum_optical_drawing.cpp \
    ../spectrum/spectrum_rgb_drawing.cpp

HEADERS += \
    ../camera/camera.h \
    ../camera/camera_cv.h \
    ../camera/camera_detector.h \
    ../camera/camera_disposer.h \
    ../camera/camera_dummy.h \
    ../camera/camera_file.h \
    ../camera/capture_worker.h \
    ../gui/config_dialog.h \
    ../gui/graph_viewer.h \
    ../gui/opencv_viewer.h \
    ../gui/settings.h \
    ../gui/spectrum_selector.h \
    ../settings/setting.h \
    ../spectrum/intensity_configuration.h \
    ../spectrum/measurement_target.h \
    ../spectrum/spectrum.h \
    ../spectrum/spectrum_config.h \
    ../spectrum/spectrum_data.h \
    ../spectrum/spectrum_disposer.h \
    ../spectrum/spectrum_drawing_strategy.h \
    ../spectrum/spectrum_intensity_colored_drawing.h \
    ../spectrum/spectrum_intensity_drawing.h \
    ../spectrum/spectrum_optical_drawing.h \
    ../spectrum/spectrum_rgb_drawing.h

FORMS += \
    ../gui/config_dialog.ui \
    ../gui/settings.ui
