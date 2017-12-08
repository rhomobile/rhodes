#ifndef CUSTOMMENU_H
#define CUSTOMMENU_H

#include <QObject>
#include <QString>
#include <QtQuick>
#include <QDebug>

class CustomMenuItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ getText)
public:
    explicit CustomMenuItem(QString name, QObject *parent = 0): QObject(parent){text = name;}
    QString getText(){return text;}
private:
    QString text;
signals:
    void isClicked();
public slots:
    void clicked(){
        qDebug() << text + " is clicked";
        emit isClicked();
    }

};

#endif // CUSTOMMENU_H
