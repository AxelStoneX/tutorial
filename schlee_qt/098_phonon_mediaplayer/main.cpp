#include "mainwindow.h"
#include "MediaPlayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MediaPlayer mediaPlayer;

    mediaPlayer.show();

    return app.exec();
}
