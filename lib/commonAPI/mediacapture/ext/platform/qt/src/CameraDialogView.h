#ifndef CAMERADIALOGVIEW_H
#define CAMERADIALOGVIEW_H

#include <QDialog>
#include <QCamera>
#include <QVBoxLayout>
#include <QDebug>
#include <QVideoWidget>
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
#include "../../../../../platform/shared/api_generator/MethodResult.h"
#include "../../../../../platform/shared/common/RhoStd.h"
#include "../../platform/shared/qt/rhodes/QtMainWindow.h"
#include "../../platform/shared/qt/rhodes/impl/MainWindowImpl.h"
#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "CameraDialogController.h"


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
    QLabel * laPreview;
    QImage currentImage;
    bool imageIsSaved;
public slots:
    void imageSaved(int id, const QString &fileName);
    void error();
    void capture();
    void imageCaptured(int id, const QImage &preview);
    void saveCurrentImage();


};

#endif // CAMERADIALOGVIEW_H
