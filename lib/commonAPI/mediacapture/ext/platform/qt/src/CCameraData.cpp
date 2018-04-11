#include "CCameraData.h"
#include "generated/cpp/ICamera.h"

QHash<QString, CCameraData *> CCameraData::camerasKeeper;

QMutex *CCameraData::getMutex()
{
    static QMutex mutex;
    return & mutex;
}
CCameraData::CCameraData():CameraDialogController(0)
{
    cameraType.clear();
    cameraID.clear();
}

const QString CCameraData::getCameraType() const
{
    return cameraType;
}

const QString CCameraData::getCameraID() const
{
    return cameraID;
}

const QList<QString> CCameraData::getKeys()
{
    return camerasKeeper.keys();
}

const QList<CCameraData *> CCameraData::getValues()
{
    return camerasKeeper.values();
}


const CCameraData *CCameraData::addNewCamera(QCameraInfo &info){
    CCameraData * data = new CCameraData(info);
    camerasKeeper.insert(data->getCameraID(), data);
    return data;
}


void CCameraData::cleanAll(){
    QMutexLocker locker(getMutex());
    foreach (CCameraData * cameraData, camerasKeeper) {
        cameraData->deleteLater();
        camerasKeeper.clear();
    }
}

const bool CCameraData::isEmpty()
{
    return camerasKeeper.isEmpty();
}

CCameraData *CCameraData::getCameraData(QString ID)
{
    QMutexLocker locker(getMutex());
    if(camerasKeeper.contains(ID)){return camerasKeeper.value(ID);
    }else{return nullptr;}

}


CCameraData::CCameraData(QCameraInfo &info):CameraDialogController(0){
    this->info = info;
#ifndef OS_SAILFISH
    cameraID = QString::number(camerasKeeper.size() + 1);

    if (info.position() == QCamera::BackFace){
        cameraType = QString::fromStdString(rho::ICamera::CAMERA_TYPE_BACK);
    }
    else{
        cameraType = QString::fromStdString(rho::ICamera::CAMERA_TYPE_FRONT);
    } 
    connect(getQMainWindow(), SIGNAL(captured(QString)), this, SLOT(captured(QString)));
#else   
    cameraID = QString::fromStdString(rho::ICamera::CAMERA_TYPE_BACK);
    cameraType = QString::fromStdString(rho::ICamera::CAMERA_TYPE_BACK);
    connect(this, SIGNAL(openCameraDialog(QString)),
            getQMainWindow(), SLOT(openQMLDocument(QString)), Qt::QueuedConnection);
#endif
}


void CCameraData::showView(rho::apiGenerator::CMethodResult &oResult)
{
    #ifndef OS_SAILFISH
    if (dialogExists()) return;
    CameraDialogBuilder * builder = new CameraDialogBuilder(this, info, oResult, getQMainWindow());
    emit builder->run();
    #else
    captureResult = oResult;
    emit openCameraDialog("CameraPage.qml");
    #endif
}

QtMainWindow * CCameraData::getQMainWindow()
{
    return IExecutable::getMainWindow();
}

void CCameraData::choosePicture(rho::apiGenerator::CMethodResult& oResult) {
    ImageFileNameGetter * getter = new ImageFileNameGetter(QThread::currentThread(),oResult,getQMainWindow());
    getter->run();
}

QString CCameraData::getTargetFileName() const
{
    return targetFileName;
}

void CCameraData::setTargetFileName(const QString &value)
{
    targetFileName = value;
}
#ifdef OS_SAILFISH
extern "C" {
const char* rho_native_rhopath();
}
void CCameraData::captured(QString fileName){
    qDebug() << "Captured: " + fileName;
    rho::Hashtable<rho::String, rho::String>& mapRes = captureResult.getStringHash();
    if (!fileName.isEmpty()){
        QString targetPath = getTargetFileName();
        QFileInfo info(fileName);
        if (targetPath.isEmpty()){
            targetPath.append(QtMainWindow::getDbFilesDir() + "IMG_" + QDate::currentDate().toString("yyyyMMdd") +
                              QTime::currentTime().toString("hhmmss") + "." + info.suffix());
        }else if (!targetPath.endsWith(info.suffix())){
            targetPath.append("." + info.suffix());
        }
        if (!targetPath.startsWith(QtMainWindow::getDbFilesDir())){
            targetPath.prepend(QtMainWindow::getDbFilesDir());
        }

        //if (mapRes.containsKey("maxWidth") || mapRes.containsKey("maxHeight")){
        QImage image(fileName);
        if (image.width() > image.height()){
            if (image.width() > 1024){
                image.scaledToWidth(1024);
            }
        }else{
            if (image.height() > 1024){
                image.scaledToHeight(1024);
            }
        }
        image.save(targetPath);

            //QFile::copy(fileName, targetPath);

        qDebug() << "Target path is " + targetPath;
        mapRes["status"] = "ok";
        mapRes["imageUri"] = targetPath.toStdString();
    }else{
        mapRes["status"] = "canceled";
        mapRes["message"] = "";
    }
    captureResult.set(mapRes);
    QFile removable(fileName);
    removable.remove();

}
#endif
