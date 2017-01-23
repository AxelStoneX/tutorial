#include <QtGui>
#include <QApplication>
#include <QLabel>
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QLabel lbl("Hello world!");
    lbl.show();
    return app.exec();
}
