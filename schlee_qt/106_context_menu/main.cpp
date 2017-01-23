//#pragma once

#include "mainwindow.h"
#include "ContextMenu.h"
#include <QApplication>
#include <QtWidgets>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ContextMenu*  cntMenu = new ContextMenu;
    cntMenu->show();

    return app.exec();
}
