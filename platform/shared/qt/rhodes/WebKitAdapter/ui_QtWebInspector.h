/********************************************************************************
** Form generated from reading UI file 'QtWebInspector.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTWEBINSPECTOR_H
#define UI_QTWEBINSPECTOR_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include "qwebinspector.h"
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtWebInspector
{
public:
    QVBoxLayout *verticalLayout;
    QWebInspector *webInspector;

    void setupUi(QWidget *QtWebInspector)
    {
        if (QtWebInspector->objectName().isEmpty())
            QtWebInspector->setObjectName(QStringLiteral("QtWebInspector"));
        QtWebInspector->resize(850, 600);
        QtWebInspector->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout = new QVBoxLayout(QtWebInspector);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        webInspector = new QWebInspector(QtWebInspector);
        webInspector->setObjectName(QStringLiteral("webInspector"));

        verticalLayout->addWidget(webInspector);


        retranslateUi(QtWebInspector);

        QMetaObject::connectSlotsByName(QtWebInspector);
    } // setupUi

    void retranslateUi(QWidget *QtWebInspector)
    {
        QtWebInspector->setWindowTitle(QApplication::translate("QtWebInspector", "Web Inspector", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QtWebInspector: public Ui_QtWebInspector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTWEBINSPECTOR_H
