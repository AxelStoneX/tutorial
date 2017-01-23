#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QtWidgets>
#include "InputDialog.h"

// ===========================================================================
class StartDialog : public QPushButton
{
    Q_OBJECT
public:
    StartDialog(QWidget* pwgt = 0) : QPushButton("Press Me", pwgt)
    {
        connect(this, SIGNAL(clicked()), SLOT(slotButtonClicked()));
    }

public slots:
    void slotButtonClicked()
    {
        InputDialog* pInputDialog = new InputDialog;
        if (pInputDialog->exec() == QDialog::Accepted)
        {
            QMessageBox::information(0,
                                     "Information",
                                     "First Name: "
                                     + pInputDialog->firstName()
                                     + "\nLast Name: "
                                     + pInputDialog->lastName()
                                     );
        }
        delete pInputDialog;
    }
};

#endif // STARTDIALOG_H
