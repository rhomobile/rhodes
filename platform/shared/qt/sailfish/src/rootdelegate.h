#ifndef ROOTDELEGATE_H
#define ROOTDELEGATE_H

#include <QObject>
#include <auroraapp.h>
#include <QCoreApplication>

class RootDelegate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString cover READ getCover WRITE setCover NOTIFY coverChanged)
    Q_PROPERTY(bool webEngineEnabled READ getWebEngineEnabled CONSTANT)
public:
    static RootDelegate * getInstance(QObject * parent = 0){
        static RootDelegate * instance = new RootDelegate(parent);
        return instance;
    }

    QString cover;
    bool webEngineEnabled;


private:
    explicit RootDelegate(QObject *parent = 0) : QObject(parent)
    {
#ifndef ENABLE_Q_WEB_ENGINE
        webEngineEnabled = false;
#else
        webEngineEnabled = true;
#endif
    }
signals:
    void coverChanged();

public slots:
    QString getCover()
    {
        return cover;
    }
    void setCover(QString value)
    {
        cover = value;
        coverChanged();
    }
    bool getWebEngineEnabled() const
    {
        return webEngineEnabled;
    }
};

#endif // ROOTDELEGATE_H


