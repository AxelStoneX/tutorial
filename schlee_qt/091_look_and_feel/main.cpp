#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

// ---------------------------------------------------------------------------
QWidget* styledWidget(QStyle* pstyle)
{
    QGroupBox*    pgr  = new QGroupBox(pstyle->metaObject()->className());
    QScrollBar*   psbr = new QScrollBar(Qt::Horizontal);
    QCheckBox*    pchk = new QCheckBox("&Check Box");
    QSlider*      psld = new QSlider(Qt::Horizontal);
    QRadioButton* prad = new QRadioButton("&Radio Button");
    QPushButton*  pcmd = new QPushButton("&Push Button");
    // Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(psbr);
    pvbxLayout->addWidget(pchk);
    pvbxLayout->addWidget(psld);
    pvbxLayout->addWidget(prad);
    pvbxLayout->addWidget(pcmd);
    pgr->setLayout(pvbxLayout);

    QList<QWidget*> pwgtList = ::qFindChildren<QWidget*>(pgr);
    foreach(QWidget* pwgt, pwgtList)
        pwgt->setStyle(pstyle);

    return pgr;
}

// ---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget wgt;

    // Layout setup
    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(styledWidget(new QWindowsStyle));
    phbxLayout->addWidget(styledWidget(new QMotifStyle));
    phbxLayout->addWidget(styledWidget(new QCDEStyle));
    phbxLayout->addWidget(styledWidget(new QCleanlooksStyle));
    phbxLayout->addWidget(styledWidget(new QPlastiqueStyle));
    wgt.setLayout(phbxLayout);

    wgt.show();

    return app.exec();
}
