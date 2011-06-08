#ifndef QTWEBINSPECTOR_H
#define QTWEBINSPECTOR_H

#include <QWidget>
#include <QWebInspector>

namespace Ui {
    class QtWebInspector;
}

class QtWebInspector : public QWidget
{
    Q_OBJECT

public:
    explicit QtWebInspector(QWidget *parent = 0);
    ~QtWebInspector();
    QWebInspector* webInspector(void);
    void addWebInspector(QWebInspector* wi);
    void removeWebInspector(QWebInspector* wi);
    void showWebInspector(QWebInspector* wi);
    void hideWebInspector(QWebInspector* wi);
private:
    Ui::QtWebInspector *ui;
};

#endif // QTWEBINSPECTOR_H
