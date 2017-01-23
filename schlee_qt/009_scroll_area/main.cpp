#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QScrollArea>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QScrollArea sa;

    QWidget* pwgt = new QWidget;
    QPixmap  pix(":/1/img.jpg");

    QPalette pal;
    pal.setBrush(pwgt->backgroundRole(), QBrush(pix));
    pwgt->setPalette(pal);
    pwgt->setAutoFillBackground(true);
    pwgt->setFixedSize(pix.width(), pix.height());

    sa.setWidget(pwgt);
    sa.resize(350,150);
    sa.show();

    return app.exec();
}

