/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 *
 * Part of the Qt GameEnabler.
 */

#include "GEAudioOut.h"

#include <QAudioOutput>
#include <QIODevice>
#include <QtMultimedia/qaudio.h>
#include <QtMultimedia/qaudiodeviceinfo.h>
#include <QString>

#include "trace.h" // For debug macros

// Constants
const int GEDefaultChannelCount(2);
const QString GEDefaultAudioCodec("audio/pcm");
const QAudioFormat::Endian GEByteOrder(QAudioFormat::LittleEndian);
const QAudioFormat::SampleType GESampleType(QAudioFormat::SignedInt);
const int GEThreadSleepTime(1); // Milliseconds

using namespace GE;

/*!
  \class Audioout
  \brief An object deploying QAudioOutput for sending the pre-mixed/processed
         audio data into an actual audio device.
*/


/*!
  Constructor.
*/
AudioOut::AudioOut(AudioSource *source, QObject *parent /* = 0 */)
    : QThread(parent),
      m_audioOutput(0),
      m_outTarget(0),
      m_source(source),
      m_sendBuffer(0),
      m_sendBufferSize(0),
      m_samplesMixed(0),
      m_threadState(NotRunning),
      m_usingThread(false)
{
    QAudioFormat format;
    format.setFrequency(AUDIO_FREQUENCY);
    format.setChannels(GEDefaultChannelCount);
    format.setSampleSize(AUDIO_SAMPLE_BITS);
    format.setCodec(GEDefaultAudioCodec);
    format.setByteOrder(GEByteOrder);
    format.setSampleType(GESampleType);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

    if (!info.isFormatSupported(format))
        format = info.nearestFormat(format);

    m_audioOutput = new QAudioOutput(info, format);
    m_audioOutput->setBufferSize(4096 * 8);
    m_sendBufferSize = 4096 * 8;
    m_outTarget = m_audioOutput->start();

    DEBUG_INFO("Buffer size: " << m_audioOutput->bufferSize());
    m_sendBuffer = new AUDIO_SAMPLE_TYPE[m_sendBufferSize];

#ifndef Q_OS_SYMBIAN
    m_usingThread = true;
    start();
#endif

}


/*!
  Destructor.
*/
AudioOut::~AudioOut()
{
    if (m_threadState == DoRun) {
        // Set the thread to exit run().
        m_threadState = DoExit;
    }

    if (QThread::isRunning() == false) {
        m_threadState = NotRunning;
    }

    while (m_threadState != NotRunning) {
        // Wait until the thread is finished.
        msleep(50);
    }

    m_audioOutput->stop();

    delete m_audioOutput;
    delete [] m_sendBuffer;
}


/*!
  For internal notification solution.
*/
void AudioOut::audioNotify()
{
    tick();
}


/*!
  TODO: Document what this method actually does and why it is needed.

  Call this method manually only if you are not using a thread (with Symbian).

  Note: When using Qt GameEnabler, the GameWindow instance owning this AudioOut
  instance will handle calling this method and you should not try to call this
  explicitly.
*/
void AudioOut::tick()
{
    // Fill data to the buffer as much as there is free space available.
    int samplesToWrite(m_audioOutput->bytesFree() /
                       (GEDefaultChannelCount * AUDIO_SAMPLE_BITS / 8));
    samplesToWrite *= 2;

    if (samplesToWrite <= 0)
        return;

    if (samplesToWrite > m_sendBufferSize)
        samplesToWrite = m_sendBufferSize;

    int mixedSamples = m_source->pullAudio(m_sendBuffer, samplesToWrite);
    m_outTarget->write((char*)m_sendBuffer, mixedSamples * 2);
}


/*!
  From QThread.

  Used only in threaded solutions.
*/
void AudioOut::run()
{
    DEBUG_INFO("Starting thread.");
    m_threadState = DoRun;

    if (!m_source) {
        DEBUG_INFO("No audio source, exiting the thread!");
        m_threadState = NotRunning;
        return;
    }

    while (m_threadState == DoRun) {
        tick();
        msleep(GEThreadSleepTime);
    }

    DEBUG_INFO("Exiting thread.");
    m_threadState = NotRunning;
}
