#include "mainwindow.h"
#include "MDIProgram.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MDIProgram* p_mdi_prog = new MDIProgram;
    p_mdi_prog->show();

    return app.exec();
}
