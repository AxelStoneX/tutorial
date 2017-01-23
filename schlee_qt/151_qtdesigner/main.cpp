#include <QApplication>
#include "LoadMyForm.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LoadMyForm wgt;

    wgt.show();

    return app.exec();
}
