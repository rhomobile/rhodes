#ifndef CUSTOMMENU_H
#define CUSTOMMENU_H

#include <QObject>
#include <QString>
#include <QtQuick>
#include <QDebug>

class CustomMenuItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ getText CONSTANT)
public:

    explicit CustomMenuItem(QString name, int item, QObject *parent): QObject(parent){
        text = name;
        this->item = item;
        qDebug() << "Creating menu item " + name;
    }
    CustomMenuItem(QString name, QObject *parent): QObject(parent){
        text = name;
        qDebug() << "Creating menu item " + name;
    }
    CustomMenuItem(QString name, QString action, QObject *parent): QObject(parent){
        text = name;
        this->action = action;
        qDebug() << "Creating menu item " + name;
    }
    QString getText(){return text;}
    int getItem() const{
        return item;
    }
    QString getAction() const{
        return action;
    }
private:
    QString text;
    int item;
    QString action;
signals:
    void isClicked();
public slots:
    void clicked(){
        qDebug() << text + " is clicked";
        emit isClicked();
    }

};

#endif // CUSTOMMENU_H


