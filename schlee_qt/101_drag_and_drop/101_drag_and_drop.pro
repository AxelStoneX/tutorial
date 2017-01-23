#-------------------------------------------------
#
# Project created by QtCreator 2016-10-03T17:51:27
#
#-------------------------------------------------

QT       += core gui\
            widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 101_drag_and_drop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    Drag.h \
    Drop.h

FORMS    += mainwindow.ui

RESOURCES += \
    rsr.qrc
