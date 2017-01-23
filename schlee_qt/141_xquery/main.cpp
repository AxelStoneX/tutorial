#include "mainwindow.h"
#include <QApplication>
#include <QtXmlPatterns>
#include <QtCore>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    if (argc < 3){
        qDebug() << "usage: XQuery any.xml any.xq";
        return 0;
    }
    QString strQuery = "";
    QFile xqFile(argv[2]);
    if(xqFile.open(QIODevice::ReadOnly))
    {
        strQuery = xqFile.readAll();
        xqFile.close();
    }
    else
    {
        qDebug() << "Can't open the file for reading:" << argv[1];
        return 0;
    }

    QFile xmlFile(argv[1]);
    if(xmlFile.open(QIODevice::ReadOnly))
    {
        QXmlQuery query;
        query.bindVariable("inputDocument", &xmlFile);
        query.setQuery(strQuery);
        if (!query.isValid())
        {
            qDebug() << "Can't evaluate the query";
            return 0;
        }

        QString strOutput;
        if (!query.evaluateTo(&strOutput))
        {
            qDebug() << "Can't evaluate the query";
            return 0;
        }

        xmlFile.close();
        qDebug() << strOutput;
    }

    return app.exec();
}
