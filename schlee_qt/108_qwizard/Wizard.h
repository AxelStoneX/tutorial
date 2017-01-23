#ifndef WIZARD_H
#define WIZARD_H

#include <QtWidgets>

class Wizard : public QWizard
{
private:
    QWizardPage* createPage(QWidget* pwgt, QString strTitle)
    {
        QWizardPage* ppage = new QWizardPage;
        ppage->setTitle(strTitle);

        QVBoxLayout* playout = new QVBoxLayout;
        playout->addWidget(pwgt);
        ppage->setLayout(playout);

        return ppage;
    }

public:
    Wizard(QWidget* pwgt = 0) : QWizard(pwgt)
    {
        addPage(createPage(new QLabel("<H1>Label 1</H1>"), "One"));
        addPage(createPage(new QLabel("<H1>Label 2</H1>"), "Two"));
        addPage(createPage(new QLabel("<H1>Label 3</H1>"), "Three"));
    }
};

#endif // WIZARD_H
