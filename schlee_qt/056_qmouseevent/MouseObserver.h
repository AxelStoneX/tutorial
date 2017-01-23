#ifndef MOUSEOBSERVER_H
#define MOUSEOBSERVER_H

#include <QtGui>

// ===========================================================================
class MouseObserver : public QLabel
{
public:
    MouseObserver(QWidget* pwgt = 0);
protected:
    virtual void mousePressEvent  (QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent   (QMouseEvent *pe);

    void    dumpEvent     (QMouseEvent *pe, const QString& strMessage);
    QString modifiersInfo (QMouseEvent *pe);
    QString buttonsInfo   (QMouseEvent *pe);
};

#endif // MOUSEOBSERVER_H
