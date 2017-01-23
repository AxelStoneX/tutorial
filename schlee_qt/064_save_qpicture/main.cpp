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
        QPicture pic;
        QPainter painter;

        painter.begin(&pic);
        painter.drawLine(20, 20, 50, 50);
        painter.end();

        if(!pic.save("myline.dat")){
            qDebug() << "can not save the file";
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
        QPicture pic;
        if(!pic.load("myline.dat")){
            qDebug() << "can not load the file";
        }
        QPainter painter;
        painter.begin(this);
        painter.drawPicture(QPoint(0, 0), pic);
        painter.end();

    }
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleExampleWidget1 wgt1;
    SimpleExampleWidget2 wgt2;
    wgt2.resize(70, 70);
    wgt2.show();
    return app.exec();
}
