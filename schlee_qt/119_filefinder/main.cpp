#include "mainwindow.h"
#include "FileFinder.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FileFinder* filewgt = new FileFinder(0);
    filewgt->show();
    return app.exec();
}
