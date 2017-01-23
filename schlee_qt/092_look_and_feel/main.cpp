#include "mainwindow.h"
#include "MyApplication.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyApplication myApplication;

    myApplication.show();

    return app.exec();
}
