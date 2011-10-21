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
#include "ui_DateTimeDialog.h"

DateTimeDialog::DateTimeDialog(CDateTimeMessage* msg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DateTimeDialog)
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
        m_dte->setCalendarPopup(true);
        //m_dte->setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
        break;
    case CDateTimeMessage::FORMAT_TIME:
        m_dte = new QTimeEdit(this);
        if (msg->m_min_time)
            m_dte->setMinimumTime(QDateTime::fromTime_t(msg->m_min_time).time());
        if (msg->m_max_time)
            m_dte->setMaximumTime(QDateTime::fromTime_t(msg->m_max_time).time());
        if (msg->m_initialTime)
            m_dte->setTime(QDateTime::fromTime_t(msg->m_initialTime).time());
        //m_dte->setDisplayFormat(QLocale::system().timeFormat(QLocale::ShortFormat));
        break;
    default: //case CDateTimeMessage::FORMAT_DATE_TIME:
        m_dte = new QDateTimeEdit(this);
        if (msg->m_min_time)
            m_dte->setMinimumDateTime(QDateTime::fromTime_t(msg->m_min_time));
        if (msg->m_max_time)
            m_dte->setMaximumDateTime(QDateTime::fromTime_t(msg->m_max_time));
        if (msg->m_initialTime)
            m_dte->setDateTime(QDateTime::fromTime_t(msg->m_initialTime));
        m_dte->setCalendarPopup(true);
        //m_dte->setDisplayFormat(QLocale::system().dateTimeFormat(QLocale::ShortFormat));
    }
    m_dte->move(20, 20);
}

DateTimeDialog::~DateTimeDialog()
{
    delete m_dte;
    delete ui;
}

time_t DateTimeDialog::getUnixTime(void)
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
