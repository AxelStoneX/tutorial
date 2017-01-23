#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget*     wgt    = new QWidget();
    QMenuBar*    mnuBar = new QMenuBar(wgt);
    QMenu*       pmnu   = new QMenu("&Menu");

    pmnu->addAction("&About Qt", &app, SLOT(aboutQt()),Qt::CTRL + Qt::Key_Q);
    pmnu->addSeparator();

    QAction* pCheckableAction = pmnu->addAction("&CheckableItem");
    pCheckableAction->setCheckable(true);
    pCheckableAction->setChecked(true);

    pmnu->addAction(QPixmap(":/img.png"), "&IconItem");

    QMenu* pmnuSubMenu = new QMenu("&SubMenu", pmnu);
    pmnu->addMenu(pmnuSubMenu);
    pmnuSubMenu->addAction("&Test");

    QAction* pDisabledAction = pmnu->addAction("&DisabledItem");
    pDisabledAction->setEnabled(false);

    pmnu->addSeparator();

    pmnu->addAction("&Exit", &app, SLOT(quit()));

    mnuBar->addMenu(pmnu);
    mnuBar->show();
    wgt->show();

    return app.exec();
}
