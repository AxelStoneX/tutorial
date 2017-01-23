#include "mainwindow.h"
#include "Wizard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Wizard* pWizard = new Wizard();

    pWizard->show();

    return app.exec();
}
