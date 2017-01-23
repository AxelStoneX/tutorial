#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

class Window : public QLabel
{
private:
    QPoint m_ptPosition;
protected:
    virtual void mousePressEvent(QMouseEvent *pe)
    {
        m_ptPosition = pe->pos();
    }
    virtual void mouseMoveEvent(QMouseEvent *pe)
    {
        move(pe->globalPos() - m_ptPosition);
    }
public:
    Window(QWidget* pwgt = 0)
        : QLabel(pwgt, Qt::FramelessWindowHint | Qt::Window)
    {
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window      win;
    QPixmap      pix(":/check.png");

    win.setPixmap(pix);
    win.setMask(pix.mask());
    win.show();

    return app.exec();
}
