#include "mainwindow.h"
#include <QApplication>
#include "Shell.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Shell* pshell = new Shell;

    pshell->show();

    return app.exec();
}
