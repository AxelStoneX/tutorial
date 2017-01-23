#include "OGLDraw.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    OGLDraw      oglDraw;

    oglDraw.resize(400, 200);
    oglDraw.show();

    return app.exec();
}
