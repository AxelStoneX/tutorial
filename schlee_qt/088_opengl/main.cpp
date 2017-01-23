#include "mainwindow.h"
#include "OGLPyramid.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    OGLPyramid   oglPyramid;

    oglPyramid.resize(200, 200);
    oglPyramid.show();

    return app.exec();
}
