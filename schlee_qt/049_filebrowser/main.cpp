#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSplitter    spl(Qt::Horizontal);
    QDirModel    model;

    QTreeView*   pTreeView = new QTreeView;
    pTreeView->setModel(&model);

    QTableView*  pTableView = new QTableView;
    pTableView->setModel(&model);

    QObject::connect(pTreeView,  SIGNAL(clicked(const QModelIndex&)),
                     pTableView, SLOT(setRootIndex(const QModelIndex&)));
    QObject::connect(pTableView, SIGNAL(activated(const QModelIndex&)),
                     pTreeView,  SLOT(setCurrentIndex(const QModelIndex&)));
    QObject::connect(pTableView, SIGNAL(activated(const QModelIndex&)),
                     pTreeView,  SLOT(setRootIndex(const QModelIndex&)));

    spl.addWidget(pTreeView);
    spl.addWidget(pTableView);
    spl.show();

    return app.exec();
}
