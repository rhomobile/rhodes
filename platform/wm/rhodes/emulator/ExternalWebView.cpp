#include "ExternalWebView.h"
#include "ui_ExternalWebView.h"

ExternalWebView::ExternalWebView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExternalWebView)
{
    ui->setupUi(this);
    this->ui->webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    this->move(0,0);
}

ExternalWebView::~ExternalWebView()
{
}

void ExternalWebView::navigate(QUrl url)
{
    ui->webView->setUrl(url);
}
