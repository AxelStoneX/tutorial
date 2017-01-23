#include <QApplication>
#include <QWidget>
#include <QPalette>
#include <QtGui>
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget      wgt;

    QWidget* pwgt1 = new QWidget(&wgt);
    QPalette pall;
    pall.setColor(pwgt1->backgroundRole(), Qt::blue);
    pwgt1->setPalette(pall);
    pwgt1->resize(100,100);
    pwgt1->move(25, 25);
    pwgt1->setAutoFillBackground(true);

    QWidget* pwgt2 = new QWidget(&wgt);
    QPalette pal2;
    pal2.setBrush(pwgt2->backgroundRole(), QBrush(QPixmap(":/rsr/stone.jpg")));
    pwgt2->setPalette(pal2);
    pwgt2->resize(100, 100);
    pwgt2->move(75, 75);
    pwgt2->setAutoFillBackground(true);

    wgt.resize(200, 200);
    wgt.show();

    return app.exec();
}
