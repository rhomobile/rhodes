#include "CCameraData.h"

QHash<QString, CCameraData *> CCameraData::camerasKeeper;


QCamera *CCameraData::getCameraObject()
{
    return cameraObject;
}

QMutex *CCameraData::getMutex()
{
    static QMutex mutex;
    return & mutex;
}
CCameraData::CCameraData()
{
    cameraObject = nullptr;
    cameraType.clear();
    cameraID.clear();
    capturer = nullptr;
}

const QString CCameraData::getCameraType() const
{
    return cameraType;
}

const QString CCameraData::getCameraID() const
{
    return cameraID;
}

void CCameraData::takeAPicture(rho::apiGenerator::CMethodResult &oResult)
{
    qDebug() << "Taking a Picture";
    emit capture(oResult);
}

const QList<QString> CCameraData::getKeys()
{
    return camerasKeeper.keys();
}


const CCameraData *CCameraData::addNewCamera(QCameraInfo &info){
    CCameraData * data = new CCameraData(info);
    camerasKeeper.insert(data->getCameraID(), data);
    return data;
}


void CCameraData::cleanAll(){
QMutexLocker locker(getMutex());
    foreach (CCameraData * cameraData, camerasKeeper) {
        cameraData->quit();
        camerasKeeper.clear();
    }
}

const bool CCameraData::isEmpty()
{
    return camerasKeeper.isEmpty();
}

CCameraData *CCameraData::getCameraData(QString &ID)
{
    QMutexLocker locker(getMutex());
    if(camerasKeeper.contains(ID)){
        return camerasKeeper.value(ID);
    }else{
        return nullptr;
    }
}


CCameraData::CCameraData(QCameraInfo &info){
    this->info = info;
    cameraID = QString::number(camerasKeeper.size() + 1);
    if (info.position() == QCamera::BackFace){cameraType = "back";}
    else{cameraType = "front";}
    start(TimeCriticalPriority);
}

void CCameraData::run()
{

    cameraObject = new QCamera(info);
    connect(this, SIGNAL(destroyed(QObject*)), cameraObject, SLOT(deleteLater()));

    capturer = new CCapturer(cameraObject);
    connect(this, SIGNAL(finished()), capturer, SLOT(deleteLater()));
    connect(this, SIGNAL(capture(rho::apiGenerator::CMethodResult)), capturer, SLOT(capture(rho::apiGenerator::CMethodResult)));

    exec();
}
