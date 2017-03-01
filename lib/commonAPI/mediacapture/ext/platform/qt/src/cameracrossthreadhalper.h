#ifndef CAMERACROSSTHREADHALPER_H
#define CAMERACROSSTHREADHALPER_H

#include <QObject>

class CameraCrossThreadHalper : public QObject
{
    Q_OBJECT
public:
    explicit CameraCrossThreadHalper(QObject *parent = 0);

signals:

public slots:
};

#endif // CAMERACROSSTHREADHALPER_H
