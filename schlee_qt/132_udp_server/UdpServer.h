#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QUdpSocket>
#include <QtWidgets>

class UdpServer : public QTextEdit
{
    Q_OBJECT
private:
    QUdpSocket* m_pudp;

public:
    UdpServer(QWidget* pwgt = 0);

private slots:
    void slotSendDatagram();
};

#endif // UDPSERVER_H
