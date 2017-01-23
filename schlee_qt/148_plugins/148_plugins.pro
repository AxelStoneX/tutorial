#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T12:55:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 148_plugins
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    PluginsWindow.cpp

HEADERS  += mainwindow.h \
    Interfaces.h \
    PluginsWindow.h

FORMS    += mainwindow.ui
