#ifndef DRAG_H
#define DRAG_H

#include <QtWidgets>

// ===========================================================================
class Drag : public QLabel
{
    Q_OBJECT
private:
    QPoint m_ptDragPos;

    void startDrag()
    {
        QMimeData* pMimeData = new QMimeData;
        pMimeData->setText(text());

        QDrag* pDrag = new QDrag(this);
        pDrag->setMimeData(pMimeData);
        pDrag->setPixmap(QPixmap(":/img1.png"));
        pDrag->exec();
    }
protected:
    virtual void mousePressEvent(QMouseEvent* pe)
    {
        if (pe->button() == Qt::LeftButton)
            m_ptDragPos = pe->pos();
        QWidget::mousePressEvent(pe);
    }
    virtual void mouseMoveEvent(QMouseEvent* pe)
    {
        if(pe->buttons() & Qt::LeftButton)
        {
            int distance = (pe->pos() - m_ptDragPos).manhattanLength();
            if(distance > QApplication::startDragDistance())
                startDrag();
        }
        QWidget::mouseMoveEvent(pe);
    }
public:
    Drag(QWidget* pwgt = 0) : QLabel("This is draggable text", pwgt)
    {
    }
};


#endif // DRAG_H
