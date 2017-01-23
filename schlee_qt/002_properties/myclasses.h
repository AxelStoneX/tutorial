#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

class MyClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool readOnly             // Тип и имя свойства
               READ isReadOnly           // Имя метода чтения
               WRITE setReadOnly)        // Имя метода записи

private:
    bool m_bReadOnly;
public:
    MyClass(QObject *pobj = 0) : QObject(pobj), m_bReadOnly(false)
    {
    }

public:
    void setReadOnly (bool bReadOnly)
    {
        m_bReadOnly = bReadOnly;
    }
    bool isReadOnly() const
    {
        return m_bReadOnly;
    }

signals:
    void doIt();
    void sendString(const QString&);
public:
    void SendSignal()
    {
        emit doIt();
        emit sendString("Information");
    }
};

class MySlot : public QObject
{
    Q_OBJECT
public:
    MySlot(QObject *pobj = 0) : QObject(pobj)
    {
    }

public slots:
    void slot()
    {
        qDebug() << "I'm a slot";
    }
};
