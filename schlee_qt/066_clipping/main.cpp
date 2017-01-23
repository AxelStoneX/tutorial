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
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setClipRect(0, 0, 100, 100);
        painter.setBrush(QBrush(Qt::green));
        painter.setPen(QPen(Qt::black, 2));
        painter.drawEllipse(0, 0, 200, 100);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleExampleWidget1 wgt1;
    wgt1.resize(220, 110);
    wgt1.show();
    return app.exec();
}
