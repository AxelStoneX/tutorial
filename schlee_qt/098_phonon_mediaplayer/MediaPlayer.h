#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>

namespace Phonon
{
class AudioOutput;
class MediaObject;
}

// ===========================================================================
class MediaPlayer : public QWidget
{
    Q_OBJECT
private:
    Phonon::AudioOutput* m_pao;
    Phonon::MediaObject* m_pmedia;
public:
    MediaPlayer(QWidget* pwgt = 0);
public slots:
    void slotLoad();
};

#endif // MEDIAPLAYER_H
