QT += widgets
QT += multimedia multimediawidgets

CONFIG += c++14
CONFIG += app_bundle
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        afvideoplayer.cpp \
        main.cpp

HEADERS += \
    ASCIIVideoConverter.h \
    VideoFrameProcessor.h \
    afvideoplayer.h
