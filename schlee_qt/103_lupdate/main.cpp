#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator  ptranslator;
    ptranslator.load("main_ru.qm",".");
    app.installTranslator(&ptranslator);


    QLabel lbl(QObject::tr("Hello"));
    lbl.show();
    return app.exec();
}
