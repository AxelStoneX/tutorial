#ifndef COUNTER_H
#define COUNTER_H

#include <QObject>

//============================================================================
class Counter : public QObject
{
    Q_OBJECT
private:
    int m_nValue;
public:
    Counter();
public slots:
    void slotInc();

signals:
    void goodbye();               // Сигнал, сообщающий о конце работы счетчика
    void counterChanged(int);     // Сигнал, увеличивающий значение счетчика на единицу
};

#endif // COUNTER_H
