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
    Q_INIT_RESOURCE(barcode);
    // setFixedSize(minimumWidth(), minimumHeight());
    QHBoxLayout * hblay = new QHBoxLayout();
    vblay->addLayout(hblay);

    QIcon iconBack = QIcon(":/barcodeimages/buttonBack.png");
    QPushButton * buttonBack = new QPushButton(iconBack,"",this);
    connect(buttonBack, SIGNAL(clicked(bool)), this, SLOT(reject()));
    hblay->addWidget(buttonBack);
    buttonBack->setFixedSize(50,50);
    buttonBack->setIconSize(QSize(40,40));

    QIcon iconRetry = QIcon(":/barcodeimages/buttonRetry.png");
    QPushButton * buttonRetry = new QPushButton(iconRetry,"",this);
    connect(buttonRetry, SIGNAL(clicked(bool)), this, SLOT(retry()));
    connect(this, SIGNAL(enableButtons(bool)), buttonRetry, SLOT(setEnabled(bool)));
    hblay->addWidget(buttonRetry);
    buttonRetry->setFixedSize(50,50);
    buttonRetry->setIconSize(QSize(40,40));

    QIcon iconSave = QIcon(":/barcodeimages/buttonSave.png");
    QPushButton * buttonSave = new QPushButton(iconSave,"",this);
    connect(buttonSave, SIGNAL(clicked(bool)), this, SLOT(saveBarcode()));
    connect(this, SIGNAL(enableButtons(bool)), buttonSave, SLOT(setEnabled(bool)));
    hblay->addWidget(buttonSave);
    buttonSave->setFixedSize(50,50);
    buttonSave->setIconSize(QSize(40,40));

    vblay->addSpacing(20);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()));

    createCamera(localInfo);

    keeper.insert(localInfo.deviceName());


    enableButtons(false);
    retry();
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
    this->text = text;
    this->format = format;
    qApp->processEvents();
    QSound::play(":/barcodesounds/scanner_sound.wav");
    laDecodeResult->setText(format + " : " + text);
    qApp->processEvents();  
    emit enableButtons(true);
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

void BarcodeDialogView::saveBarcode()
{
    timer->stop();
    emit saveResult(text, format);
    qApp->processEvents();
    accept();
}

void BarcodeDialogView::retry()
{
    timer->start(50);
    emit enableButtons(false);
}

