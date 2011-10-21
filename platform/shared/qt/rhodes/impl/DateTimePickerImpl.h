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
