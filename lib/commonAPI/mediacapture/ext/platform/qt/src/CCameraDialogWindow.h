#ifndef CCAMERADIALOGWINDOW_H
#define CCAMERADIALOGWINDOW_H

#include <QDialog>
#include <QCamera>
#include <QVBoxLayout>
#include <QDebug>
#include <QVideoWidget>
#include <QEventLoop>
#include <QTimer>

class CCameraDialogWindow : public QDialog
{
    Q_OBJECT
public:
    static CCameraDialogWindow *showInstace(QCamera *camera);
    static void hideInstace(QCamera *camera);
    void closeDialog();
    ~CCameraDialogWindow();
private:

    QCamera * cameraObject;
    QVideoWidget * videoWidget;
    explicit CCameraDialogWindow(QCamera *camera, QWidget *parent = 0);
    static QHash<QCamera *, CCameraDialogWindow *> dialogsKeeper;
signals:

public slots:
    void buttonReject();
};

#endif // CCAMERADIALOGWINDOW_H
