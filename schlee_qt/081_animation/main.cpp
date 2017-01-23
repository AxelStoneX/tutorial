#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel lbl;
    QMovie mov(":/animation.gif");

    lbl.setMovie(&mov);
    lbl.resize(100, 100);
    lbl.show();
    mov.start();

    return app.exec();
}
