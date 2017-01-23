#-------------------------------------------------
#
# Project created by QtCreator 2016-09-12T17:12:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 03_signals_and_slots
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    counter.cpp

HEADERS  += mainwindow.h \
    Counter.h

FORMS    += mainwindow.ui
