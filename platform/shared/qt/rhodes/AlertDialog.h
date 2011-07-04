#ifndef ALERTDIALOG_H
#define ALERTDIALOG_H

#include "impl/DateTimePickerImpl.h"
#undef null
#include <QDialog>
#include <QDateTimeEdit>

namespace Ui {
    class AlertDialog;
}

class AlertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlertDialog(CDateTimeMessage* msg, QWidget *parent = 0);
    ~AlertDialog();
    time_t getUnixTime(void);

private:
    Ui::AlertDialog *ui;
    QDateTimeEdit* m_dte;
    int m_type;
};

#endif // ALERTDIALOG_H
