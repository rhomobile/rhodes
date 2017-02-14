#ifndef CCAMERADIALOGWINDOWS_H
#define CCAMERADIALOGWINDOWS_H

#include <QThread>
#include <QTimer>
#include "../../platform/shared/qt/rhodes/QtMainWindow.h"
#include "../../platform/shared/qt/rhodes/impl/MainWindowImpl.h"
#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "ImageFileNameGetter.h"
#include "CameraDialogController.h"
#include "CameraDialogBuilder.h"

class CCameraDialogWindows : public CameraDialogController
{
    Q_OBJECT
public:
    static void showInstace(QCamera *camera);
    static void hideInstace(QCamera *camera);
    static QtMainWindow *getQMainWindow();
    static void choosePicture(rho::apiGenerator::CMethodResult &oResult);
    explicit CCameraDialogWindows(QCamera *camera);
private:
    static QHash<QCamera *, CameraDialogController *> dialogsKeeper;

};

#endif // CCAMERADIALOGWINDOWS_H
