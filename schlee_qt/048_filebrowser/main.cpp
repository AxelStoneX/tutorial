#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDirModel    model;
    QTreeView    treeView;

    treeView.setModel(&model);
    treeView.show();

    return app.exec();
}
