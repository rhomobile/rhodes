#include "CameraDialogView.h"


CameraDialogView::CameraDialogView(QCameraInfo &info, rho::apiGenerator::CMethodResult &oResult,
                                   CameraDialogController *controller, QWidget *parent) : QDialog(parent){
    camera = new QCamera(info, this);
    this->oResult = oResult;
    this->controller = controller;
    imageIsSaved = false;

    imageCapture = new QCameraImageCapture(camera, this);
    imageCapture->setBufferFormat(QVideoFrame::Format_RGB32);

    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    QImageEncoderSettings settings = imageCapture->encodingSettings();
    settings.setQuality(QMultimedia::EncodingQuality::VeryHighQuality);
    imageCapture->setEncodingSettings(settings);

    connect(imageCapture, SIGNAL(imageSaved(int, const QString)), this, SLOT(imageSaved(int,const QString)));

    QHBoxLayout * hblay = new QHBoxLayout(this);

    QVBoxLayout * vblay = new QVBoxLayout();
    videoWidget = new QVideoWidget(this);
    vblay->addWidget(videoWidget,0,Qt::AlignCenter);
    videoWidget->setFixedWidth(300);
    Q_INIT_RESOURCE(mediacapture);

    imageCaptureClose = QIcon(":/mcimages/diaClosed.png");

    imageCaptureOpened = QIcon(":/mcimages/diaOpened.png");
    imageSave = QIcon(":/mcimages/photoSave.png");

    connect(&timerToRestoreCaptureButtonImage, SIGNAL(timeout()), this, SLOT(restoreCaptureButtonImage()));
    timerToRestoreCaptureButtonImage.setSingleShot(true);

    buttonCapture = new QPushButton(imageCaptureOpened, "", this);
    buttonCapture->setFixedSize(50,50);
    buttonCapture->setIconSize(QSize(40,40));
    vblay->addWidget(buttonCapture,0,Qt::AlignCenter);
    connect(buttonCapture, SIGNAL(clicked(bool)), this, SLOT(capture()));
    connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(imageCaptured(int,QImage)));
    hblay->addLayout(vblay);

    vblay = new QVBoxLayout();
    laPreview = new QLabel(this);
    vblay->addWidget(laPreview,0,Qt::AlignCenter);
    QPushButton * buttonSave = new QPushButton(imageSave,"",this);
    buttonSave->setFixedSize(50,50);
    buttonSave->setIconSize(QSize(35,35));

    connect(buttonSave, SIGNAL(clicked(bool)), this, SLOT(saveCurrentImage()));
    vblay->addWidget(buttonSave,0,Qt::AlignCenter);
    hblay->addSpacing(4);
    hblay->addLayout(vblay);
    hblay->setMargin(1);
    hblay->setSpacing(1);

    currentImage = QImage(640,480,QImage::Format_ARGB32);
    laPreview->setPixmap(QPixmap::fromImage(currentImage.scaledToWidth(videoWidget->size().width())));

    camera->setCaptureMode(QCamera::CaptureViewfinder);
    camera->focus()->setFocusMode(QCameraFocus::AutoFocus);
    camera->exposure()->setExposureMode(QCameraExposure::ExposureAuto);
    camera->exposure()->setAutoIsoSensitivity();

    camera->setViewfinder(videoWidget);

    connect(this, SIGNAL(rejected()), this, SLOT(error()));
    connect(this, SIGNAL(rejected()), this, SLOT(deleteLater()), Qt::QueuedConnection);
    camera->start();
    camera->searchAndLock();

    setFixedSize(minimumWidth(), minimumHeight());
}

CameraDialogView::~CameraDialogView()
{
    controller->setDialogRejected();
}


void CameraDialogView::imageSaved(int id, const QString &fileName)
{
    qDebug() << "IMSaved: " + fileName;
}


void CameraDialogView::error()
{
    if (imageIsSaved) return;
    qDebug() << "Error in Taking Picture";

    rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
    mapRes["status"] = "error";
    mapRes["message"] = "Dialog canceled without saving";
    oResult.set(mapRes);
}


void CameraDialogView::capture()
{
    buttonCapture->setIcon(imageCaptureClose);
    imageCapture->capture("dev/null");
    timerToRestoreCaptureButtonImage.start(300);
}

void CameraDialogView::imageCaptured(int id, const QImage &preview)
{
    currentImage = preview;
    laPreview->setPixmap(QPixmap::fromImage(currentImage.scaledToWidth(videoWidget->size().width())));
}


void CameraDialogView::saveCurrentImage()
{
    imageIsSaved = true;
    QDir dir = getImageDir();

    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QString fullPath = dir.absolutePath() + "/img" + QString::number(date.year()) +
        QString::number(date.month()) + QString::number(date.day()) +
        QString::number(time.hour()) + QString::number(time.minute()) +
        QString::number(time.second()) + QString::number(time.msec()) + ".jpg";
    if (currentImage.save(fullPath)){
        rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
        getImageData(mapRes, fullPath);
        oResult.set(mapRes);
        reject();
    }else{
        rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
        mapRes["status"] = "error";
        mapRes["message"] = "Error in saving image process";
        oResult.set(mapRes);
        reject();
    }
}

void CameraDialogView::restoreCaptureButtonImage()
{
    buttonCapture->setIcon(imageCaptureOpened);
}


void CameraDialogView::getImageData(rho::Hashtable<rho::String, rho::String> &mapRes, const QString &fileNameToOpen){
    QImage image(fileNameToOpen);
    if (image.isNull()){
        mapRes["status"] = "error";
        mapRes["message"] = "Image loading error";
        return;
    }
    mapRes["status"] = "ok";

    rho::String uri = QDir::current().relativeFilePath(fileNameToOpen).toStdString();
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

QDir CameraDialogView::getImageDir()
{
    QDir dir = QDir(QDir::current().absolutePath()+"/db/db-files/");
    if (!dir.exists()) dir.mkpath(dir.absolutePath());
    return dir;
}


