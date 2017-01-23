#include <QApplication>
#include <QWidget>
#include <QPixmap>
#include <QCursor>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget      wgt;
    QPixmap      pix(":/new/clock/clock.png");
    QCursor      cur(pix);

    wgt.setCursor(cur);
    wgt.resize(180,100);
    wgt.show();

    return app.exec();
}
