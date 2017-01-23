#ifndef KEYFILTER_H
#define KEYFILTER_H

#include <QtGui>

// ===========================================================================
class KeyFilter : public QObject
{
protected:
    bool eventFilter(QObject *pobj, QEvent *pe)
    {
        if (pe->type() == QEvent::KeyPress)
        {
            if (((QKeyEvent*)pe)->key() == Qt::Key_Z)
            {
                QKeyEvent* pe = new QKeyEvent(QEvent::KeyPress, Qt::Key_A,
                                              Qt::NoModifier, "A"         );
                QApplication::sendEvent(pobj, pe);
                return true;
            }
        }
        return false;
    }
public:
    KeyFilter(QObject *pobj = 0)
        :QObject(pobj)
    {
    }
};



#endif // KEYFILTER_H
