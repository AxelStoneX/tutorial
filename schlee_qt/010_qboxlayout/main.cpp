#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QBoxLayout>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget      wgt;

    QPushButton* pcmdA = new QPushButton("A");
    QPushButton* pcmdB = new QPushButton("B");
    QPushButton* pcmdC = new QPushButton("C");

    //Layout setup
    QBoxLayout* pbxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    pbxLayout->addWidget(pcmdA,1);
    pbxLayout->addWidget(pcmdB,1);
    pbxLayout->addWidget(pcmdC,1);
    wgt.setLayout(pbxLayout);

    wgt.resize(450, 40);
    wgt.show();

    return app.exec();

}
