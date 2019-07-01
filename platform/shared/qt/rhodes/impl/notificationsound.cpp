#include "notificationsound.h"

#ifndef RHODES_VERSION_LIBRARY
#include <QAudioOutput>
#include <QPointer>
#include <QBuffer>
#include <QtMath>
#include <QSound>
#include <QEventLoop>
#include <QTimer>
#endif

void NotificationSound::playSound()
{
#ifndef RHODES_VERSION_LIBRARY
    //QSound::play(":/sound/notification_sound.wav");
    //return;
    qreal frequency = ((sFrequency <= 0)?2000:sFrequency);
    qreal mseconds = ((sDuration <= 0)?1000:sDuration);
    quint8 volume = ((sVolume <= 0)?1:sVolume);
    qreal sampleRate = 2.0 * M_PI / (192000./frequency);

    QByteArray bytebuf;
    bytebuf.resize(mseconds * 192);

    for (int i=0; i < bytebuf.size(); i++) {
        bytebuf[i] = (quint8)(qreal(255) * qSin(qreal(i) * sampleRate));
    }


    QDataStream stream(&bytebuf, QIODevice::ReadWrite);

    QAudioFormat format;
    format.setSampleRate(192000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioOutput * audio = new QAudioOutput(format, this);
    audio->setVolume(1.0 * (qreal(volume + 1)/4));
    audio->setBufferSize(bytebuf.size());
    audio->start(stream.device());

    QEventLoop loop;
    QTimer::singleShot(mseconds*2, &loop, SLOT(quit()));
    loop.exec();
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
