#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T16:07:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 116_mdi_app
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DocWindow.cpp \
    MDIProgram.cpp

HEADERS  += mainwindow.h \
    MDIProgram.h \
    DocWindow.h

FORMS    += mainwindow.ui
