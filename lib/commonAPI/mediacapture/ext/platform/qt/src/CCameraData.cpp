#include "CCameraData.h"

CCameraData::CCameraData()
{
    cameraObject = nullptr;
    cameraType.clear();
    cameraID.clear();
}


QCamera *CCameraData::getCameraObject() const
{
    return cameraObject;
}

const QString CCameraData::getCameraType() const
{
    return cameraType;
}

const QString CCameraData::getCameraID() const
{
    return cameraID;
}

const CCapturer::Result * CCameraData::takeAPicture()
{
    if (cameraObject == nullptr) return nullptr;
    CCapturer capturer(cameraObject);
    return capturer.getResult();
}

const QList<QString> CCameraData::getKeys()
{
    return camerasKeeper.keys();
}


const CCameraData *CCameraData::addNewCamera(QCameraInfo &info){
    CCameraData * data = new CCameraData(info);
    camerasKeeper.insert(data->cameraID, data);
    return data;
}


void CCameraData::cleanAll(){
    foreach (CCameraData * cameraData, camerasKeeper) {
        delete cameraData;
        camerasKeeper.clear();
    }
}

const bool CCameraData::isEmpty()
{
    return camerasKeeper.isEmpty();
}

CCameraData *CCameraData::getCameraData(QString &ID)
{
    if(camerasKeeper.contains(ID)){
        return camerasKeeper.value(ID);
    }else{
        return nullptr;
    }
}


CCameraData::CCameraData(QCameraInfo &info){
    cameraObject = new QCamera(info);
    cameraID = QString::number(camerasKeeper.size() + 1);
    if (info.position() == QCamera::BackFace){cameraType = "back";}
    else{cameraType = "front";}
}
