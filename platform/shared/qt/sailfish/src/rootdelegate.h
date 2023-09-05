#ifndef ROOTDELEGATE_H
#define ROOTDELEGATE_H

#include <QObject>
#include <QCoreApplication>

class RootDelegate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString cover READ getCover WRITE setCover NOTIFY coverChanged)
public:
    static RootDelegate * getInstance(QObject * parent = 0){
        static RootDelegate * instance = new RootDelegate(parent);
        return instance;
    }

    QString cover;
private:
    explicit RootDelegate(QObject *parent = 0) : QObject(parent)
    {

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

};

#endif // ROOTDELEGATE_H


