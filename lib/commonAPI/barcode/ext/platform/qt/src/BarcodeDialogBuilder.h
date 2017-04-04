#ifndef BARCODEDIALOGBUILDER_H
#define BARCODEDIALOGBUILDER_H

#include "../../platform/shared/qt/rhodes/iexecutable.h"
#include "../../../../../platform/shared/api_generator/MethodResult.h"
#include "../../ext/shared/generated/cpp/BarcodeBase.h"
#include "BarcodeDialogView.h"

class BarcodeDialogBuilder : public IExecutable
{
    Q_OBJECT
private:
    QCameraInfo info;
    QWidget * parentWidget;
    rho::apiGenerator::CMethodResult oResult;
public:
    explicit BarcodeDialogBuilder(QCameraInfo info,
                                 rho::apiGenerator::CMethodResult &oResult, QWidget *parent) : IExecutable(parent){
        this->info = info;
        this->oResult = oResult;
        parentWidget = parent;
    }

public slots:
    void execute(){
        if (!BarcodeDialogView::dialogExists(info)){
            BarcodeDialogView * dialog = new BarcodeDialogView(info, parentWidget);
            connect(parentWidget, SIGNAL(destroyed(QObject*)), dialog, SLOT(reject()));
            connect(parentWidget, SIGNAL(destroyed(QObject*)), dialog, SLOT(deleteLater()));
            connect(dialog, SIGNAL(rejected()), this, SLOT(rejected()));
            connect(dialog, SIGNAL(saveResult(QString, QString)), this, SLOT(resultHandler(QString, QString)));
            connect(dialog, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));
            dialog->show();
        }else{
            deleteLater();
        }
    }
    void resultHandler(QString tag, QString format){
        rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
        mapRes[rho::barcode::HK_STATUS] = "ok";
        mapRes[rho::barcode::HK_BARCODE] = tag.toStdString();
        mapRes["format"] = format.toStdString();
        oResult.set(mapRes);
    }
    void rejected(){
        rho::Hashtable<rho::String, rho::String>& mapRes = oResult.getStringHash();
        mapRes[rho::barcode::HK_STATUS] = "cancel";
        oResult.set(mapRes);
    }
};

#endif // BARCODEDIALOGBUILDER_H
