#include "notificationsound.h"

#ifndef RHODES_VERSION_LIBRARY
#include <QAudioOutput>
#include <QPointer>
#include <QBuffer>
#include <QtMath>
#include <QEventLoop>
#include <QTimer>
#endif

#if QT_VERSION >= 0x060000
#include <QAudioSink>
#include <QMediaDevices>
#else
#include <QAudioOutput>
#include <QSound>
#endif

void NotificationSound::playSound()
{
#ifndef RHODES_VERSION_LIBRARY
    qreal frequency = (sFrequency <= 0 ? 2000 : sFrequency);
    qreal mseconds  = (sDuration  <= 0 ? 1000 : sDuration);
    quint8 volume   = (sVolume    <= 0 ? 1    : sVolume);

    const qreal sampleRateHz = 192000.0;
    const qreal w = 2.0 * M_PI / (sampleRateHz / frequency);

    QByteArray bytebuf;
    bytebuf.resize(static_cast<int>(mseconds * (sampleRateHz / 1000.0)));

    for (int i = 0; i < bytebuf.size(); ++i)
        bytebuf[i] = static_cast<char>(255.0 * qSin(qreal(i) * w));

    QBuffer *buffer = new QBuffer(this);
    buffer->setData(bytebuf);
    buffer->open(QIODevice::ReadOnly);

    QAudioFormat format;
    format.setSampleRate(static_cast<int>(sampleRateHz));
    format.setChannelCount(1);

#if QT_VERSION >= 0x060000
    format.setSampleFormat(QAudioFormat::UInt8);

    QAudioDevice device = QMediaDevices::defaultAudioOutput();
    QAudioSink *audio = new QAudioSink(device, format, this);
    audio->setVolume(qreal(volume + 1) / 4.0);    // 0.0–1.0
    audio->start(buffer);

#else
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioOutput *audio = new QAudioOutput(format, this);
    audio->setVolume(qreal(volume + 1) / 4.0);    // 0.0–1.0
    audio->setBufferSize(bytebuf.size());
    audio->start(buffer);
#endif

    QEventLoop loop;
    QTimer::singleShot(static_cast<int>(mseconds * 2), &loop, SLOT(quit()));
    loop.exec();

#if QT_VERSION >= 0x060000
    audio->stop();
#endif
    buffer->close();
    buffer->deleteLater();
    audio->deleteLater();
#endif
}

void NotificationSound::beep()
{
#ifndef RHODES_VERSION_LIBRARY
    rho::Hashtable<rho::String, rho::String> propertyMap;
    beep(propertyMap);
#endif
}

NotificationSound::NotificationSound(int frequency, int duration, int volume)
#ifndef RHODES_VERSION_LIBRARY
    : IExecutable(getMainWindow())
#endif
{
#ifndef RHODES_VERSION_LIBRARY
    sFrequency = frequency;
    sDuration = duration;
    sVolume = volume;
#endif
}


void NotificationSound::beep(const rho::Hashtable<rho::String, rho::String> &propertyMap)
{
#ifndef RHODES_VERSION_LIBRARY
    int mseconds = 1000;
    if (propertyMap.containsKey("duration")){
        QString propDur(propertyMap.get("duration").c_str());
        quint32 value = propDur.toUInt();
        mseconds = value ? value : mseconds;
    }

    int frequency = 2000;
    if (propertyMap.containsKey("frequency")){
        QString propFreq(propertyMap.get("frequency").c_str());
        quint32 value = propFreq.toUInt();
        frequency = value ? value : frequency;
    }

    int volume = 1;
    if (propertyMap.containsKey("volume")){
        QString propVol(propertyMap.get("volume").c_str());
        quint32 value = propVol.toUInt();
        volume = value ? value : volume;
    }

    NotificationSound * sound = new NotificationSound(frequency,mseconds,volume);
    sound->run();
#endif
}

extern "C" void notification_beep(const rho::Hashtable<rho::String, rho::String>& propertyMap){
#ifndef RHODES_VERSION_LIBRARY
    NotificationSound::beep(propertyMap);
#endif
}
