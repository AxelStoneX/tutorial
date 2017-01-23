#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QtWidgets>

// ===========================================================================
class HelpBrowser : public QWidget
{
    Q_OBJECT
public:
    HelpBrowser(const QString& strPath,
                const QString& strFileName,
                      QWidget* pwgt        = 0
               ) : QWidget(pwgt)
    {
        QPushButton* pcmdBack = new QPushButton("<<");
        QPushButton* pcmdHome = new QPushButton("Home");
        QPushButton* pcmdForward = new QPushButton(">>");
        QTextBrowser* ptxtBrowser = new QTextBrowser;

        connect(pcmdBack, SIGNAL(clicked()),
                ptxtBrowser, SLOT(backward()));
        connect(pcmdHome, SIGNAL(clicked()),
                ptxtBrowser, SLOT(home()));
        connect(pcmdForward, SIGNAL(clicked()),
                ptxtBrowser, SLOT(forward()));
        connect(ptxtBrowser, SIGNAL(backwardAvailable(bool)),
                pcmdBack, SLOT(setEnabled(bool)));
        connect(ptxtBrowser, SIGNAL(forwardAvailable(bool)),
                pcmdForward, SLOT(setEnabled(bool)));

        ptxtBrowser->setSearchPaths(QStringList() << strPath);
        ptxtBrowser->setSource(QString(strFileName));

        // Layout setup
        QVBoxLayout* pvbxLayout = new QVBoxLayout;
        QHBoxLayout* phbxLayout = new QHBoxLayout;
        phbxLayout->addWidget(pcmdBack);
        phbxLayout->addWidget(pcmdHome);
        phbxLayout->addWidget(pcmdForward);
        pvbxLayout->addLayout(phbxLayout);
        pvbxLayout->addWidget(ptxtBrowser);
        setLayout(pvbxLayout);
    }
};

#endif // HELPBROWSER_H
