#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include "MyThread.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QProgressBar prb;
    MyThread     thread;

    QObject::connect(&thread, SIGNAL(progress(int)),
                     &prb,    SLOT(setValue(int)));

    prb.show();

    thread.start();

    return app.exec();
}
