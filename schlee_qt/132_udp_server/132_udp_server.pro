#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T14:32:46
#
#-------------------------------------------------

QT       += core gui\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 132_udp_server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    UdpServer.cpp

HEADERS  += mainwindow.h \
    UdpServer.h

FORMS    += mainwindow.ui
