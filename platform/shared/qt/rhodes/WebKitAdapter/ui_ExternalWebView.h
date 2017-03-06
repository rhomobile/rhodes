/********************************************************************************
** Form generated from reading UI file 'ExternalWebView.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXTERNALWEBVIEW_H
#define UI_EXTERNALWEBVIEW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include "qwebview.h"
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExternalWebView
{
public:
    QVBoxLayout *verticalLayout;
    QWebView *webView;

    void setupUi(QWidget *ExternalWebView)
    {
        if (ExternalWebView->objectName().isEmpty())
            ExternalWebView->setObjectName(QStringLiteral("ExternalWebView"));
        ExternalWebView->resize(800, 600);
        ExternalWebView->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout = new QVBoxLayout(ExternalWebView);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        webView = new QWebView(ExternalWebView);
        webView->setObjectName(QStringLiteral("webView"));
        webView->setUrl(QUrl(QStringLiteral("about:blank")));

        verticalLayout->addWidget(webView);


        retranslateUi(ExternalWebView);

        QMetaObject::connectSlotsByName(ExternalWebView);
    } // setupUi

    void retranslateUi(QWidget *ExternalWebView)
    {
        ExternalWebView->setWindowTitle(QApplication::translate("ExternalWebView", "External Browser", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ExternalWebView: public Ui_ExternalWebView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXTERNALWEBVIEW_H
