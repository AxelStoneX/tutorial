#include "Printer.h"
#include <QtGui>

// ---------------------------------------------------------------------------
Printer::Printer(QWidget *pwgt) : QWidget(pwgt)
{
    m_pprinter = new QPrinter;
}

// ---------------------------------------------------------------------------
Printer::~Printer()
{
    delete m_pprinter;
}

void Printer::paintEvent(QPaintEvent*)
{
    draw(this);
}

void Printer::slotPrint()
{
    QPrintDialog dlg(m_pprinter, this);

    dlg.setMinMax(1, 1);
    if (dlg.exec() == QDialog::Accepted)
        draw(m_pprinter);
}

void Printer::draw(QPaintDevice *ppd)
{
    QPainter painter(ppd);
    QRect r(painter.viewport());

    painter.setBrush(Qt::white);
    painter.drawRect(r);
    painter.drawLine(0, 0, r.width(), r.height());
    painter.drawLine(r.width(), 0, 0, r.height());

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::red, 3, Qt::DashLine));
    painter.drawEllipse(r);

    painter.setPen(Qt::blue);
    painter.setFont(QFont("Times", 20, QFont::Normal));
    painter.drawText(r, Qt::AlignCenter, "Printer Test");
}
