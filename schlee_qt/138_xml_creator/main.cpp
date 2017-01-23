#include "mainwindow.h"
#include <QApplication>
#include <QtXml>

QDomElement makeElement(QDomDocument& domDoc,
                        const QString&  strName,
                        const QString& strAttr = QString(),
                        const QString& strText = QString())
{
    QDomElement domElement = domDoc.createElement(strName);

    if (!strAttr.isEmpty())
    {
        QDomAttr domAttr = domDoc.createAttribute("number");
        domAttr.setValue(strAttr);
        domElement.setAttributeNode(domAttr);
    }

    if(!strText.isEmpty())
    {
        QDomText domText = domDoc.createTextNode(strText);
        domElement.appendChild(domText);
    }
    return domElement;
}

QDomElement contact (      QDomDocument& domDoc,
                     const QString&      strName,
                     const QString&      strPhone,
                     const QString&      strEmail)
{
    static int nNumber = 1;

    QDomElement domElement = makeElement(domDoc,
                                         "contact",
                                         QString().setNum(nNumber));
    domElement.appendChild(makeElement(domDoc, "name", "", strName));
    domElement.appendChild(makeElement(domDoc, "phone", "", strPhone));
    domElement.appendChild(makeElement(domDoc, "email", "", strEmail));

    nNumber++;

    return domElement;
}

int main(int argc, char *argv[])
{
    QDomDocument doc("addressbook");
    QDomElement domElement = doc.createElement("addressbook");
    doc.appendChild(domElement);

    QDomElement contact1 = contact(doc, "Piggy", "+49631322187", "piggy@mega.de");
    QDomElement contact2 = contact(doc, "Kermit", "+49631322181", "kermit@mega.de");
    QDomElement contact3 = contact(doc, "Gonzo", "+49631322186", "gonzo@mega.de");

    domElement.appendChild(contact1);
    domElement.appendChild(contact2);
    domElement.appendChild(contact3);

    QFile file("addressbook.xml");
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream(&file) << doc.toString();
        file.close();
    }

    return 0;
}
