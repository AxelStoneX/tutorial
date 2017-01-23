#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget       wgt;
    QDial*        pdia = new QDial;
    QProgressBar* pprb = new QProgressBar;

    pdia->setRange(0, 100);
    pdia->setNotchTarget(5);
    pdia->setNotchesVisible(true);
    QObject::connect(pdia, SIGNAL(valueChanged(int)),
                     pprb, SLOT(setValue(int)));

    // Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(pdia);
    pvbxLayout->addWidget(pprb);
    wgt.setLayout(pvbxLayout);

    wgt.resize(180, 200);
    wgt.show();

    return app.exec();
}
