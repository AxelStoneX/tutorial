#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T14:45:26
#
#-------------------------------------------------

QT       += core gui\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 133_udp_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    UdpClient.cpp

HEADERS  += mainwindow.h \
    UdpClient.h

FORMS    += mainwindow.ui
