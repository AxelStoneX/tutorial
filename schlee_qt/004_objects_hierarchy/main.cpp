#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QObject* pobj1 = new QObject;
    QObject* pobj2 = new QObject(pobj1);
    QObject* pobj4 = new QObject(pobj2);
    QObject* pobj3 = new QObject(pobj1);
    pobj2->setObjectName("the first child of pobj1");
    pobj3->setObjectName("the second child of pobj1");
    pobj4->setObjectName("the first child of pobj2");

    for (QObject* pobj = pobj4; pobj; pobj = pobj->parent())
        qDebug() << pobj->objectName();
    qDebug() << pobj2->metaObject()->className();

    return a.exec();
}
