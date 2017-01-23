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
        QImage img(size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter;

        painter.begin(&img);
        painter.initFrom(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.eraseRect(rect());
        painter.drawEllipse(0,0,size().width(), size().height());
        painter.end();

        painter.begin(this);
        painter.drawImage(0, 0, img);
        painter.end();
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleExampleWidget1 wgt1;
    wgt1.resize(500, 250);
    wgt1.show();

    return app.exec();
}
