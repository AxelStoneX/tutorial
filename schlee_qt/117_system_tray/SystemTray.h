#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#pragma once

#include <QLabel>

class QSystemTrayIcon;
class QMenu;

// ===========================================================================
class SystemTray : public QLabel
{
    Q_OBJECT
private:
    QSystemTrayIcon* m_ptrayIcon;
    QMenu* m_ptrayIconMenu;
    bool m_bIconSwitcher;
protected:
    virtual void closeEvent(QCloseEvent*);
public:
    SystemTray(QWidget* pwgt = 0);

public slots:
    void slotShowHide();
    void slotShowMessage();
    void slotChangeIcon();
};

#endif // SYSTEMTRAY_H
