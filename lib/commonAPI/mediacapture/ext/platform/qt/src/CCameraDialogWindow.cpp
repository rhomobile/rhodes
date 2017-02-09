#include "CCameraDialogWindow.h"

QHash<QCamera *, CCameraDialogWindow *> CCameraDialogWindow::dialogsKeeper;

CCameraDialogWindow::CCameraDialogWindow(QCamera * camera, QWidget *parent) : QDialog(parent)
{
    cameraObject = camera;

    setMinimumHeight(200);
    setMinimumWidth(300);

    QVBoxLayout * vblay = new QVBoxLayout(this);
    videoWidget = new QVideoWidget(this);
    vblay->addWidget(videoWidget);
    show();

    cameraObject->setCaptureMode(QCamera::CaptureViewfinder);
    cameraObject->setViewfinder(videoWidget);

    connect(this, SIGNAL(rejected()), this, SLOT(buttonReject()));

    cameraObject->start();

    if(dialogsKeeper.contains(cameraObject)){
        dialogsKeeper.value(cameraObject)->deleteLater();
    }
    dialogsKeeper.insert(camera, this);
}

CCameraDialogWindow *CCameraDialogWindow::showInstace(QCamera *camera)
{
    CCameraDialogWindow * dialog = new CCameraDialogWindow(camera, 0);
    connect(camera, SIGNAL(destroyed(QObject*)), dialog, SLOT(hide()));
    connect(camera, SIGNAL(destroyed(QObject*)), dialog, SLOT(deleteLater()));
    return dialog;
}

void CCameraDialogWindow::hideInstace(QCamera *camera)
{
    if (dialogsKeeper.contains(camera)){
        CCameraDialogWindow * dialog = dialogsKeeper.value(camera);
        if (!dialog->isHidden()) dialog->reject();
    }
}

void CCameraDialogWindow::closeDialog()
{
    rejected();
}

CCameraDialogWindow::~CCameraDialogWindow()
{

}

void CCameraDialogWindow::buttonReject()
{
    cameraObject->stop();
}

