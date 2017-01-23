#include "mainwindow.h"
#include <QApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTcpServer   tcpServer;
    int          nPort = 2424;

    if(!tcpServer.listen(QHostAddress::Any, nPort))
    {
        qDebug() << "Can't listen on port: " << nPort;
        return 0;
    }

    forever
    {
        while(tcpServer.waitForNewConnection(60000))
        {
            do
            {
                QTcpSocket* pSocket = tcpServer.nextPendingConnection();
                QString strDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
                pSocket->write(strDateTime.toLatin1());
                pSocket->flush();
                qDebug() << "Server date & time:" + strDateTime;
                pSocket->disconnectFromHost();
                if (pSocket->state() == QAbstractSocket::ConnectedState)
                    pSocket->waitForDisconnected();
                delete pSocket;
            } while (tcpServer.hasPendingConnections());
        }
    }

    return 0;
}
