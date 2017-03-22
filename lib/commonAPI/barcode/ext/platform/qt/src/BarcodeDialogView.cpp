#include "BarcodeDialogView.h"
QSet<QString> BarcodeDialogView::keeper;

BarcodeDialogView::BarcodeDialogView(QCameraInfo &info, QWidget *parent) : QDialog(parent){

    this->showFullScreen();
    this->setStyleSheet("QDialog { background-color: black }");

    camera = nullptr;
    imageCapture = nullptr;
    localInfo = info;

    decThread = new DecoderThread(this);
    connect(decThread, SIGNAL(encoded(QString, QString)), this, SLOT(encoded(QString, QString)));
    connect(decThread, SIGNAL(scanningProcessMsg()), this, SLOT(scanningProcessMsg()));

    connect(this, SIGNAL(accepted()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(rejected()), this, SLOT(deleteLater()));

    QVBoxLayout * vblay = new QVBoxLayout(this);

    videoWidget = new QVideoWidget(this);
    vblay->addWidget(videoWidget,0,Qt::AlignCenter);

    vblay->addSpacing(20);

    laDecodeResult = new QLabel(this);
    laDecodeResult->setAlignment(Qt::AlignCenter);
    laDecodeResult->setStyleSheet("QLabel {color : white; }");
    vblay->addWidget(laDecodeResult,0,Qt::AlignHCenter);

    vblay->addSpacing(20);
    vblay->setMargin(1);
    vblay->setSpacing(1);

    // setFixedSize(minimumWidth(), minimumHeight());

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()));

    createCamera(localInfo);

    keeper.insert(localInfo.deviceName());

    Q_INIT_RESOURCE(barcode);
    timer->start(50);
}

BarcodeDialogView::~BarcodeDialogView()
{
    keeper.remove(localInfo.deviceName());
    if (imageCapture != nullptr){imageCapture->deleteLater();}
    if (camera != nullptr) {
        camera->stop();
        camera->deleteLater();
    }
}

bool BarcodeDialogView::dialogExists(QCameraInfo &info)
{
    return keeper.contains(info.deviceName());
}

void BarcodeDialogView::createCamera(QCameraInfo &info)
{
    if (imageCapture != nullptr){
        imageCapture->deleteLater();
    }
    if (camera != nullptr) {
        camera->stop();
        camera->deleteLater();
    }

    camera = new QCamera(info, this);
    camera->focus()->setFocusMode(QCameraFocus::AutoFocus);
    camera->focus()->setFocusPointMode(QCameraFocus::FocusPointCenter);
    camera->exposure()->setExposureMode(QCameraExposure::ExposureAuto);
    camera->exposure()->setAutoIsoSensitivity();
    camera->setViewfinder(videoWidget);

    imageCapture = new QCameraImageCapture(camera, this);
    imageCapture->setBufferFormat(QVideoFrame::Format_RGB32);

    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    QImageEncoderSettings settings = imageCapture->encodingSettings();
    settings.setQuality(QMultimedia::EncodingQuality::VeryHighQuality);
    imageCapture->setEncodingSettings(settings);

    connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), decThread, SLOT(imageCaptured(int,QImage)));

    camera->start();
    camera->searchAndLock();
}


void BarcodeDialogView::capture()
{
    imageCapture->capture("dev/null");
}

void BarcodeDialogView::encoded(QString text, QString format)
{
    timer->stop();
    disconnect(decThread, SIGNAL(scanningProcessMsg()), this, SLOT(scanningProcessMsg()));
    QSound::play(":/barcodesounds/scanner_sound.wav");
    laDecodeResult->setText(text);
    qApp->processEvents();  
    emit saveResult(text, format);
    QTimer::singleShot(500,this,SLOT(accept()));
}

void BarcodeDialogView::timeOut()
{
    if (!decThread->isRunning()){capture();}
}


void BarcodeDialogView::scanningProcessMsg()
{
    static QString msg = "Scanning";
    static int msgFirstSize = msg.size();

    if (msg.size() < (msgFirstSize + 10)){
        msg.append(" .");
    }else{
        msg = "Scanning";
    }
    laDecodeResult->setText(msg);
    qApp->processEvents();
}

