#ifndef SHELL_H
#define SHELL_H

#include <QtWidgets>

class Shell : public QWidget
{
    Q_OBJECT
private:
    QProcess* m_process;
    QLineEdit* m_ptxtCommand;
    QTextEdit* m_ptxtDisplay;

public:
    // -----------------------------------------------------------------------
    Shell(QWidget* pwgt = 0) : QWidget(pwgt)
    {
        m_process = new QProcess(this);
        m_ptxtDisplay = new QTextEdit;

        QLabel* plbl = new QLabel("&Command:");

        m_ptxtCommand = new QLineEdit("cd");
        plbl->setBuddy(m_ptxtCommand);

        QPushButton* pcmd = new QPushButton("&Enter");

        connect(m_process,
                SIGNAL(readyReadStandardOutput()),
                SLOT(slotDataOnStdout()));
        connect(m_ptxtCommand,
                SIGNAL(returnPressed()),
                SLOT(slotReturnPrssed));
        connect(pcmd, SIGNAL(clicked()), SLOT(slotReturnPressed()));

        // Layout setup
        QHBoxLayout* phbxLayout = new QHBoxLayout;
        phbxLayout->addWidget(plbl);
        phbxLayout->addWidget(m_ptxtCommand);
        phbxLayout->addWidget(pcmd);

        QVBoxLayout* pvbxLayout = new QVBoxLayout;
        pvbxLayout->addWidget(m_ptxtDisplay);
        pvbxLayout->addLayout(phbxLayout);
        setLayout(pvbxLayout);
    }

public slots:
    // -----------------------------------------------------------------------
    void slotDataOnStdout()
    {
        m_ptxtDisplay->append(m_process->readAllStandardOutput());
    }

    // -----------------------------------------------------------------------
    void slotReturnPressed()
    {
        QString strCommand = "";

#ifdef Q_WS_LINUX
        strCommand = "cd /";
#endif
        strCommand += m_ptxtCommand->text();
        m_process->start(strCommand);
    }

};

#endif // SHELL_H
