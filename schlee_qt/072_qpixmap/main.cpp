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
        QPixmap pix(":/test.png");
        painter.drawPixmap(0, 0, pix);

        QRect r(pix.width(), 0, pix.width()/2, pix.height());
        painter.drawPixmap(r, pix);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleExampleWidget1 wgt1;
    wgt1.resize(375, 185);
    wgt1.show();

    return app.exec();
}
