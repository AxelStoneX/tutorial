#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QSplitter>
#include <QTextEdit>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSplitter sp1(Qt::Vertical);
    QTextEdit* ptxt1 = new QTextEdit;
    QTextEdit* ptxt2 = new QTextEdit;

    sp1.addWidget(ptxt1);
    sp1.addWidget(ptxt2);

    ptxt1->setPlainText("Line1\n"
                        "Line2\n"
                        "Line3\n"
                        "Line4\n"
                        "Line5\n"
                        "Line6\n"
                        "Line7\n");
    ptxt2->setPlainText(ptxt1->toPlainText());

    sp1.resize(200,220);
    sp1.show();

    return app.exec();
}
