#ifndef GRABWIDGET_H
#define GRABWIDGET_H

#include <QtWidgets>

//============================================================================
class GrabWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel* m_plbl;
public:
    GrabWidget(QWidget* pwgt = 0);
public slots:
    void slotGrabScreen();
};

#endif // GRABWIDGET_H
