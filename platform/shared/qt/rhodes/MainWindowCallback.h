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

#ifndef MAINWINDOWCALLBACK_H
#define MAINWINDOWCALLBACK_H

#include <string>

typedef struct _TNavigateData {
    int index;
    wchar_t* url;
} TNavigateData;

class IMainWindowCallback
{
public:
    virtual void updateSizeProperties(int width, int height) = 0;
    virtual void onActivate(int active) = 0;
    virtual void logEvent(const ::std::string& message) = 0;
    virtual void createCustomMenu(void) = 0;
    virtual void onCustomMenuItemCommand(int nItemPos) = 0;
    virtual void onWindowClose(void) = 0;
    virtual void onWebViewUrlChanged(const ::std::string& url) = 0;
protected:
    virtual ~IMainWindowCallback() {};
};

#endif // MAINWINDOWCALLBACK_H
