#include "mainwindow.h"
#include "DocWindow.h"
#include "SDIProgram.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SDIProgram p_sdi_prog;

    p_sdi_prog.show();

    return app.exec();
}
