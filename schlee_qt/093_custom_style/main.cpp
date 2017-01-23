#include "mainwindow.h"
#include "CustomStyle.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget      wgt;
    QPushButton* pcmdA = new QPushButton("Button1");
    QPushButton* pcmdB = new QPushButton("Button2");
    QPushButton* pcmdC = new QPushButton("Button3");

    // Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(pcmdA);
    pvbxLayout->addWidget(pcmdB);
    pvbxLayout->addWidget(pcmdC);
    wgt.setLayout(pvbxLayout);

    app.setStyle(new CustomStyle);
    wgt.show();

    return app.exec();
}
