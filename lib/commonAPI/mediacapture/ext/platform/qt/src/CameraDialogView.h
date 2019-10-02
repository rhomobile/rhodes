#ifndef CAMERADIALOGVIEW_H
#define CAMERADIALOGVIEW_H

#include <QDialog>
#include <QCamera>
#include <QVBoxLayout>
#include <QDebug>
#ifndef OS_SAILFISH
#include <QVideoWidget>
#endif
#include <QEventLoop>
#include <QThread>
#include <QDebug>
#include <QFileInfo>
#include <QCameraFocus>
#include <QCameraExposure>
#include <QImageEncoderSettings>
#include <QDir>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QPushButton>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QTime>
#include <QDate>
#include <QTimer>
#include <QIcon>
#include "../../../../../platform/shared/api_generator/MethodResult.h"
#include "../../../../../platform/shared/common/RhoStd.h"
#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "CameraDialogController.h"
#include <QtPlugin>
#include <QPluginLoader>
#include <QJsonObject>
#include <QTextCodec>

class CameraDialogView : public QDialog
{
    Q_OBJECT
public:
    explicit CameraDialogView(QCameraInfo & info, rho::apiGenerator::CMethodResult &oResult,
                              CameraDialogController * controller, QWidget *parent);
    ~CameraDialogView();

    static void getImageData(rho::Hashtable<rho::String, rho::String> & mapRes, const QString &fileNameToOpen);
    static QDir getImageDir();
private:
    QVideoWidget * videoWidget;
    rho::apiGenerator::CMethodResult oResult;
    QCameraImageCapture * imageCapture;
    QCamera * camera;
    CameraDialogController * controller;
    QImage currentImage;
    bool imageIsSaved;
    QIcon imageCaptureClose;
    QIcon imageCaptureOpened;
    QPushButton * buttonCapture;
public slots:
    void imageSaved(int id, const QString &fileName);
    void error();
    void capture();
    void imageCaptured(int id, const QImage &preview);
    void saveCurrentImage();
    void restoreCaptureButtonImage();

};

#endif // CAMERADIALOGVIEW_H
