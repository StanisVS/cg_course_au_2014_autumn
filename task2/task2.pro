#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T12:07:20
#
#-------------------------------------------------
CONFIG +=  c++11
QMAKE_CXXFLAGS += -std=c++11

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = task2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    fragment.frag \
    vertex.vert
