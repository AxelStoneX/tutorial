#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

void mousePress(QWidget*         pwgt,
                int              x,
                int              y,
                Qt::MouseButton  bt = Qt::LeftButton,
                Qt::MouseButtons bts = Qt::LeftButton)
{
    if(pwgt)
    {
        QMouseEvent* pePress =
            new QMouseEvent(QEvent::MouseButtonPress, QPoint(x, y),
                            bt, bts, Qt::NoModifier              );
        QApplication::postEvent(pwgt, pePress);
    }
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPushButton* pcmd = new QPushButton("Push Button");
    pcmd->resize(100,25);
    pcmd->show();
    mousePress(pcmd, 50, 13);

    return app.exec();
}
