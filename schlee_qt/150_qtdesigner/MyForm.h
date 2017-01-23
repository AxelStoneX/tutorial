#ifndef MYFORM_H
#define MYFORM_H

#include "ui_MyForm.h"

class MyForm : public QWidget
{
    Q_OBJECT
private:
    Ui::MyForm m_ui;
public:
    MyForm(QWidget* pwgt = 0) : QWidget(pwgt)
    {
        m_ui.setupUi(this);
        connect(m_ui.m_pcmdReset, SIGNAL(clicked()), SLOT(slotReset()));
    }
public slots:
    void slotReset()
    {
        m_ui.m_psld->setValue(0);
        m_ui.m_plcd->display(0);
    }
};

#endif // MYFORM_H
