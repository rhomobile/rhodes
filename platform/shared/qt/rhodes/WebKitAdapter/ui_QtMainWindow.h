/********************************************************************************
** Form generated from reading UI file 'QtMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTMAINWINDOW_H
#define UI_QTMAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include "qwebview.h"
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QtNativeTabBar.h"

QT_BEGIN_NAMESPACE

class Ui_QtMainWindow
{
public:
    QAction *actionBack;
    QAction *actionRotateRight;
    QAction *actionRotateLeft;
    QAction *actionRotate180;
    QAction *actionAbout;
    QAction *actionExit;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QtNativeTabBar *tabBar;
    QWebView *webView;
    QMenuBar *menubar;
    QMenu *menuMain;
    QMenu *menuSimulate;
    QMenu *menuHelp;
    QToolBar *toolBar;
    QToolBar *toolBarRight;

    void setupUi(QMainWindow *QtMainWindow)
    {
        if (QtMainWindow->objectName().isEmpty())
            QtMainWindow->setObjectName(QStringLiteral("QtMainWindow"));
        QtMainWindow->resize(400, 600);
        QtMainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        actionBack = new QAction(QtMainWindow);
        actionBack->setObjectName(QStringLiteral("actionBack"));
        actionRotateRight = new QAction(QtMainWindow);
        actionRotateRight->setObjectName(QStringLiteral("actionRotateRight"));
        actionRotateLeft = new QAction(QtMainWindow);
        actionRotateLeft->setObjectName(QStringLiteral("actionRotateLeft"));
        actionRotate180 = new QAction(QtMainWindow);
        actionRotate180->setObjectName(QStringLiteral("actionRotate180"));
        actionAbout = new QAction(QtMainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionExit = new QAction(QtMainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(QtMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tabBar = new QtNativeTabBar(centralWidget);
        tabBar->setObjectName(QStringLiteral("tabBar"));
        tabBar->setVisible(false);

        verticalLayout->addWidget(tabBar);

        webView = new QWebView(centralWidget);
        webView->setObjectName(QStringLiteral("webView"));
        webView->setUrl(QUrl(QStringLiteral("about:blank")));

        verticalLayout->addWidget(webView);

        QtMainWindow->setCentralWidget(centralWidget);
        menubar = new QMenuBar(QtMainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 400, 21));
        menuMain = new QMenu(menubar);
        menuMain->setObjectName(QStringLiteral("menuMain"));
        menuSimulate = new QMenu(menubar);
        menuSimulate->setObjectName(QStringLiteral("menuSimulate"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        QtMainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(QtMainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        toolBar->setMovable(false);
        toolBar->setAllowedAreas(Qt::BottomToolBarArea);
        toolBar->setFloatable(false);
        QtMainWindow->addToolBar(Qt::BottomToolBarArea, toolBar);
        toolBarRight = new QToolBar(QtMainWindow);
        toolBarRight->setObjectName(QStringLiteral("toolBarRight"));
        toolBarRight->setLayoutDirection(Qt::RightToLeft);
        toolBarRight->setMovable(false);
        toolBarRight->setAllowedAreas(Qt::BottomToolBarArea);
        toolBarRight->setFloatable(false);
        QtMainWindow->addToolBar(Qt::BottomToolBarArea, toolBarRight);

        menubar->addAction(menuMain->menuAction());
        menubar->addAction(menuSimulate->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuMain->addAction(actionExit);
        menuSimulate->addAction(actionBack);
        menuSimulate->addSeparator();
        menuSimulate->addAction(actionRotateRight);
        menuSimulate->addAction(actionRotateLeft);
        menuSimulate->addAction(actionRotate180);
        menuHelp->addAction(actionAbout);

        retranslateUi(QtMainWindow);

        QMetaObject::connectSlotsByName(QtMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *QtMainWindow)
    {
        QtMainWindow->setWindowTitle(QApplication::translate("QtMainWindow", "MainWindow", Q_NULLPTR));
        actionBack->setText(QApplication::translate("QtMainWindow", "Back", Q_NULLPTR));
        actionRotateRight->setText(QApplication::translate("QtMainWindow", "Rotate 90\302\260 Clockwise", Q_NULLPTR));
        actionRotateLeft->setText(QApplication::translate("QtMainWindow", "Rotate 90\302\260 Counter-clockwise", Q_NULLPTR));
        actionRotate180->setText(QApplication::translate("QtMainWindow", "Rotate 180\302\260", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("QtMainWindow", "About", Q_NULLPTR));
        actionExit->setText(QApplication::translate("QtMainWindow", "Exit", Q_NULLPTR));
        menuMain->setTitle(QApplication::translate("QtMainWindow", "Main", Q_NULLPTR));
        menuSimulate->setTitle(QApplication::translate("QtMainWindow", "Simulate", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("QtMainWindow", "Help", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("QtMainWindow", "toolBar", Q_NULLPTR));
        toolBarRight->setWindowTitle(QApplication::translate("QtMainWindow", "toolBarRight", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QtMainWindow: public Ui_QtMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTMAINWINDOW_H
