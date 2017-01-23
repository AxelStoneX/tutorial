#include <QtGui>
#include "Progress.h"

//----------------------------------------------------------------------------

Progress::Progress(QWidget* pwgt)
    :QWidget(pwgt)
    ,m_nStep(0)
{
    m_pprb = new QProgressBar;
    m_pprb->setRange(0, 5);
    m_pprb->setMinimumWidth(200);
    m_pprb->setAlignment(Qt::AlignCenter);

    QPushButton* pcmdStep = new QPushButton("&Step");
    QPushButton* pcmdReset = new QPushButton("&Reset");

    QObject::connect(pcmdStep, SIGNAL(clicked()), SLOT(slotStep()));
    QObject::connect(pcmdReset, SIGNAL(clicked()), SLOT(slotReset()));

    //Layout setup
    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(m_pprb);
    phbxLayout->addWidget(pcmdStep);
    phbxLayout->addWidget(pcmdReset);
    setLayout(phbxLayout);
}

//----------------------------------------------------------------------------

void Progress::slotStep()
{
    m_pprb->setValue(++m_nStep);
}

//----------------------------------------------------------------------------

void Progress::slotReset()
{
    m_nStep = 0;
    m_pprb->reset();
}
