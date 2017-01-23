#include "mainwindow.h"
#include "SoundPlayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SoundPlayer soundPlayer;

    soundPlayer.show();

    return app.exec();
}
