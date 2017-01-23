#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

class SimpleExampleWidget1 : public QWidget
{
public:
    SimpleExampleWidget1(QWidget *obj = 0)
        : QWidget(obj)
    {
    }
    void paintEvent(QPaintEvent *)
    {
        QPixmap pix(size());
        QPainter painter;

        painter.begin(&pix);
        painter.drawPixmap(rect(), QPixmap(":/test.png"));
        painter.end();

        QBitmap bmp(size());
        bmp.fill();

        painter.begin(&bmp);
        painter.setPen(QPen(Qt::color1));
        painter.setFont(QFont("Times", 75, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "Draw Text");
        painter.end();

        pix.setMask(bmp);

        painter.begin(this);
        painter.drawPixmap(rect(), pix);
        painter.end();
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleExampleWidget1 wgt1;
    wgt1.resize(450, 185);
    wgt1.show();
    return app.exec();
}
