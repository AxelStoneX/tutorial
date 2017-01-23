#-------------------------------------------------
#
# Project created by QtCreator 2016-09-30T10:16:57
#
#-------------------------------------------------

QT       += core gui\
            opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 88_opengl_3d
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    OGLPyramid.cpp

HEADERS  += mainwindow.h \
    OGLPyramid.h

FORMS    += mainwindow.ui
