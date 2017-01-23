#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QWidget>
class QSound;

// ===========================================================================
class SoundPlayer : public QWidget
{
    Q_OBJECT
private:
    QSound* m_psnd;
public:
    SoundPlayer(QWidget* pwgt = 0);
public slots:
    void slotPlay();
    void slotStop();
    void slotLoopToggled(bool);
};

#endif // SOUNDPLAYER_H
