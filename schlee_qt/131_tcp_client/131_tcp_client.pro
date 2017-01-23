#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T13:37:09
#
#-------------------------------------------------

QT       += core gui\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 131_tcp_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MyClient.cpp

HEADERS  += mainwindow.h \
    MyClient.h

FORMS    += mainwindow.ui
