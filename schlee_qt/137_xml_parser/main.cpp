#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include <QtXml>

// ---------------------------------------------------------------------------
void traverseNode(const QDomNode& node)
{
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull())
    {
        if(domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull())
            {
                if(domElement.tagName() == "contact")
                {
                    qDebug() << "Attr: "
                             << domElement.attribute("number", "");
                }
                else
                {
                    qDebug() << "TagName: " << domElement.tagName()
                             << "\tText: " << domElement.text();
                }
            }
        }
        traverseNode(domNode);
        domNode = domNode.nextSibling();
    }
}

int main(int argc, char *argv[])
{
    QDomDocument domDoc;
    QFile        file(":/addressbook.xml");

    if(file.open(QIODevice::ReadOnly))
    {
        qDebug() << "file has been opened";
        if(!domDoc.setContent(&file))
        {
            qDebug() << "contents has been set";
            QDomElement domElement = domDoc.documentElement();
            traverseNode(domElement);
        }
        file.close();
    }

    return 0;
}
