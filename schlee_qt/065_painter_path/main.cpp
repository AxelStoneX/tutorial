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
        QPainterPath path;
        QPointF pt1(width()  , height()/2 );
        QPointF pt2(width()/2, -height()  );
        QPointF pt3(width()/2, 2*height() );
        QPointF pt4(0        , height()/2 );
        path.moveTo(pt1);
        path.cubicTo(pt2, pt3, pt4);

        QRect rect(width()/4, height()/4, width()/2, height()/2);
        path.addRect(rect);
        path.addEllipse(rect);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::blue, 6));
        painter.drawPath(path);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleExampleWidget1 wgt1;
    wgt1.resize(200, 200);
    wgt1.show();
    return app.exec();
}
