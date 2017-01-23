#include "mainwindow.h"
#include <QApplication>
#include "UdpClient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    UdpClient* p_udpclnt = new UdpClient;

    p_udpclnt->show();

    return app.exec();
}
