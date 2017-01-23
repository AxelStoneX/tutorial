#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include "GrabWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GrabWidget wgt;

    wgt.show();

    return app.exec();
}
