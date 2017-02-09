#include "CCapturer.h"

CCapturer::CCapturer(QCamera * camera, QObject *parent) : QObject(parent)
{
    result = new Result();
    result->errorFlag = true;
    imageCapture = new QCameraImageCapture(camera);
    QThread * anotherThread = new QThread(this);
    imageCapture->moveToThread(anotherThread);
    imageCapture->setBufferFormat(QVideoFrame::Format_RGB32);
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    connect(&timer, SIGNAL(timeout()), &looper, SLOT(quit()));

    connect(imageCapture, SIGNAL(destroyed(QObject*)), anotherThread, SLOT(quit()));
    connect(imageCapture, SIGNAL(destroyed(QObject*)), anotherThread, SLOT(deleteLater()));
    connect(imageCapture, SIGNAL(imageSaved(int, const QString)), this, SLOT(imageSaved(int,QString)));
    connect(imageCapture, SIGNAL(error(int,QCameraImageCapture::Error,QString)), this, SLOT(error()));
    anotherThread->start(QThread::TimeCriticalPriority);

    if (camera->isAvailable()) {

        if (camera->state() != QCamera::ActiveState){
            camera->setCaptureMode(QCamera::CaptureStillImage);
            camera->start();
            connect(this, SIGNAL(workingEnds()), camera, SLOT(stop()));
            //this->thread()->msleep(100);
        }

    timer.start(15000);
    imageCapture->capture();
    looper.exec();
    }
}

CCapturer::~CCapturer()
{
    timer.stop();
    imageCapture->deleteLater();
    delete result;
}



void CCapturer::imageSaved(int id, const QString &fileName)
{
    result->savedFileName = fileName;
    looper.quit();
    timer.stop();
    emit workingEnds();
}

void CCapturer::error()
{
    result->errorFlag = true;
    result->errorMessage = imageCapture->errorString();
    looper.quit();
    emit workingEnds();
}


CCapturer::Result *CCapturer::getResult() const
{
    return result;
}
