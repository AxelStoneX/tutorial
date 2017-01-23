#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTabWidget   tab;
    QStringList  lst;

    lst << "Linux" << "Windows" << "MacOS" << "OS2";
    foreach(QString str, lst)
        tab.addTab(new QLabel(str, &tab), QPixmap(":/" + str + ".png"), str);
    tab.resize(300, 100);
    tab.show();

    return app.exec();
}
