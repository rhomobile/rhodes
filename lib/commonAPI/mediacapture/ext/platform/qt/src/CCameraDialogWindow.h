#ifndef CCAMERADIALOGWINDOW_H
#define CCAMERADIALOGWINDOW_H

#include <QDialog>
#include <QCamera>
#include <QVBoxLayout>
#include <QDebug>
#include <QVideoWidget>
#include <QEventLoop>
#include <QTimer>
#include "../../platform/shared/qt/rhodes/QtMainWindow.h"
#include "../../platform/shared/qt/rhodes/impl/MainWindowImpl.h"
#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "ImageFileNameGetter.h"

class CCameraDialogWindow : public QDialog
{
    Q_OBJECT
public:
    static CCameraDialogWindow *showInstace(QCamera *camera);
    static void hideInstace(QCamera *camera);
    static QtMainWindow *getQMainWindow();
    void closeDialog();
    ~CCameraDialogWindow();
    static void choosePicture(rho::apiGenerator::CMethodResult &oResult);
private:
    QCamera * cameraObject;
    QVideoWidget * videoWidget;
    explicit CCameraDialogWindow(QCamera *camera, QWidget *parent = 0);
    static QHash<QCamera *, CCameraDialogWindow *> dialogsKeeper;
signals:

public slots:
    void buttonReject();
};

#endif // CCAMERADIALOGWINDOW_H
