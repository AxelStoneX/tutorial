#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel       lbl("Text here");

    QTimer::singleShot(5 * 1000, &app, SLOT(quit()));

    lbl.show();

    return app.exec();
}
