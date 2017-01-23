#include "mainwindow.h"
#include "Drag.h"
#include "Drop.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Drag* pdrg = new Drag();
    pdrg->show();

    Drop* pdrp = new Drop();
    pdrp->show();

    return app.exec();
}
