#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication     app(argc, argv);
    QListWidget      lwg;
    QListWidgetItem* pitem = 0;
    QStringList      lst;

    lwg.setIconSize(QSize(50, 50));
    lwg.setSelectionMode(QAbstractItemView::MultiSelection);
    lwg.setViewMode(QListView::IconMode);
    lst << "Linux" << "Windows" << "MacOS" << "OS2";
    foreach(QString str, lst)
    {
        pitem = new QListWidgetItem(str, &lwg);
        pitem->setIcon(QPixmap(":/" + str + ".png"));
        pitem->setFlags(Qt::ItemIsEnabled  | Qt::ItemIsSelectable |
                        Qt::ItemIsEditable | Qt::ItemIsDragEnabled );
    }
    lwg.resize(135, 145);
    lwg.show();

    return app.exec();
}
