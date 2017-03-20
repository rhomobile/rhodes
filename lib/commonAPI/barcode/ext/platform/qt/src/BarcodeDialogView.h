#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "decoderthread.h"
#include <QDialog>
#include <QWidget>
#include <QCamera>
#include <QVBoxLayout>
#include <QDebug>
#include <QVideoWidget>
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
    QPushButton * buttonCapture;
    QCameraImageCapture * imageCapture;
    QTimer * timer;
    DecoderThread * decThread;
    QLabel * laDecodeResult;
    QString currentDecodeResult;
    QCameraInfo localInfo;
    static QSet<QString> keeper;
signals:
    void saveResult(QString);
public slots:
    void createCamera(QCameraInfo & info);
    void capture();
    void encoded(QString text);
    void timeOut();
    void buttonClicked();
    void buttonSaveClicked();
    void scanningProcessMsg();


};

#endif // MAINWIDGET_H
