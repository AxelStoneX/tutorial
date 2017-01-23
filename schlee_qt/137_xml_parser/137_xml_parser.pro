#-------------------------------------------------
#
# Project created by QtCreator 2016-10-18T17:25:40
#
#-------------------------------------------------

QT       += core gui\
            xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 137_xml_parser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

CONFIG   += console

OTHER_FILES += \
    addressbook.xml

RESOURCES += \
    rsr.qrc
