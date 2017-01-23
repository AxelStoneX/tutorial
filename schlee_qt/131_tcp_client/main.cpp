#include "mainwindow.h"
#include <QApplication>
#include "MyClient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyClient     client("localhost", 1234);

    client.show();

    return app.exec();
}
