#include "decoderthread.h"

DecoderThread::DecoderThread(QObject *parent) : QThread(parent)
{
    decoder = new QZXing(this);
    //connect(decoder, SIGNAL(tagFound(QString)), this, SIGNAL(toLog(QString)));
}

DecoderThread::~DecoderThread()
{

}

void DecoderThread::run()
{
    qDebug() << QTime::currentTime().toString() + " : Scanning";
    QString result = decoder->decodeImage(image, image.width(), image.height());
    if (!result.isEmpty()){emit encoded(result);}
    else{emit scanningProcessMsg();}

}

void DecoderThread::imageCaptured(int id, const QImage &preview)
{
    image = preview;
    start(TimeCriticalPriority);
}
