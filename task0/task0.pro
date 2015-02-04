#-------------------------------------------------
#
# Project created by QtCreator 2015-01-30T20:50:51
#
#-------------------------------------------------
CONFIG +=  c++11

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = task0
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h

FORMS    += mainwindow.ui

DISTFILES += \
    1.vert

OTHER_FILES += \
    1.vert \
    2.vert \
    3.vert \
    1.frag \
    2.frag \
    3.frag \
    pal.ppm

RESOURCES += \
    shaders.qrc
