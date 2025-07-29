/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "DateTimeDialog.h"
#include <QDateTimeEdit>
#if QT_VERSION >= 0x060000
#include <QDateTime>
#endif

DateTimeDialog::DateTimeDialog(CDateTimeMessage* msg, QWidget *parent) : QDialog(parent){

    if (msg->m_title && (strlen(msg->m_title) > 0))
        this->setWindowTitle(QString(msg->m_title));

    QDateTimeEdit* m_dte;
    switch (m_type = msg->m_format) {
    case CDateTimeMessage::FORMAT_DATE:
        m_dte = new QDateEdit(this);
        if (msg->m_min_time)
#if QT_VERSION >= 0x060000
            m_dte->setMinimumDate(QDateTime::fromSecsSinceEpoch(qint64(msg->m_min_time)).date());
#else
            m_dte->setMinimumDate(QDateTime::fromTime_t(msg->m_min_time).date());
#endif
        if (msg->m_max_time)
#if QT_VERSION >= 0x060000
            m_dte->setMaximumDate(QDateTime::fromSecsSinceEpoch(qint64(msg->m_max_time)).date());
#else
            m_dte->setMaximumDate(QDateTime::fromTime_t(msg->m_max_time).date());
#endif
        if (msg->m_initialTime)
#if QT_VERSION >= 0x060000
            m_dte->setDate(QDateTime::fromSecsSinceEpoch(qint64(msg->m_initialTime)).date());
#else
            m_dte->setDate(QDateTime::fromTime_t(msg->m_initialTime).date());
#endif
        m_dte->setCalendarPopup(true);
        //m_dte->setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
        break;
    case CDateTimeMessage::FORMAT_TIME:
        m_dte = new QTimeEdit(this);
        if (msg->m_min_time)
#if QT_VERSION >= 0x060000
            m_dte->setMinimumTime(QDateTime::fromSecsSinceEpoch(qint64(msg->m_min_time)).time());
#else
            m_dte->setMinimumTime(QDateTime::fromTime_t(msg->m_min_time).time());
#endif
        if (msg->m_max_time)
#if QT_VERSION >= 0x060000
            m_dte->setMaximumTime(QDateTime::fromSecsSinceEpoch(qint64(msg->m_max_time)).time());
#else
            m_dte->setMaximumTime(QDateTime::fromTime_t(msg->m_max_time).time());
#endif
        if (msg->m_initialTime)
#if QT_VERSION >= 0x060000
            m_dte->setTime(QDateTime::fromSecsSinceEpoch(qint64(msg->m_initialTime)).time());
#else
            m_dte->setTime(QDateTime::fromTime_t(msg->m_initialTime).time());
#endif
        //m_dte->setDisplayFormat(QLocale::system().timeFormat(QLocale::ShortFormat));
        break;
    default: //case CDateTimeMessage::FORMAT_DATE_TIME:
        m_dte = new QDateTimeEdit(this);
        if (msg->m_min_time)
#if QT_VERSION >= 0x060000
            m_dte->setMinimumDateTime(QDateTime::fromSecsSinceEpoch(qint64(msg->m_min_time), Qt::LocalTime));
#else
            m_dte->setMinimumDateTime(QDateTime::fromTime_t(msg->m_min_time));
#endif
        if (msg->m_max_time)
#if QT_VERSION >= 0x060000
            m_dte->setMaximumDateTime(QDateTime::fromSecsSinceEpoch(qint64(msg->m_max_time), Qt::LocalTime));
#else
            m_dte->setMaximumDateTime(QDateTime::fromTime_t(msg->m_max_time));
#endif
        if (msg->m_initialTime)
#if QT_VERSION >= 0x060000
            m_dte->setDateTime(QDateTime::fromSecsSinceEpoch(qint64(msg->m_initialTime), Qt::LocalTime));
#else
            m_dte->setDateTime(QDateTime::fromTime_t(msg->m_initialTime));
#endif
        m_dte->setCalendarPopup(true);
        //m_dte->setDisplayFormat(QLocale::system().dateTimeFormat(QLocale::ShortFormat));
    }
    //m_dte->move(20, 20);
    QVBoxLayout * vblay = new QVBoxLayout(this);
    vblay->addWidget(m_dte);
    QDialogButtonBox * buttonBox = new QDialogButtonBox(this);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    mv_dte = m_dte;
}

DateTimeDialog::~DateTimeDialog()
{
    delete (QDateTimeEdit*)mv_dte;
}

time_t DateTimeDialog::getUnixTime(void)
{
    QDateTimeEdit* m_dte = (QDateTimeEdit*)mv_dte;
    switch (m_type) {
    case CDateTimeMessage::FORMAT_DATE:
#if QT_VERSION >= 0x060000
        return static_cast<long>(
            QDateTime(m_dte->date(), QTime(0, 0), Qt::LocalTime).toSecsSinceEpoch()
        );
#else
        return QDateTime(m_dte->date()).toTime_t();
#endif
    case CDateTimeMessage::FORMAT_TIME:
#if QT_VERSION >= 0x060000
        return static_cast<long>(m_dte->dateTime().toSecsSinceEpoch());
#else
        return QDateTime(QDateTime::fromTime_t(0).date(),m_dte->time()).toTime_t();
#endif
    default: //case CDateTimeMessage::FORMAT_DATE_TIME:
#if QT_VERSION >= 0x060000
        return static_cast<long>(m_dte->dateTime().toSecsSinceEpoch());
#else
        return m_dte->dateTime().toTime_t();
#endif
    }
}
