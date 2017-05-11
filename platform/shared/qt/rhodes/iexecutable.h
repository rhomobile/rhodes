#ifndef IEXECUTABLE_H
#define IEXECUTABLE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QMainWindow>
#include <QApplication>

class IExecutable : public QObject
{
    Q_OBJECT
public:
    explicit IExecutable(QObject *parent){
        moveToThread(parent->thread());
        //setParent(parent);
        connect(parent, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));
        connect(this, SIGNAL(run()), this, SLOT(execute()), Qt::QueuedConnection);
#ifdef RHODES_MAC_BUILD
        qDebug() << "Creating Executable in thred ID 0x" + QString::number((quint64)QThread::currentThreadId(), 16);
#else
        qDebug() << "Creating Executable in thred ID 0x" + QString::number((quint32)QThread::currentThreadId(), 16);
#endif
    }
    static QMainWindow * getMainWindow(){
        foreach (QWidget * widget, qApp->topLevelWidgets()) {
            if (widget->inherits("QMainWindow")) return qobject_cast<QMainWindow *>(widget);
        }
        return nullptr;
    }

    virtual ~IExecutable(){}

signals:
    void run();
public slots:
    virtual void execute(){deleteLater();}
};

#endif // IEXECUTABLE_H
