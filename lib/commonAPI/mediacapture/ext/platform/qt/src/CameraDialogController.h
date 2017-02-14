#ifndef CAMERADIALOGCONTROLLER_H
#define CAMERADIALOGCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

class CameraDialogController : public QObject
{
    Q_OBJECT
protected:
    volatile bool exists;
    QMutex mutex;

public:
    explicit CameraDialogController(QObject *parent):QObject(parent){
        exists = false;
    }
    virtual ~CameraDialogController(){

    }

    volatile bool dialogExists(){return exists;}
public slots:
    void setDialogExists(){exists = true;}
    void setDialogRejected(){exists = false;}

};

#endif // CAMERADIALOGCONTROLLER_H
