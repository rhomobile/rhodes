#ifndef IEXECUTABLE_H
#define IEXECUTABLE_H

#include <QObject>
#include <QThread>
#include <QDebug>

class IExecutable : public QObject
{
    Q_OBJECT
public:
    explicit IExecutable(QObject *parent){
        moveToThread(parent->thread());
        //setParent(parent);
        connect(parent, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));
        connect(this, SIGNAL(run()), this, SLOT(execute()), Qt::QueuedConnection);
        qDebug() << "Creating Executable in thred ID 0x" + QString::number((quint32)QThread::currentThreadId(), 16);
    }
    virtual ~IExecutable(){}

signals:
    void run();
public slots:
    virtual void execute(){deleteLater();}
};

#endif // IEXECUTABLE_H
