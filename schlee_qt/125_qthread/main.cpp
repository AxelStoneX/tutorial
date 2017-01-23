#include "mainwindow.h"
#include <QApplication>
#include "MyThread.h"
#include "MyWorker.h"
#include <QtWidgets>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLCDNumber   lcd;
    MyThread     thread;
    MyWorker     worker;

    QObject::connect(&worker, SIGNAL(valueChanged(int)),
                     &lcd,    SLOT(display(int)));

    lcd.setSegmentStyle(QLCDNumber::Filled);
    lcd.display(10);
    lcd.resize(220, 90);
    lcd.show();

    worker.moveToThread(&thread);
    QObject::connect(&worker, SIGNAL(finished()),
                     &thread, SLOT(quit()));
    QObject::connect(&thread, SIGNAL(finished()),
                     &app,    SLOT(quit()));

    thread.start();
    worker.doWork();


    return app.exec();
}
