#ifndef CLOCK_H
#define CLOCK_H

#include <QtWidgets>

// ===========================================================================
class Clock : public QLabel
{
    Q_OBJECT
public:
    Clock(QWidget* pwgt = 0) : QLabel(pwgt)
    {
        QTimer* ptimer = new QTimer(this);
        connect(ptimer, SIGNAL(timeout()), SLOT(slotUpdateDateTime()));
        ptimer->start(500);
        slotUpdateDateTime();
    }

public slots:
    void slotUpdateDateTime()
    {
        QString str = QDateTime::currentDateTime().toString(Qt::SystemLocaleDate);
        setText("<H2><CENTER>" + str + "</CENTER></H1>");
    }
};

#endif // CLOCK_H
