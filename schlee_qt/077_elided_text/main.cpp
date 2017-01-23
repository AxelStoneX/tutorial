#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

// ===========================================================================
class ElidedText : public QWidget
{
protected:
    virtual void paintEvent(QPaintEvent*)
    {
        QString str = "this is a long text, please, resize the window";
        QString strElided = fontMetrics().elidedText(str, Qt::ElideMiddle, width());
        QPainter painter(this);
        painter.drawText(rect(), strElided);
    }
public:
    ElidedText(QWidget* pwgt = 0) : QWidget(pwgt)
    {
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ElidedText et;
    et.resize(200, 30);
    et.show();

    return app.exec();
}
0
