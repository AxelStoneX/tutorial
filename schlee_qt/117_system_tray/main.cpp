#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include "SystemTray.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SystemTray st;

    QApplication::setQuitOnLastWindowClosed(false);

    return app.exec();
}
