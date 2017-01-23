#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel       lbl("this is the example text");
    QLibrary     lib("dynlib");
    typedef QString (*Fct) (const QString&);
    Fct fct = (Fct)(lib.resolve("oddUpper"));
    if(fct)
    {
        lbl.setText(fct(lbl.text()));
    }
    else
    {
        qDebug() << "Library was not resolved";
    }
    lbl.show();


    return app.exec();
}
