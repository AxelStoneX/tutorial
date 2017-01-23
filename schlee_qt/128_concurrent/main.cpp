#include "mainwindow.h"
#include <QApplication>
#include <QtCore>
#include <QtConcurrent/QtConcurrent>

QString myToUpper(const QString& str)
{
    return str.toUpper();
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QFuture<QString> future = QtConcurrent::run(myToUpper, QString("test"));
    future.waitForFinished();
    qDebug() << future.result();

    return 0;
}
