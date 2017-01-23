    #include "mainwindow.h"
#include <QApplication>
#include <QtXml>

int main(int argc, char *argv[])
{
    QFile file("./../140_qxmlstreamreader/addressbook.xml");
    if(file.open(QIODevice::ReadOnly))
    {
        QXmlStreamReader sr(&file);
        do
        {
            sr.readNext();
            qDebug() << sr.tokenString() << sr.name() << sr.text();
        } while(!sr.atEnd());

        if(sr.hasError())
        {
            qDebug() << "Error:" << sr.errorString();
        }

        file.close();
    }

    return 0;
}
