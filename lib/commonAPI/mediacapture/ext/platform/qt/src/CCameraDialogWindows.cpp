#include "CCameraDialogWindows.h"

QHash<QCamera *, CameraDialogController *> CCameraDialogWindows::dialogsKeeper;

CCameraDialogWindows::CCameraDialogWindows(QCamera * camera):CameraDialogController(camera)
{
    if(dialogsKeeper.contains(camera)){
        dialogsKeeper.value(camera)->deleteLater();
    }
    dialogsKeeper.insert(camera, this);
}

void CCameraDialogWindows::showInstace(QCamera *camera)
{
    if (dialogsKeeper.contains(camera)){
        emit dialogsKeeper.value(camera)->show();
    }else{
        CameraDialogController * controller = new CCameraDialogWindows(camera);
        CameraDialogBuilder * builder = new CameraDialogBuilder(controller,camera,getQMainWindow());
        emit builder->run();
        emit controller->show();
    }
}

void CCameraDialogWindows::hideInstace(QCamera *camera)
{
    if (dialogsKeeper.contains(camera)){
        emit dialogsKeeper.value(camera)->hide();
    }
}

QtMainWindow *CCameraDialogWindows::getQMainWindow()
{
    return ((QtMainWindow *) CMainWindow::getInstance()->getQtMainWindow());
}

void CCameraDialogWindows::choosePicture(rho::apiGenerator::CMethodResult& oResult) {
    ImageFileNameGetter * getter = new ImageFileNameGetter(QThread::currentThread(),oResult,getQMainWindow());
    getter->run();
}
