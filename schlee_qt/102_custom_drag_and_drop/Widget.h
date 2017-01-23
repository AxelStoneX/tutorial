#ifndef WIDGET_H
#define WIDGET_H

#include <QPoint>
#include <QLabel>

// ===========================================================================
class Widget : public QLabel
{
    Q_OBJECT
private:
    QPoint m_ptDragPos;

    void startDrag();

protected:
    virtual void mousePressEvent(QMouseEvent*    );
    virtual void mouseMoveEvent (QMouseEvent*    );
    virtual void dragEnterEvent (QDragEnterEvent*);
    virtual void dropEvent      (QDropEvent*     );

public:
    Widget(QWidget* pwgt = 0);
};

#endif // WIDGET_H
