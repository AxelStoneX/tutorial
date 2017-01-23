#include "mainwindow.h"
#include <QApplication>
#include "Clock.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Clock* pclk = new Clock;
    pclk->show();

    return a.exec();
}
