TEMPLATE = lib
CONFIG  += plugin
QT      -= gui
DESTDIR  = ../plugins
SOURCES  = MyPlugin.cpp \
    MyPlugin.cpp
HEADERS  = MyPlugin.h \
           ../148_plugins/Interfaces.h \
    MyPlugin.h
TARGET   = myplugin
