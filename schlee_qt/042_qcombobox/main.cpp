#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QComboBox    cbo;
    QStringList  lst;

    lst << "John" << "Paul" << "George" << "Ringo";
    cbo.addItems(lst);
    cbo.setEditable(true);
    cbo.show();

    return app.exec();
}
