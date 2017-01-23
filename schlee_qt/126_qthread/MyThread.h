#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QtWidgets>

class MyThread : public QThread
{
    Q_OBJECT
public:
    void run(){
        for (int i = 0; i <= 100; ++i)
        {
            usleep(100000);
            emit progress(i);
        }
    }
signals:
    void progress(int);
};

#endif // MYTHREAD_H
