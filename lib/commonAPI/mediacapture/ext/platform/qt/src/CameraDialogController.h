#ifndef CAMERADIALOGCONTROLLER_H
#define CAMERADIALOGCONTROLLER_H

#include <QObject>

class CameraDialogController : public QObject
{
    Q_OBJECT
public:
    explicit CameraDialogController(QObject *parent = 0):QObject(parent){}

signals:
    void show();
    void hide();

};

#endif // CAMERADIALOGCONTROLLER_H
