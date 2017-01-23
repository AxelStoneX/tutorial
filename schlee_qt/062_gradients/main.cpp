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
        QLinearGradient gradient(0, 0, width(), height());
        gradient.setColorAt(0, Qt::red);
        gradient.setColorAt(0.5, Qt::green);
        gradient.setColorAt(1, Qt::blue);
        painter.setBrush(gradient);

        painter.drawRect(rect());
    }
};

class SimpleExampleWidget2 : public QWidget
{
public:
    SimpleExampleWidget2(QWidget *obj = 0)
        : QWidget(obj)
    {
    }
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        QConicalGradient gradient(width() / 2, height() / 2, 0);
        gradient.setColorAt(0, Qt::red);
        gradient.setColorAt(0.4,Qt::green);
        gradient.setColorAt(0.8, Qt::blue);
        gradient.setColorAt(1, Qt::red);
        painter.setBrush(gradient);
        painter.drawRect(rect());

    }
};

class SimpleExampleWidget3 : public QWidget
{
public:
    SimpleExampleWidget3(QWidget *obj = 0)
        : QWidget(obj)
    {
    }
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        QPointF  ptCenter(rect().center());
        QRadialGradient gradient(ptCenter, width()/2, ptCenter);
        gradient.setColorAt(0, Qt::red);
        gradient.setColorAt(0.5, Qt::green);
        gradient.setColorAt(1, Qt::blue);
        painter.setBrush(gradient);
        painter.drawRect(rect());
    }
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SimpleExampleWidget1 wgt1;
    SimpleExampleWidget2 wgt2;
    SimpleExampleWidget3 wgt3;
    wgt1.show();
    wgt2.show();
    wgt3.show();

    return app.exec();
}
