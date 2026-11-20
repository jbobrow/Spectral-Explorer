HEADERS += \
    spectrum/spectrum.h \
    spectrum/spectrum_data.h \
    spectrum/spectrum_disposer.h \
    spectrum/spectrum_drawing_strategy.h \
    spectrum/spectrum_intensity_drawing.h \
    spectrum/spectrum_rgb_drawing.h \
    spectrum/spectrum_optical_drawing.h \
    $$PWD/spectrum_intensity_colored_drawing.h \
    $$PWD/spectrum_config.h \
    $$PWD/intensity_configuration.h \
    $$PWD/measurement_target.h

SOURCES += \
    spectrum/spectrum.cpp \
    spectrum/spectrum_data.cpp \
    spectrum/spectrum_disposer.cpp \
    spectrum/spectrum_drawing_strategy.cpp \
    spectrum/spectrum_intensity_drawing.cpp \
    spectrum/spectrum_rgb_drawing.cpp \
    spectrum/spectrum_optical_drawing.cpp \
    $$PWD/spectrum_intensity_colored_drawing.cpp \
    $$PWD/spectrum_config.cpp \
    $$PWD/intensity_configuration.cpp
