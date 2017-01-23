#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel lbl;

    lbl.setPixmap(QPixmap(":/check.png"));

    QGraphicsColorizeEffect effect;
    lbl.setGraphicsEffect(&effect);

    QPropertyAnimation anim(&effect, "color");
    anim.setStartValue(Qt::gray);
    anim.setKeyValueAt(0.25f, Qt::green);
    anim.setKeyValueAt(0.5f, Qt::blue);
    anim.setKeyValueAt(0.75f, Qt::red);
    anim.setEndValue(Qt::black);
    anim.setDuration(3000);
    anim.setLoopCount(-1);
    anim.start();

    lbl.show();

    return app.exec();
}
