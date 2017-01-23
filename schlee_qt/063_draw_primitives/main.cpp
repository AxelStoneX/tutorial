#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

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
        painter.setPen(QPen(Qt::black, 3));
        int n = 8;
        for (int i = 0; i < n; ++i)
        {
            qreal fAngle = 2 * 3.14 * i / n;
            qreal x = 50 + cos(fAngle) * 40;
            qreal y = 50 + sin(fAngle) * 40;
            painter.drawPoint(QPointF(x, y));
        }
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
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.drawLine(QPointF(10, 10), QPointF(90, 90));
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
        painter.setRenderHint(QPainter::Antialiasing);

        int n = 8;
        QPointF a[n];

        for (int i = 0; i < n; ++i)
        {
            qreal fAngle = 2 * 3.14 * i / n;
            qreal x = 50 + cos(fAngle) * 40;
            qreal y = 50 + sin(fAngle) * 40;
            a[i] = QPointF(x, y);
        }
        painter.drawPolyline(a, n);
    }
};

class SimpleExampleWidget4 : public QWidget
{
public:
    SimpleExampleWidget4(QWidget *obj = 0)
        : QWidget(obj)
    {
    }
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        QBrush brush(Qt::red, Qt::Dense4Pattern);
        painter.fillRect(10, 10, 100, 100, brush);
        painter.eraseRect(20, 20, 80, 80);
    }
};

class SimpleExampleWidget5 : public QWidget
{
public:
    SimpleExampleWidget5(QWidget *obj = 0)
        : QWidget(obj)
    {
    }
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QBrush(Qt::red, Qt::DiagCrossPattern));
        painter.setPen(QPen(Qt::blue, 3, Qt::DashLine));
        painter.drawRect(QRect(10, 10 , 110, 70));
    }
};

class SimpleExampleWidget6 : public QWidget
{
public:
    SimpleExampleWidget6(QWidget *obj = 0)
        : QWidget(obj)
    {
    }
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(Qt::green));
        painter.setPen(QPen(Qt::black));
        painter.drawRoundRect(QRect(10, 10, 110, 70), 30, 30);
    }
};

class SimpleExampleWidget7 : public QWidget
{
public:
    SimpleExampleWidget7(QWidget *obj = 0)
        : QWidget(obj)
    {
    }
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(Qt::green, Qt::CrossPattern));
        painter.setPen(QPen(Qt::red, 3, Qt::DotLine));
        painter.drawEllipse(QRect(10, 10, 110, 70));
    }
};

class SimpleExampleWidget8 : public QWidget
{
public:
    SimpleExampleWidget8(QWidget *obj = 0)
        : QWidget(obj)
    {
    }
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QBrush(Qt::yellow));
        painter.setPen(QPen(Qt::blue));
        painter.drawChord(QRect(10, 10, 110, 70), 45 * 16, 180 * 16);
    }
};

class SimpleExampleWidget9 : public QWidget
{
public:
    SimpleExampleWidget9(QWidget *obj = 0)
        : QWidget(obj)
    {
    }
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QBrush(Qt::black, Qt::BDiagPattern));
        painter.setPen(QPen(Qt::blue, 4));
        painter.drawPie(QRect(10, 10, 110, 70), 90 * 16, 270 * 16);
    }
};

class SimpleExampleWidget10 : public QWidget
{
public:
    SimpleExampleWidget10(QWidget *obj = 0)
        : QWidget(obj)
    {
    }
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(Qt::lightGray));
        painter.setPen(QPen(Qt::black));

        int n = 8;
        QPolygonF polygon;
        for (int i = 0; i< n; ++i)
        {
            qreal fAngle = 2 * 3.14 * i/n;
            qreal x = 50 + cos(fAngle) * 40;
            qreal y = 50 + sin(fAngle) * 40;
            polygon << QPointF(x, y);
        }
        painter.drawPolygon(polygon);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleExampleWidget1 wgt1;
    SimpleExampleWidget2 wgt2;
    SimpleExampleWidget3 wgt3;
    SimpleExampleWidget4 wgt4;
    SimpleExampleWidget5 wgt5;
    SimpleExampleWidget6 wgt6;
    SimpleExampleWidget7 wgt7;
    SimpleExampleWidget8 wgt8;
    SimpleExampleWidget9 wgt9;
    SimpleExampleWidget10 wgt10;

    wgt1.resize(100, 100);
    wgt2.resize(100, 100);
    wgt3.resize(100, 100);
    wgt4.resize(120, 120);
    wgt5.resize(130, 90);
    wgt6.resize(130, 90);
    wgt7.resize(130, 90);
    wgt8.resize(130, 90);
    wgt9.resize(130, 90);
    wgt10.resize(100, 100);
    wgt1.show();
    wgt2.show();
    wgt3.show();
    wgt4.show();
    wgt5.show();
    wgt6.show();
    wgt7.show();
    wgt8.show();
    wgt9.show();
    wgt10.show();
    return app.exec();
}
