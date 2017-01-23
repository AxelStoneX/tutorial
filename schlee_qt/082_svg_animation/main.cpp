#include "mainwindow.h"
#include <QApplication>
#include <QtSvg>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSvgWidget *svg = new QSvgWidget(":/prefix/linux.svg");
    svg->show();

    /* QObject::connect(svg.renderer(), SIGNAL(repaintNeeded()),
                     &svg, SLOT(repaint())                  ); */

    return app.exec();
}
