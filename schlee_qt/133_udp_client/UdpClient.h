#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QtWidgets>
#include <QUdpSocket>

class UdpClient : public QTextEdit
{
    Q_OBJECT
private:
    QUdpSocket* m_pudp;

public:
    UdpClient(QWidget* pwgt = 0);

private slots:
    void slotProcessDatagrams();
};

#endif // UDPCLIENT_H
