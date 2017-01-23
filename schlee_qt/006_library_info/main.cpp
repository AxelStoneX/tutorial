#include "mainwindow.h"
#include <QApplication>
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qDebug() << "Build date:"
             << QLibraryInfo::buildDate().toString("yyyy-MM-dd");
    qDebug() << "License Products:"
             << QLibraryInfo::licensedProducts();
    qDebug() << "Licensee:"
             << QLibraryInfo::licensee();

    qDebug() << "Locations";
    qDebug() << " Headers:"
             << QLibraryInfo::location(QLibraryInfo::HeadersPath);
    qDebug() << " Libraries:"
             << QLibraryInfo::location(QLibraryInfo::LibrariesPath);
    qDebug() << " Binaries:"
             << QLibraryInfo::location(QLibraryInfo::BinariesPath);
    qDebug() << " Prefix:"
             << QLibraryInfo::location(QLibraryInfo::PrefixPath);
    qDebug() << " Documentation:"
             << QLibraryInfo::location(QLibraryInfo::DocumentationPath);
    qDebug() << " Plugins:"
             << QLibraryInfo::location(QLibraryInfo::PluginsPath);
    qDebug() << " Data:"
             << QLibraryInfo::location(QLibraryInfo::DataPath);
    qDebug() << " Settings:"
             << QLibraryInfo::location(QLibraryInfo::SettingsPath);
    qDebug() << " Demos:"
             << QLibraryInfo::location(QLibraryInfo::ExamplesPath);

    return a.exec();
}
