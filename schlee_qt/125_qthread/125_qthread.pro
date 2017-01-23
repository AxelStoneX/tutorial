#-------------------------------------------------
#
# Project created by QtCreator 2016-10-17T17:18:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 125_qthread
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MyWorker.cpp

HEADERS  += mainwindow.h \
    MyWorker.h \
    MyThread.h

FORMS    += mainwindow.ui
