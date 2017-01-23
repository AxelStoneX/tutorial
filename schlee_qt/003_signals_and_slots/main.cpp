#include <QtGui>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include "Counter.h"

int main (int argc, char** argv)
{
    QApplication app(argc, argv);

    QLabel  lbl("0");           // Создаем объект надписи lbl
    QPushButton cmd("ADD");     // Нажимающаяся кнопка cmd
    Counter counter;            // Объект счетчика counter

    lbl.show();
    cmd.show();

    QObject::connect(&cmd, SIGNAL(clicked()),
                     &counter, SLOT(slotInc()));
    QObject::connect(&counter, SIGNAL(counterChanged(int)),
                     &lbl, SLOT(setNum(int)));
    QObject::connect(&counter, SIGNAL(goodbye()),
                     &app, SLOT(quit()));
    return app.exec();
}
