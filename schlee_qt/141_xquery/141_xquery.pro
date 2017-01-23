#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T10:01:40
#
#-------------------------------------------------

QT       += core gui xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 141_xquery
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    _piggy.xq \
    _kermit.xq \
    _count.xq \
    _all.xq
