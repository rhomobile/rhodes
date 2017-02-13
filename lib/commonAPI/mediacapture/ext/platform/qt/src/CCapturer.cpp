#include "CCapturer.h"

CCapturer::CCapturer(QCamera * camera, QObject *parent) : QObject(parent)
{

    this->camera = camera;
    timer.setSingleShot(true);

    imageCapture = new QCameraImageCapture(camera,camera);
    imageCapture->setBufferFormat(QVideoFrame::Format_RGB32);
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    QImageEncoderSettings settings = imageCapture->encodingSettings();
    settings.setQuality(QMultimedia::EncodingQuality::VeryHighQuality);
    //settings.setCodec("image/png");

    imageCapture->setEncodingSettings(settings);


    connect(imageCapture, SIGNAL(imageSaved(int, const QString)), this, SLOT(imageSaved(int,const QString)));
    connect(imageCapture, SIGNAL(error(int,QCameraImageCapture::Error,QString)), this, SLOT(error()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(error()));


}

CCapturer::~CCapturer()
{
    timer.stop();
}

void CCapturer::imageSaved(int id, const QString &fileName)
{
    rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
    qDebug() << "FILE SAVED TO " + fileName;
    getImageData( mapRes, fileName);

    oResult.set(mapRes);
    timer.stop();
    if (stopCamera){
        camera->unlock();
        camera->stop();
    }
}

void CCapturer::error()
{
    qDebug() << "Error in Taking Picture";
    rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
    mapRes["status"] = "error";
    mapRes["message"] = "Camera error";
    oResult.set(mapRes);    
    if (stopCamera){
        camera->unlock();
        camera->stop();
    }
}

void CCapturer::capture(rho::apiGenerator::CMethodResult result)
{

    oResult = result;

    if (camera->isAvailable()) {
        if (camera->state() != QCamera::ActiveState){
            camera->setCaptureMode(QCamera::CaptureStillImage);
            camera->focus()->setFocusMode(QCameraFocus::AutoFocus);
            camera->exposure()->setExposureMode(QCameraExposure::ExposureAuto);
            camera->exposure()->setAutoIsoSensitivity();
            camera->start();

            camera->searchAndLock();


            stopCamera = true;
        }else{
            stopCamera = false;
        }
    }
    if (!camera->isAvailable()){
        error();
        return;
    }

    timer.start(10000);

    imageCapture->capture();
}



void CCapturer::getImageData(rho::Hashtable<rho::String, rho::String> & mapRes, const QString &fileNameToOpen){
    QImage image(fileNameToOpen);
    if (image.isNull()){
        mapRes["status"] = "error";
        mapRes["message"] = "Image loading error";
        return;
    }
    mapRes["status"] = "ok";

    rho::String uri = fileNameToOpen.toStdString();
    rho::String height = QString::number(image.height()).toStdString();
    rho::String width = QString::number(image.width()).toStdString();
    rho::String format = (QFileInfo(fileNameToOpen)).suffix().toStdString();

    mapRes["imageUri"] = uri;
    mapRes["imageHeight"] = height;
    mapRes["imageWidth"] = width;
    mapRes["imageFormat"] = format;

    mapRes["image_uri"] = uri;
    mapRes["image_height"] = height;
    mapRes["image_width"] = width;
    mapRes["image_format"] = format;
}
