#ifndef DECODERTHREAD_H
#define DECODERTHREAD_H

#include <QThread>
#include <QImage>
#include <QSet>
#include "qzxing/QZXing.h"

class DecoderThread : public QThread
{
    Q_OBJECT
public:
    struct DecodeResult{
        QString format;
        QString result;
        quint32 id = 0;
    };

    explicit DecoderThread(QObject *parent = 0);
    ~DecoderThread();

    DecodeResult getLastSavedValue() const;

private:
    QZXing * decoder;
    QImage image;
    QSet<QString> codeKeeper;
    DecodeResult lastSavedValue;
signals:
    void encoded(QString, QString);
    void scanningProcessMsg();
public slots:
    void run();
    void imageCaptured(int id, const QImage &preview);
    void imageCaptured(const QImage &preview);

};

#endif // DECODERTHREAD_H
