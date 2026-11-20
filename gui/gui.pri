INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
SOURCES += gui/graph_viewer.cpp \
    gui/dock.cpp \
    gui/dock_container.cpp \
    gui/empty_dock.cpp \
    gui/spectrum_graph_dock.cpp \
    gui/spectrum_optical_dock.cpp \
    gui/spectrometer_window.cpp \
    gui/opencv_viewer.cpp \
    gui/spectrum_combination_dock.cpp \
    gui/spectrum_selector.cpp \
    $$PWD/config_dialog.cpp \
    $$PWD/settings.cpp \
    $$PWD/export_dialog.cpp \
    $$PWD/about_box.cpp
HEADERS += gui/graph_viewer.h \
    gui/dock.h \
    gui/dock_container.h \
    gui/empty_dock.h \
    gui/spectrum_graph_dock.h \
    gui/spectrum_optical_dock.h \
    gui/spectrometer_window.h \
    gui/opencv_viewer.h \
    gui/spectrum_combination_dock.h \
    gui/spectrum_selector.h \
    $$PWD/config_dialog.h \
    $$PWD/settings.h \
    $$PWD/about_box.h \
    $$PWD/export_dialog.h \

FORMS += \
    gui/dock_container.ui \
    gui/spectrum_graph_dock.ui \
    gui/spectrum_optical_dock.ui \
    gui/spectrometer_window.ui \
    gui/spectrum_combination_dock.ui \
    $$PWD/config_dialog.ui \
    $$PWD/settings.ui \
    $$PWD/about_box.ui \
    $$PWD/export_dialog.ui
