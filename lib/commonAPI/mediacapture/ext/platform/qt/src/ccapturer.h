#ifndef CCAPTURER_H
#define CCAPTURER_H

#include <QMediaRecorder>
#include <QCameraImageCapture>
#include <QEventLoopLocker>
#include <QCamera>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QFileInfo>
#include <QCameraFocus>
#include <QCameraExposure>
#include <QImageEncoderSettings>
#include <QDir>
#include "../../../../../platform/shared/api_generator/MethodResult.h"
#include "../../../../../platform/shared/common/RhoStd.h"

class CCapturer : public QObject
{
    Q_OBJECT
private:
    CCapturer();
    QCameraImageCapture * imageCapture;
    QTimer timer;
    QCamera * camera;
    rho::apiGenerator::CMethodResult oResult;
    bool stopCamera;
public:
    explicit CCapturer(QCamera *camera, QObject *parent = 0);
    ~CCapturer();
    static void getImageData(rho::Hashtable<rho::String, rho::String> &mapRes, const QString &fileNameToOpen);
private slots:
    void imageSaved(int id, const QString &fileName);
    void error();
public slots:
    void capture(rho::apiGenerator::CMethodResult result);

};

#endif // CCAPTURER_H
