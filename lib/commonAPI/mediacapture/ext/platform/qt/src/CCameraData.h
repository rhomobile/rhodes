#ifndef CCAMERADATA_H
#define CCAMERADATA_H
#include <QObject>
#include <QCameraInfo>
#include <QCamera>
#include <QMultimedia>
#include <QMediaRecorder>
#include <QCameraImageCapture>
#include <QEventLoopLocker>
#include "CCapturer.h"
#include "CCameraDialogWindow.h"

class CCameraData : public QObject{
    Q_OBJECT
private:
    QCamera * cameraObject;
    QString cameraType;
    QString cameraID;

    CCameraData();
    CCameraData(QCameraInfo &info);

    static QHash<QString, CCameraData *> camerasKeeper;
public:

    ~CCameraData(){delete cameraObject;}
    static const CCameraData *addNewCamera(QCameraInfo &info);
    static void cleanAll();
    static const QList<QString> getKeys();
    static const bool isEmpty();
    static CCameraData *getCameraData(QString &ID);

    QCamera *getCameraObject() const;
    const QString getCameraType() const;
    const QString getCameraID() const;
    const CCapturer::Result *takeAPicture();

};

#endif // CCAMERADATA_H
