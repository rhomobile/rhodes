#ifndef ALERTDIALOG_H
#define ALERTDIALOG_H

#include "impl/DateTimePickerImpl.h"
#undef null
#include <QDialog>

namespace Ui {
    class AlertDialog;
}

class AlertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlertDialog(CDateTimeMessage* msg, QWidget *parent = 0);
    ~AlertDialog();

private:
    Ui::AlertDialog *ui;
};

#endif // ALERTDIALOG_H
