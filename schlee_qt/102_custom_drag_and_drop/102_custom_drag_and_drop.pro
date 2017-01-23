#-------------------------------------------------
#
# Project created by QtCreator 2016-10-03T18:22:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 102_custom_drag_and_drop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Widget.cpp

HEADERS  += mainwindow.h \
    WidgetDrag.h \
    Widget.h

FORMS    += mainwindow.ui
