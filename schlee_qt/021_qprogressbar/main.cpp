#include "mainwindow.h"
#include <QApplication>
#include "Progress.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Progress progress;

    progress.show();

    return app.exec();
}
