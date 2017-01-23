#include "MediaPlayer.h"
#include <phonon>
#include <QtGui>

MediaPlayer::MediaPlayer(QWidget *pwgt) : QWidget(pwgt)
{
    m_pmedia = new Phonon::MediaObject(this);
    m_pao = new Phonon::AudioOutput(Phonon::VideoCategory, this);

    Phonon::VideoWidget* pvw = new Phonon::VideoWidget;

    Phonon::createPath(m_pmedia, m_pao);
    Phonon::createPath(m_pmedia, pvw);

    QPushButton* pcmdPlay = new QPushButton("&Play");
    QPushButton* pcmdStop = new QPushButton("&Stop");
    QPushButton* pcmdPause = new QPushButton("P&ause");
    QPushButton* pcmdLoad = new QPushButton("&Load");
    Phonon::VolumeSlider* psldVolume = new Phonon::VolumeSlider;
    Phonon::SeekSlider* psldSeek = new Phonon::SeekSlider;

    psldSeek->setMediaObject(m_pmedia);
    psldVolume->setAudioOutput(m_pao);
    psldVolume->setOrientation(Qt::Vertical);
    psldVolume->setMaximumHeight(30);

    connect(pcmdPlay, SIGNAL(clicked()), m_pmedia, SLOT(play()));
    connect(pcmdStop, SIGNAL(clicked()), m_pmedia, SLOT(stop()));
    connect(pcmdPause, SIGNAL(clicked()), m_pmedia, SLOT(pause()));
    connect(pcmdLoad, SIGNAL(clicked()), SLOT(slotLoad()));

    // Layout setup
    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(pcmdPlay);
    phbxLayout->addWidget(pcmdPause);
    phbxLayout->addWidget(pcmdStop);
    phbxLayout->addWidget(psldSeek);
    phbxLayout->addWidget(pcmdLoad);
    phbxLayout->addWidget(psldVolume);

    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(pvw);
    pvbxLayout->addLayout(phbxLayout);

    setLayout(pvbxLayout);
}

void MediaPlayer::slotLoad()
{
    QString str = QFileDialog::getOpenFileName(0, "Load", "", "*.*");
    if(!str.isEmpty())
        m_pmedia->setCurrentSource(Phonon::MediaSource(str));
}
