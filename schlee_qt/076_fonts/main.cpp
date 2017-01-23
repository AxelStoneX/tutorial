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
        painter.setFont(QFont("Times", 25, QFont::Normal));
        painter.drawText(10, 40, "Draw Text");
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
        QRect r = QRect(0, 0, 120, 120);
        painter.setFont(QFont("Times", 25, QFont::Normal));
        painter.drawRect(r);
        painter.drawText(r, Qt::AlignCenter | Qt::TextWordWrap, "Draw Text");
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
        QLinearGradient gradient(0, 0, 500, 0);
        gradient.setColorAt(0, Qt::red);
        gradient.setColorAt(0.5, Qt::green);
        gradient.setColorAt(1, Qt::blue);

        QPainter painter(this);
        painter.setPen(QPen(gradient, 0));
        painter.setFont(QFont("Times", 50, QFont::Normal));
        painter.drawText(60, 60, "Gradient Text");
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleExampleWidget1 wgt1;
    SimpleExampleWidget2 wgt2;
    SimpleExampleWidget3 wgt3;
    wgt1.resize(170, 60);
    wgt2.resize(120, 120);
    wgt3.resize(500, 80);
    wgt1.show();
    wgt2.show();
    wgt3.show();
    return app.exec();
}
