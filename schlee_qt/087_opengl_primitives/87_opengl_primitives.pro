#-------------------------------------------------
#
# Project created by QtCreator 2016-09-29T18:32:45
#
#-------------------------------------------------

QT       += core gui\
            opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 87_opengl_primitives
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    OGLDraw.cpp

HEADERS  += mainwindow.h \
    OGLDraw.h

FORMS    += mainwindow.ui
