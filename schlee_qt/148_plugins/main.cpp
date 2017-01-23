#include "mainwindow.h"
#include <QApplication>
#include "PluginsWindow.h"
#include "Interfaces.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PluginsWindow* p_plw = new PluginsWindow();
    p_plw->show();

    return app.exec();
}
