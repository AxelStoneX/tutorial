#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QWidget>

// ===========================================================================
class MyApplication : public QWidget
{
    Q_OBJECT
public:
    MyApplication(QWidget* pwgt = 0);

public slots:
    void slotChangeStyle(const QString& str);
};

#endif // MYAPPLICATION_H
