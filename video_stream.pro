#-------------------------------------------------
#
# Project created by QtCreator 2020-01-09T00:08:31
#
#-------------------------------------------------

QT       += core gui widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = video_stream
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    src/main_window.cpp \
    src/video_player.cpp \
    src/video_widget/frame_emitter.cpp \
    src/video_widget/gaussian_blur.cpp \
    src/video_widget/output_video_surface.cpp \
    src/video_widget/sobel_operator.cpp

HEADERS += \
    src/main_window.h \
    src/video_player.h \
    src/video_widget/frame_emitter.h \
    src/video_widget/gaussian_blur.h \
    src/video_widget/output_video_surface.h \
    src/video_widget/sobel_operator.h

