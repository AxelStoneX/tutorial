#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTreeWidget twg;
    QStringList lst;

    lst << "Folders" << "Used Space";
    twg.setHeaderLabels(lst);
    twg.setSortingEnabled(true);

    QTreeWidgetItem* ptwgItem = new QTreeWidgetItem(&twg);
    ptwgItem->setText(0, "Local Disk(C)");
    ptwgItem->setIcon(0, QPixmap(":/rsr/drive.jpg"));

    QTreeWidgetItem* ptwgItemDir = 0;
    for(int i = 1; i < 20; ++i)
    {
        ptwgItemDir = new QTreeWidgetItem(ptwgItem);
        ptwgItemDir->setText(0, "Directory" + QString::number(i));
        ptwgItemDir->setText(1, QString::number(i) + "MB");
        ptwgItemDir->setIcon(0, QPixmap(":/rsr/folder.png"));
        if (i == 18)
        {
            qDebug() << "Catched";
            delete ptwgItemDir;
        }
    }
    twg.setItemExpanded(ptwgItem, true);

    twg.resize(250, 110);
    twg.show();

    return app.exec();
}
