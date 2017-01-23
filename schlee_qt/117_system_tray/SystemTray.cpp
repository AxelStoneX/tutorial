#include "SystemTray.h"
#include <QtWidgets>

SystemTray::SystemTray(QWidget *pwgt)
    : QLabel("<H1>Application Window</H1>", pwgt)
    , m_bIconSwitcher(false)
{
    setWindowTitle("System Tray");

    QAction* pactShowHide = new QAction("&Show/Hide Application Window", this);

    connect(pactShowHide, SIGNAL(triggered()),
            this, SLOT(slotShowHide()));

    QAction* pactShowMessage = new QAction("S&how Message", this);
    connect(pactShowMessage, SIGNAL(triggered()),
            this, SLOT(slotShowMessage()));

    QAction* pactChangeIcon = new QAction("&ChangeIcon", this);
    connect(pactChangeIcon, SIGNAL(triggered()),
            this, SLOT(slotChangeIcon()));

    QAction* pactQuit = new QAction("&Quit", this);
    connect(pactQuit, SIGNAL(triggered()),
            qApp, SLOT(quit()));

    m_ptrayIconMenu = new QMenu(this);
    m_ptrayIconMenu->addAction(pactShowHide);
    m_ptrayIconMenu->addAction(pactShowMessage);
    m_ptrayIconMenu->addAction(pactChangeIcon);
    m_ptrayIconMenu->addAction(pactQuit);

    m_ptrayIcon = new QSystemTrayIcon(this);
    m_ptrayIcon->setContextMenu(m_ptrayIconMenu);
    m_ptrayIcon->setToolTip("System Tray");

    slotChangeIcon();

    m_ptrayIcon->show();
}

void SystemTray::closeEvent(QCloseEvent*)
{
    if (m_ptrayIcon->isVisible())
        hide();
}

void SystemTray::slotShowHide()
{
    setVisible(!isVisible());
}

void SystemTray::slotShowMessage()
{
    m_ptrayIcon->showMessage("For your information",
                             "You have selected the "
                             "\"Show Message!\" option",
                             QSystemTrayIcon::Information,
                             3000);
}

void SystemTray::slotChangeIcon()
{
    m_bIconSwitcher = !m_bIconSwitcher;
    QString strPixmapName = m_bIconSwitcher ? ":/icon1.png" : ":/icon2.png";
    m_ptrayIcon->setIcon(QPixmap(strPixmapName));
}
