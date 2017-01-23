#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include "MyServer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyServer     server(1234);

    server.show();

    return app.exec();
}
