#-------------------------------------------------
#
# Project created by QtCreator 2016-09-30T17:51:01
#
#-------------------------------------------------

QT       += core gui\
            phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 98_phonon_mediaplayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MediPlayer.cpp

HEADERS  += mainwindow.h \
    MediaPlayer.h

FORMS    += mainwindow.ui
