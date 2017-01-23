#include "Widget.h"
#include "WidgetDrag.h"

Widget::Widget(QWidget* pwgt) : QLabel(pwgt)
{
    setAcceptDrops(true);
}

void Widget::startDrag()
{
    WidgetDrag* pDrag = new WidgetDrag(this);
    pDrag->setWidget(this);
    pDrag->exec(Qt::CopyAction);
}

void Widget::mousePressEvent(QMouseEvent * pe)
{
    if (pe->button() == Qt::LeftButton)
        m_ptDragPos = pe->pos();
    QWidget::mousePressEvent(pe);
}

void Widget::mouseMoveEvent(QMouseEvent * pe)
{
    if (pe->buttons() & Qt::LeftButton)
    {
        int distance = (pe->pos() - m_ptDragPos).manhattanLength();
        if (distance > QApplication::startDragDistance())
            startDrag();
    }
    QWidget::mouseMoveEvent(pe);
}

void Widget::dragEnterEvent(QDragEnterEvent* pe)
{
    if (pe->mimeData()->hasFormat(WidgetMimeData::mimeType()))
        pe->acceptProposedAction();
}

void Widget::dropEvent(QDropEvent* pe)
{
    const WidgetMimeData* pmmd =
            dynamic_cast<const WidgetMimeData*>(pe->mimeData());
    if (pmmd)
    {
        QWidget* pwgt = pmmd->widget();
        QString str("Widget is dropped\n ObjectName:%1");
        setText(str.arg(pwgt->objectName()));
    }
}


