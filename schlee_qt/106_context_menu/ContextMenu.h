#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QtWidgets>

// ===========================================================================
class ContextMenu : public QTextEdit
{
    Q_OBJECT
private:
    QMenu* m_pmnu;
protected:
    virtual void contextMenuEvent(QContextMenuEvent* pe)
    {
        m_pmnu->exec(pe->globalPos());
    }

public:
    ContextMenu(QWidget* pwgt = 0)
        : QTextEdit(pwgt)
    {
        setReadOnly(true);
        m_pmnu = new QMenu(this);
        m_pmnu->addAction("&Red");
        m_pmnu->addAction("&Green");
        m_pmnu->addAction("&Blue");
        connect(m_pmnu,
                SIGNAL(triggered(QAction*)),
                SLOT(slotActivated(QAction*))
                );
    }

public slots:
    void slotActivated(QAction* pAction)
    {
        QString strColor = pAction->text().remove("&");
        setHtml(QString("<BODY BGCOLOR=%1></BODY>").arg(strColor));
    }

};

#endif // CONTEXTMENU_H
