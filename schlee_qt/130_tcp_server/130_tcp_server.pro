#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T12:59:19
#
#-------------------------------------------------

QT       += core gui\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 130_tcp_server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MyServer.cpp

HEADERS  += mainwindow.h \
    MyServer.h

FORMS    += mainwindow.ui
