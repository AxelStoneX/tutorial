#include "MyWorker.h"

MyWorker::MyWorker(QObject* pobj)
    : QObject(pobj)
    , m_nValue(10)
{
    connect(&m_timer, SIGNAL(timeout()), SLOT(setNextValue()));
}

void MyWorker::doWork()
{
    m_timer.start(1000);
}

void MyWorker::setNextValue()
{
    emit valueChanged(--m_nValue);

    if(!m_nValue)
    {
        emit finished();
    }
}
