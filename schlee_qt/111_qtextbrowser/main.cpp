#include "mainwindow.h"
#include "HelpBrowser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    HelpBrowser helpBrowser(":/", "index.htm");
    helpBrowser.resize(400, 300);
    helpBrowser.show();

    return app.exec();
}
