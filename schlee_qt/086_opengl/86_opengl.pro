#-------------------------------------------------
#
# Project created by QtCreator 2016-09-29T18:13:34
#
#-------------------------------------------------

QT       += core gui\
            opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 86_opengl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    OGLQuad.cpp

HEADERS  += mainwindow.h \
    OGLQuad.h

FORMS    += mainwindow.ui
