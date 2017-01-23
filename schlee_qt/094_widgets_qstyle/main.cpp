#include "mainwindow.h"
#include <QApplication>
#include <QtGui>


class SimpleExampleWidget1 : public QLabel
{
public:
    SimpleExampleWidget1(QWidget *obj = 0)
        : QLabel(obj)
    {
    }
    void paintEvent(QPaintEvent *)
    {
        QStylePainter painter(this);
        QStyleOptionButton option;

        option.initFrom(this);
        option.text = "This is a label";

        painter.drawControl(QStyle::CE_PushButton, option);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleExampleWidget1 wgt1;
    wgt1.resize(200, 50);
    wgt1.show();
    return app.exec();
}
