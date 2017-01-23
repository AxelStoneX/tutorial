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
        QImage   img(":/test.png");
        painter.drawImage(0,0,img);
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
        QImage   img(":/test.png");
        painter.drawImage(0, 0, img, 50, 50, 70, 70);
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
        QImage img(":/test.png");
        painter.drawImage(0, 0, img);
        img.invertPixels(QImage::InvertRgb);
        painter.drawImage(img.width(), 0, img);
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
        QImage img1(":/test.png");
        painter.drawImage(0, 0, img1);

        QImage img2 = img1.scaled(img1.width() / 2, img1.height(), Qt::IgnoreAspectRatio);
        painter.drawImage(img1.width(), 0, img2);

        QImage img3 = img1.scaled(img1.width(), img1.height() / 2, Qt::IgnoreAspectRatio);
        painter.drawImage(0, img1.height(), img3);

        QImage img4 = img1.scaled(img1.width() / 2, img1.height(), Qt::KeepAspectRatio);
        painter.drawImage(img1.width(), img1.height(), img4);
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
        QImage img(":/test.png");
        painter.drawImage(0, 0, img);
        painter.drawImage(img.width(), 0, img.mirrored(true, true));
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleExampleWidget1 wgt1;
    SimpleExampleWidget1 wgt2;
    SimpleExampleWidget3 wgt3;
    SimpleExampleWidget4 wgt4;
    SimpleExampleWidget5 wgt5;
    wgt1.resize(250, 187);
    wgt2.resize(250, 187);
    wgt3.resize(500, 187);
    wgt4.resize(375, 280);
    wgt5.resize(500, 187);
    wgt1.show();
    wgt2.show();
    wgt3.show();
    wgt4.show();
    wgt5.show();

    return app.exec();
}
