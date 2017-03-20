#include "BarcodeDialogView.h"
QSet<QString> BarcodeDialogView::keeper;

BarcodeDialogView::BarcodeDialogView(QCameraInfo &info, QWidget *parent) : QDialog(parent){
    camera = nullptr;
    imageCapture = nullptr;
    localInfo = info;

    decThread = new DecoderThread(this);
    connect(decThread, SIGNAL(encoded(QString)), this, SLOT(encoded(QString)));
    connect(decThread, SIGNAL(scanningProcessMsg()), this, SLOT(scanningProcessMsg()));

    connect(this, SIGNAL(accepted()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(rejected()), this, SLOT(deleteLater()));

    QVBoxLayout * vblay = new QVBoxLayout(this);

    videoWidget = new QVideoWidget(this);
    vblay->addWidget(videoWidget,0,Qt::AlignCenter);

    vblay->addSpacing(30);

    laDecodeResult = new QLabel(this);
    laDecodeResult->setAlignment(Qt::AlignCenter);
    vblay->addWidget(laDecodeResult,0,Qt::AlignHCenter);

    vblay->addSpacing(30);

    QHBoxLayout * hblay = new QHBoxLayout();
    vblay->addLayout(hblay);

    buttonCapture = new QPushButton("Scan", this);
    hblay->addWidget(buttonCapture,0,Qt::AlignCenter);
    connect(buttonCapture, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));

    QPushButton * buttonSave = new QPushButton("Save", this);
    hblay->addWidget(buttonSave,0,Qt::AlignCenter);
    connect(buttonSave, SIGNAL(clicked(bool)), this, SLOT(buttonSaveClicked()));

    this->setMinimumHeight(400);

    vblay->setMargin(1);
    vblay->setSpacing(1);

    // setFixedSize(minimumWidth(), minimumHeight());

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()));

    createCamera(localInfo);
    buttonClicked();

    keeper.insert(localInfo.deviceName());
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

void BarcodeDialogView::encoded(QString text)
{
    laDecodeResult->setText(text);
    qApp->processEvents();
    currentDecodeResult = text;
    timer->stop();
    buttonCapture->setEnabled(true);
}

void BarcodeDialogView::timeOut()
{
    if (!decThread->isRunning()){capture();}
}

void BarcodeDialogView::buttonClicked()
{   
    if (!timer->isActive()){
        timer->start(20);
        buttonCapture->setEnabled(false);
    }else{
        buttonCapture->setEnabled(true);
    }

}

void BarcodeDialogView::buttonSaveClicked()
{
    timer->stop();
    emit saveResult(currentDecodeResult);
    this->accept();
    deleteLater();
}

void BarcodeDialogView::scanningProcessMsg()
{
    static QString msg = "Scanning";
    static int msgFirstSize = msg.size();

    msg.append(".");
    if (msg.size() > (msgFirstSize + 3)){
        msg = "Scanning";
    }
    laDecodeResult->setText(msg);
    qApp->processEvents();

}

