#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T17:24:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 117_system_tray
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    SystemTray.cpp

HEADERS  += mainwindow.h \
    SystemTray.h

FORMS    += mainwindow.ui

RESOURCES += \
    rsr.qrc
