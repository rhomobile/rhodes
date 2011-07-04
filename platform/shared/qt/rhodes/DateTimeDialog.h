#ifndef DATETIMEDIALOG_H
#define DATETIMEDIALOG_H

#include "impl/DateTimePickerImpl.h"
#undef null
#include <QDialog>
#include <QDateTimeEdit>

namespace Ui {
    class DateTimeDialog;
}

class DateTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DateTimeDialog(CDateTimeMessage* msg, QWidget *parent = 0);
    ~DateTimeDialog();
    time_t getUnixTime(void);

private:
    Ui::DateTimeDialog *ui;
    QDateTimeEdit* m_dte;
    int m_type;
};

#endif // DATETIMEDIALOG_H
