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

#include "logging/RhoLog.h"
#include "MainWindowImpl.h"
#include "DateTimePickerImpl.h"

static char ourTitle[1024];

extern "C" {

void  choose_datetime_with_range(char* callback, char* title,
                                 long initial_time, int format, char* data,
                                 long min_time, long max_time)
{
    RAWLOGC_INFO3("DateTimePicker", "%s callback = %s title = %s", __FUNCTION__, callback, title);

    if (title != NULL)
        strcpy(ourTitle, title);
    else
        strcpy(ourTitle, "");
    CDateTimeMessage *msg = new CDateTimeMessage(callback, ourTitle, initial_time, format, data, min_time, max_time);
    CMainWindow::getInstance()->dateTimePicker(msg);
}


void  choose_datetime(char* callback, char* title,
                      long initial_time, int format, char* data)
{
    choose_datetime_with_range(callback, title, initial_time, format, data, 0, 0);
}

void set_change_value_callback_datetime(char* callback)
{
    //TODO: implement set_change_value_callback_datetime
}

} //extern "C"
