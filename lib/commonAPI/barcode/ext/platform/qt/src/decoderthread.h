#ifndef DECODERTHREAD_H
#define DECODERTHREAD_H

#include <QThread>
#include <QImage>
#include "qzxing/QZXing.h"

class DecoderThread : public QThread
{
    Q_OBJECT
public:
    explicit DecoderThread(QObject *parent = 0);
    ~DecoderThread();
private:
    QZXing * decoder;
    QImage image;
signals:
    void encoded(QString);
    void scanningProcessMsg();
public slots:
    void run();
    void imageCaptured(int id, const QImage &preview);
};

#endif // DECODERTHREAD_H
