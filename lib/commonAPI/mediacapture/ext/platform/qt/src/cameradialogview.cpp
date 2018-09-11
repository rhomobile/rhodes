#include "CameraDialogView.h"

CameraDialogView::CameraDialogView(QCameraInfo &info, rho::apiGenerator::CMethodResult &oResult,
                                   CameraDialogController *controller, QWidget *parent) : QDialog(parent){
    this->showFullScreen();
    this->setStyleSheet("QDialog { background-color: black }");

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


    QVBoxLayout * vblay = new QVBoxLayout(this);
    videoWidget = new QVideoWidget(this);
    vblay->addWidget(videoWidget,0,Qt::AlignCenter);
    Q_INIT_RESOURCE(mediacapture);

    imageCaptureClose = QIcon(":/mcimages/diaClosed.png");
    imageCaptureOpened = QIcon(":/mcimages/diaOpened.png");

    QHBoxLayout * hblay = new QHBoxLayout();

    buttonCapture = new QPushButton(imageCaptureOpened, "", this);
    buttonCapture->setFixedSize(50,50);
    buttonCapture->setIconSize(QSize(40,40));
    connect(buttonCapture, SIGNAL(clicked(bool)), this, SLOT(capture()));
    connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(imageCaptured(int,QImage)));

    QIcon iconBack = QIcon(":/mcimages/buttonBack.png");
    QPushButton * buttonBack = new QPushButton(iconBack,"",this);
    connect(buttonBack, SIGNAL(clicked(bool)), this, SLOT(reject()));
    hblay->addWidget(buttonBack);
    buttonBack->setFixedSize(50,50);
    buttonBack->setIconSize(QSize(40,40));


    QLabel * empty = new QLabel(this);
    empty->setFixedWidth(50);

    vblay->addSpacing(20);
    vblay->addLayout(hblay);
    hblay->addWidget(buttonBack);
    hblay->addWidget(buttonCapture);
    hblay->addWidget(empty);

    vblay->addSpacing(20);

    vblay->setMargin(1);
    vblay->setSpacing(1);

    currentImage = QImage(640,480,QImage::Format_ARGB32);

    camera->setCaptureMode(QCamera::CaptureViewfinder);
    camera->focus()->setFocusMode(QCameraFocus::AutoFocus);
    camera->exposure()->setExposureMode(QCameraExposure::ExposureAuto);
    camera->exposure()->setAutoIsoSensitivity();

    camera->setViewfinder(videoWidget);

    connect(this, SIGNAL(rejected()), this, SLOT(error()));
    connect(this, SIGNAL(rejected()), this, SLOT(deleteLater()), Qt::QueuedConnection);
    camera->start();
    camera->searchAndLock();
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
    mapRes["status"] = "cancel";
    mapRes["message"] = "Dialog canceled without saving";
    oResult.set(mapRes);
}


void CameraDialogView::capture()
{
    buttonCapture->setIcon(imageCaptureClose);
    imageCapture->capture("dev/null");
    QTimer::singleShot(300, this, SLOT(restoreCaptureButtonImage()));
}

void CameraDialogView::imageCaptured(int id, const QImage &preview)
{
    currentImage = preview;
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
    QTimer::singleShot(300, this, SLOT(saveCurrentImage()));
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
    QDir dir = QDir(qApp->applicationDirPath()+"/db/db-files/");
    if (!dir.exists()) dir.mkpath(dir.absolutePath());
    if (dir.exists()){return dir;}
    else{QDir::current();}
}


