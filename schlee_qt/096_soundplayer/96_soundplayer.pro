#-------------------------------------------------
#
# Project created by QtCreator 2016-09-30T16:20:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 96_soundplayer
TEMPLATE = app

x11:REQUIRES = nas

SOURCES += main.cpp\
        mainwindow.cpp \
    SoundPlayer.cpp

HEADERS  += mainwindow.h \
    SoundPlayer.h

FORMS    += mainwindow.ui

RESOURCES += \
    rsr.qrc
