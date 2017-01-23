#include "mainwindow.h"
#include <QApplication>
#include "UdpServer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    UdpServer*   pudp_srv = new UdpServer;

    pudp_srv->show();

    return app.exec();
}
