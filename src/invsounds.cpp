/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

#include "invsounds.h"

#include <QDebug>
#include <QDir>

#include "GEAudioBufferPlayInstance.h"
#include "trace.h"

using namespace GE;


/*!
  \class InvSounds
  \brief Class responsible for playing the game audio.
*/


/*!
  Constructor.
*/
InvSounds::InvSounds(QObject *parent)
    : QObject(parent),
      m_musicInstance(0),
      m_audioOut(0),
      m_sounds(0),
      m_internalSounds(0),
      m_soundCount(0),
      m_internalSoundCount(0)
{
    m_audioOut = new AudioOut(&m_mixer, this);

#ifdef Q_OS_SYMBIAN
    m_audioPullTimer.setInterval(5);
    connect(&m_audioPullTimer, SIGNAL(timeout()), m_audioOut, SLOT(tick()));
#endif

    enableSounds(true);
    enableInternalSounds();
}


/*!
  Destructor.
*/
InvSounds::~InvSounds()
{
    if (m_audioOut) {
        delete m_audioOut;
        m_audioOut = 0;
    }

    disableSounds();
    disableInternalSounds();
}


/*!
  Mutes/unmutes the sounds according to \a enable.
*/
void InvSounds::enableSounds(bool enable)
{
    DEBUG_INFO(enable);

    if (enable) {
#ifdef Q_OS_SYMBIAN
        m_audioPullTimer.start();
#endif
        m_mixer.setAbsoluteVolume(0.2f);
    }
    else {
#ifdef Q_OS_SYMBIAN
        m_audioPullTimer.stop();
#endif
        m_mixer.setAbsoluteVolume(0);
    }
}


/*!
*/
void InvSounds::enableInternalSounds()
{
    DEBUG_POINT;

    disableInternalSounds();

    // Application internal sounds
    m_internalSoundPaths.append(":/sound/menu1.wav");
    m_internalSoundPaths.append(":/sound/menu2.wav");
    m_internalSoundPaths.append(":/sound/gamestart.wav");
    m_internalSoundPaths.append(":/sound/gameover.wav");
    m_internalSoundPaths.append(":/sound/youwin.wav");

    m_internalSoundCount = m_internalSoundPaths.count();
    m_internalSounds = new AudioBuffer*[m_internalSoundCount];

    // Load general application sounds
    for (int ff = 0; ff < m_internalSoundPaths.count(); ff++) {
        m_internalSounds[ff] = AudioBuffer::loadWav(m_internalSoundPaths[ff]);
    }
}


/*!
  Reloads sounds from files in \a sounds.
*/
void InvSounds::enableSounds(QStringList sounds)
{
    DEBUG_INFO(sounds);
    disableSounds();

    // Sounds from the level
    m_soundPaths = sounds;
    m_soundCount = sounds.count();
    m_sounds = new AudioBuffer*[m_soundCount];

    // Load level sounds
    for (int f = 0; f < m_soundPaths.count(); f++) {
        m_sounds[f] = AudioBuffer::loadWav(m_soundPaths[f]);
    }
}


/*!
*/
void InvSounds::disableInternalSounds()
{
    DEBUG_POINT;

    for (int f = 0; f < m_internalSoundCount; f++) {
        if (m_internalSounds[f]) {
            delete m_internalSounds[f];
        }
    }

    delete [] m_internalSounds;
    m_internalSoundCount = 0;
    m_internalSoundPaths.clear();
    m_musicInstance = 0;
}


/*!
  Deletes the current set of sounds.
*/
void InvSounds::disableSounds()
{
    DEBUG_POINT;

    for (int f = 0; f < m_soundCount; f++) {
        if (m_sounds[f]) {
            delete m_sounds[f];
        }
    }

    delete [] m_sounds;
    m_sounds = 0;
    m_soundCount = 0;
    m_soundPaths.clear();
    m_musicInstance = 0;
}


/*!
  Starts playing the theme music.
*/
void InvSounds::beginMusicOn()
{
    DEBUG_POINT;

    m_musicInstance = new GE::AudioBufferPlayInstance(m_sounds[0]);

    if (m_mixer.addAudioSource(m_musicInstance)) {
        // Loop the music forever.
        m_musicInstance->setLoopCount(-1);
    }
    else {
        DEBUG_INFO("Failed to play the music!");
    }
}


/*!
  Stops the theme music.
*/
void InvSounds::beginMusicOff()
{
    DEBUG_POINT;

    if (!m_musicInstance) {
        DEBUG_INFO("m_musicInstance == 0");
        return;
    }

    m_musicInstance->setLoopCount(0); // stop when finished
    m_musicInstance = 0;
}


/*!
  Plays sound with \a index.
*/
void InvSounds::playSound(int index)
{
    DEBUG_INFO(index);

    if (index < m_soundCount && index > -1) {
        m_sounds[index]->playWithMixer(m_mixer);
    }
    else {
        DEBUG_INFO("Error, invalid index!");
    }
}


/*!
  Plays sound wiht \a index \a count times.
*/
void InvSounds::playSounds(int index, int count)
{
    DEBUG_INFO(index << "," << count);

    if (index < m_soundCount && index > -1) {
        AudioBufferPlayInstance* instance =
                m_sounds[index]->playWithMixer(m_mixer);
        instance->setLoopCount(count);
    }
    else {
        DEBUG_INFO("Error, invalid index!");
    }
}


/*!
  Plays internal sound with \a index.
*/
void InvSounds::playInternalSound(int index)
{
    DEBUG_INFO(index);

    if (index < m_internalSoundCount && index > -1) {
        m_internalSounds[index]->playWithMixer(m_mixer);
    }
    else {
        DEBUG_INFO("Error, invalid index!");
    }
}


/*!
  Plays internal sound with \a index \a count times.
*/
void InvSounds::playInternalSounds(int index, int count)
{
    DEBUG_INFO(index << "," << count);

    if (index < m_internalSoundCount && index > -1) {
        AudioBufferPlayInstance* instance =
                m_internalSounds[index]->playWithMixer(m_mixer);
        instance->setLoopCount(count);
    }
    else {
        DEBUG_INFO("Error, invalid index!");
    }
}


/*!
*/
void InvSounds::gameStartSound()
{
    DEBUG_POINT;
    AudioBufferPlayInstance* instance =
            m_internalSounds[2]->playWithMixer(m_mixer);
    instance->setLoopCount(2);
}
