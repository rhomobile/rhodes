#include "AlertDialog.h"
#include "ui_AlertDialog.h"

AlertDialog::AlertDialog(CDateTimeMessage* msg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertDialog)
{
    ui->setupUi(this);

    if (msg->m_title && (strlen(msg->m_title) > 0))
        this->setWindowTitle(QString(msg->m_title));

    switch (m_type = msg->m_format) {
    case CDateTimeMessage::FORMAT_DATE:
        m_dte = new QDateEdit(this);
        if (msg->m_min_time)
            m_dte->setMinimumDate(QDateTime::fromTime_t(msg->m_min_time).date());
        if (msg->m_max_time)
            m_dte->setMaximumDate(QDateTime::fromTime_t(msg->m_max_time).date());
        if (msg->m_initialTime)
            m_dte->setDate(QDateTime::fromTime_t(msg->m_initialTime).date());
        break;
    case CDateTimeMessage::FORMAT_TIME:
        m_dte = new QTimeEdit(this);
        if (msg->m_min_time)
            m_dte->setMinimumTime(QDateTime::fromTime_t(msg->m_min_time).time());
        if (msg->m_max_time)
            m_dte->setMaximumTime(QDateTime::fromTime_t(msg->m_max_time).time());
        if (msg->m_initialTime)
            m_dte->setTime(QDateTime::fromTime_t(msg->m_initialTime).time());
        break;
    default: //case CDateTimeMessage::FORMAT_DATE_TIME:
        m_dte = new QDateTimeEdit(this);
        if (msg->m_min_time)
            m_dte->setMinimumDateTime(QDateTime::fromTime_t(msg->m_min_time));
        if (msg->m_max_time)
            m_dte->setMaximumDateTime(QDateTime::fromTime_t(msg->m_max_time));
        if (msg->m_initialTime)
            m_dte->setDateTime(QDateTime::fromTime_t(msg->m_initialTime));
    }

    m_dte->move(20, 20);
}

AlertDialog::~AlertDialog()
{
    delete m_dte;
    delete ui;
}

time_t AlertDialog::getUnixTime(void)
{
    switch (m_type) {
    case CDateTimeMessage::FORMAT_DATE:
        return QDateTime(m_dte->date()).toTime_t();
    case CDateTimeMessage::FORMAT_TIME:
        return QDateTime(QDateTime::fromTime_t(0).date(),m_dte->time()).toTime_t();
    default: //case CDateTimeMessage::FORMAT_DATE_TIME:
        return m_dte->dateTime().toTime_t();
    }
}
