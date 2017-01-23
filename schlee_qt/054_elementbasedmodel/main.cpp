#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication     app(argc, argv);
    QStringList      lst;
    QListWidget      lwg;
    QListWidgetItem* pitem = 0;

    lwg.setIconSize(QSize(50, 50));
    lst << "Linux" << "Windows" << "MacOS" << "OS2";
    foreach(QString str, lst)
    {
        pitem = new QListWidgetItem(str, &lwg);
        pitem->setIcon(QPixmap(":/" + str +".png"));
    }
    lwg.setWindowTitle("ListWidget");
    lwg.show();

    QListView listView;
    listView.setModel(lwg.model());
    listView.setSelectionModel(lwg.selectionModel());
    listView.setWindowTitle("ListView");
    listView.show();

    return app.exec();
}
