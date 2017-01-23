#ifndef PLUGINSWINDOW_H
#define PLUGINSWINDOW_H

#include <QMainWindow>
#include "Interfaces.h"

class QLabel;
class QMenu;
// ===========================================================================
class PluginsWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLabel* m_plbl;
    QMenu*  m_pmnuPlugins;
public:
    PluginsWindow(QWidget* pwgt = 0);
    void loadPlugins();
    void addToMenu (QObject* pobj);
protected slots:
    void slotStringOperation();
};

#endif // PLUGINSWINDOW_H
