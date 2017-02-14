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
    QCamera * camera;
    QWidget * parentWidget;
public:
    explicit CameraDialogBuilder(CameraDialogController * controller, QCamera * camera, QWidget *parent) : IExecutable(parent){
        this->controller = controller;
        this->camera = camera;
        parentWidget = parent;
    }

public slots:
    void execute(){
        CameraDialogView * dialog = new CameraDialogView(camera, parentWidget);
        connect(controller, SIGNAL(show()), dialog, SLOT(show()));
        connect(controller, SIGNAL(hide()), dialog, SLOT(hide()));
        connect(controller, SIGNAL(destroyed(QObject*)), dialog, SLOT(reject()));
        connect(controller, SIGNAL(destroyed(QObject*)), dialog, SLOT(deleteLater()));
        deleteLater();
    }
};

#endif // CAMERADIALOGBUILDER_H
