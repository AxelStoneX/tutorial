#include "mainwindow.h"
#include "myclasses.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    MyClass pSender;
    MySlot  pReceiver;

    QObject::connect(&pSender,   SIGNAL(sendString(const QString&)),
                     &pReceiver, SLOT(slot()));

    return a.exec();
}


