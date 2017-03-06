#ifndef QWEBINSPECTOR_H
#define QWEBINSPECTOR_H

#include <QObject>

class QWebInspector : public QObject
{
    Q_OBJECT
public:
    explicit QWebInspector(QObject *parent = 0);

signals:

public slots:
};

#endif // QWEBINSPECTOR_H