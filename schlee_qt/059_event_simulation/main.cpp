#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLineEdit txt("User input: ");
    txt.show();
    txt.resize(280, 20);

    int i;
    for( i = 0; i < (Qt::Key_Z - Qt::Key_A + 1); ++i )
    {
        QChar ch = 65 + i;
        int nKey = Qt::Key_A + i;
        QKeyEvent* pePress = new QKeyEvent(QEvent::KeyPress, nKey, Qt::NoModifier, ch);
        QApplication::sendEvent(&txt, pePress);

        QKeyEvent* peRelease = new QKeyEvent(QEvent::KeyRelease, nKey, Qt::NoModifier, ch);
        QApplication::sendEvent(&txt, peRelease);
    }

    return app.exec();
}
