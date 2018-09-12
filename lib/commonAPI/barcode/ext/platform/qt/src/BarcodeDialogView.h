#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "decoderthread.h"
#include <QDialog>
#include <QWidget>
#include <QCamera>
#include <QVBoxLayout>
#include <QDebug>
#ifndef OS_SAILFISH
#include <QVideoWidget>
#endif
#include <QThread>
#include <QDebug>
#include <QFileInfo>
#include <QCameraFocus>
#include <QCameraExposure>
#include <QImageEncoderSettings>
#include <QDir>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QPushButton>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QTime>
#include <QComboBox>
#include <QTextEdit>
#include <QVideoRendererControl>
#include <QTimer>
#include <QLabel>
#include <QSet>
#include <QSound>
#include <QIcon>

class BarcodeDialogView : public QDialog
{
    Q_OBJECT
public:
    explicit BarcodeDialogView(QCameraInfo &info, QWidget *parent);
    ~BarcodeDialogView();
    static bool dialogExists(QCameraInfo &info);

private:
    QVideoWidget * videoWidget;
    QCamera * camera;
    QCameraImageCapture * imageCapture;
    QTimer * timer;
    DecoderThread * decThread;
    QLabel * laDecodeResult;
    QCameraInfo localInfo;
    static QSet<QString> keeper;
    QString text;
    QString format;
signals:
    void saveResult(QString, QString);
    void enableButtons(bool);
public slots:
    void createCamera(QCameraInfo & info);
    void capture();
    void encoded(QString text, QString format);
    void timeOut();
    void scanningProcessMsg();
    void saveBarcode();
    void retry();

};

#endif // MAINWIDGET_H
