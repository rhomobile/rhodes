#ifndef CCAPTURER_H
#define CCAPTURER_H

#include <QMediaRecorder>
#include <QCameraImageCapture>
#include <QEventLoopLocker>
#include <QCamera>
#include <QTimer>
#include <QThread>

class CCapturer : public QObject
{
    Q_OBJECT
private:
    CCapturer();
    QEventLoop looper;
    QCameraImageCapture * imageCapture;
    QTimer timer;

    bool stopCamera;
public:

    struct Result{
        bool errorFlag;
        QString errorMessage;
        QString savedFileName;
        Result(){
            errorFlag = true;
            errorMessage = "No camera found";
        }
    };

    explicit CCapturer(QCamera *camera, QObject *parent = 0);
    ~CCapturer();
    Result * getResult() const;

private:
    Result * result;
signals:
    void workingEnds();
public slots:
    void imageSaved(int id, const QString &fileName);
    void error();
};

#endif // CCAPTURER_H
