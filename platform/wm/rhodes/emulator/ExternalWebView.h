#ifndef EXTERNALWEBVIEW_H
#define EXTERNALWEBVIEW_H

#include <QWidget>
#include <QUrl>

namespace Ui {
    class ExternalWebView;
}

class ExternalWebView : public QWidget
{
    Q_OBJECT

public:
    explicit ExternalWebView(QWidget *parent = 0);
    ~ExternalWebView();
    void navigate(QUrl url);
private:
    Ui::ExternalWebView *ui;
};

#endif // EXTERNALWEBVIEW_H
