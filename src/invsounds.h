/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef INVSOUNDS_H
#define INVSOUNDS_H

#include <QObject>
#include <QStringList>

#ifdef Q_OS_SYMBIAN
    #include <QTimer>
#endif

#include "GEAudioBuffer.h"
#include "GEAudioMixer.h"
#include "GEAudioOut.h"


class InvSounds : public QObject
{
    Q_OBJECT

public:
    explicit InvSounds(QObject *parent = 0);
    ~InvSounds();

public:
    void enableInternalSounds();
    void enableSounds(QStringList sounds);
    void disableInternalSounds();
    void disableSounds();
    void beginMusicOn();
    void beginMusicOff();
    void enableSounds(bool enable);
    void gameStartSound();

public slots:
    void playSound(int index);
    void playSounds(int index, int count);
    void playInternalSound(int index);
    void playInternalSounds(int index, int count);

protected:
    GE::AudioBufferPlayInstance *m_musicInstance;
    GE::AudioMixer m_mixer;
    GE::AudioOut *m_audioOut;
    GE::AudioBuffer **m_sounds;
    GE::AudioBuffer **m_internalSounds;
    int m_soundCount;
    int m_internalSoundCount;
    QStringList m_soundPaths;
    QStringList m_internalSoundPaths;
#ifdef Q_OS_SYMBIAN
    QTimer m_audioPullTimer; // Used to tick the audio engine
#endif
};

#endif
