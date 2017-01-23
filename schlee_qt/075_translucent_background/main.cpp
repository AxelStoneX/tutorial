#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel       lbl;

    lbl.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    lbl.setAttribute(Qt::WA_TranslucentBackground);
    lbl.setPixmap(QPixmap(":/check.png"));

    QPushButton* pcmdQuit = new QPushButton("X");
    pcmdQuit->setFixedSize(16, 16);
    QObject::connect(pcmdQuit, SIGNAL(clicked()), &app, SLOT(quit()));

    // Setup layout
    QVBoxLayout* pvbx = new QVBoxLayout;
    pvbx->addWidget(pcmdQuit);
    pvbx->addStretch(1);
    lbl.setLayout(pvbx);

    lbl.show();

    return app.exec();
}
