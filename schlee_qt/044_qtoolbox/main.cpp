#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QToolBox     tbx;
    QStringList  lst;

    lst << "Linux" << "Windows" << "MacOS" << "OS2";
    foreach(QString str, lst)
        tbx.addItem(new QLabel(str, &tbx), QPixmap(":/" + str + ".png"), str);
    tbx.resize(100, 80);
    tbx.show();

    return app.exec();
}
