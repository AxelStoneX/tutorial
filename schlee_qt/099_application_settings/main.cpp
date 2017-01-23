#include "mainwindow.h"
#include "MyProgram.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyProgram    myProgram;

    myProgram.show();

    return app.exec();
}
