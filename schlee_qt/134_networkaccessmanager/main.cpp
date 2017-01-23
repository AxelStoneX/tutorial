#include "mainwindow.h"
#include <QApplication>
#include "DownloaderGui.h"

int main(int argc, char *argv[])
{
    QApplication   app(argc, argv);
    DownloaderGui  downloader;

    downloader.show();

    return app.exec();
}
