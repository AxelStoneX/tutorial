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

    QStringList lst(QStringList() << "one" << "two" << "three");
    QFuture<QString> future = QtConcurrent::mapped(lst.begin(), lst.end(), myToUpper);
    future.waitForFinished();
    qDebug() << future.results();

    return 0;
}
