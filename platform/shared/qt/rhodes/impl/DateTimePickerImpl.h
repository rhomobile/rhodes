#ifndef DATETIMEPICKERIMPL_H
#define DATETIMEPICKERIMPL_H

#include "common/RhoPort.h"

class CDateTimeMessage
{
public:
    enum CDateTimePickerType
    {
        FORMAT_DATE_TIME = 0,
        FORMAT_DATE,
        FORMAT_TIME
    };

    CDateTimeMessage (const char* callback, const char* title, long initial_time, int format, const char* data, long min_time, long max_time)
    {
        m_callback    = strdup (callback);
        m_title       = strdup (title);
        m_initialTime = initial_time;
        m_format      = format;
        m_data        = strdup (data);
        m_min_time    = min_time;
        m_max_time    = max_time;
    }

    ~CDateTimeMessage ()
    {
        free (m_callback);
        free (m_title);
        free (m_data);
    }

    char *m_callback;
    char *m_title;
    char *m_data;
    long  m_initialTime;
    int   m_format;
    long  m_min_time;
    long  m_max_time;
};


#endif // DATETIMEPICKERIMPL_H
