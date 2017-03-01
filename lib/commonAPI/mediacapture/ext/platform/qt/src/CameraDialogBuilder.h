#ifndef CAMERADIALOGBUILDER_H
#define CAMERADIALOGBUILDER_H

#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "CameraDialogView.h"
#include "CameraDialogController.h"

class CameraDialogBuilder : public IExecutable
{
    Q_OBJECT
private:
    CameraDialogController * controller;
    QCameraInfo info;
    QWidget * parentWidget;
    rho::apiGenerator::CMethodResult oResult;
public:
    explicit CameraDialogBuilder(CameraDialogController * controller, QCameraInfo &info,
                                 rho::apiGenerator::CMethodResult &oResult, QWidget *parent) : IExecutable(parent){
        this->controller = controller;
        this->info = info;
        this->oResult = oResult;
        parentWidget = parent;
    }

public slots:
    void execute(){
        CameraDialogView * dialog = new CameraDialogView(info, oResult, controller, parentWidget);
        connect(parentWidget, SIGNAL(destroyed(QObject*)), dialog, SLOT(reject()));
        connect(parentWidget, SIGNAL(destroyed(QObject*)), dialog, SLOT(deleteLater()));
        dialog->show();
        controller->setDialogExists();
        deleteLater();
    }
};

#endif // CAMERADIALOGBUILDER_H
