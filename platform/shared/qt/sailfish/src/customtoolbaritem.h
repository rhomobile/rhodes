#ifndef CUSTOMTOOLBARITEM_H
#define CUSTOMTOOLBARITEM_H

#include <QDebug>
#include <QObject>

class CustomToolBarItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ getText CONSTANT)
    Q_PROPERTY(QString icon READ getIcon CONSTANT)
public:
    CustomToolBarItem(QString name, QObject *parent): QObject(parent){text = name;}
    explicit CustomToolBarItem(QString name, QString action, QString ico, QObject *parent): QObject(parent){
        text = name;
        this->action = action;
        icon = ico;
    }
    explicit CustomToolBarItem(QString name, QString action, QObject *parent): QObject(parent){
        text = name;
        this->action = action;
    }

    QString getText(){return text;}
    QString getIcon(){return icon;}
    QString getAction(){
        return action;
    }

private:
    QString text;
    QString icon;
    QString action;
signals:
    void isClicked();
public slots:
    void clicked(){
        qDebug() << text + " is clicked";
        emit isClicked();
    }

signals:

public slots:
};

#endif // CUSTOMTOOLBARITEM_H


