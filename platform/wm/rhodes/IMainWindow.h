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

#pragma once

#include "qt/rhodes/MainWindowCallback.h"

static UINT WM_TAKEPICTURE             = ::RegisterWindowMessage(L"RHODES_WM_TAKEPICTURE");
static UINT WM_SELECTPICTURE           = ::RegisterWindowMessage(L"RHODES_WM_SELECTPICTURE");
//static UINT WM_TAKESIGNATURE           = ::RegisterWindowMessage(L"RHODES_WM_TAKESIGNATURE");
static UINT WM_CONNECTIONSNETWORKCOUNT = ::RegisterWindowMessage(L"RHODES_WM_CONNECTIONSNETWORKCOUNT");
static UINT WM_CONNECTIONSNETWORKCELL  = ::RegisterWindowMessage(L"RHODES_WM_CONNECTIONSNETWORKCELL");
static UINT WM_ALERT_SHOW_POPUP        = ::RegisterWindowMessage(L"RHODES_WM_ALERT_SHOW_POPUP");
static UINT WM_ALERT_HIDE_POPUP        = ::RegisterWindowMessage(L"RHODES_WM_ALERT_HIDE_POPUP");
static UINT WM_DATETIME_PICKER         = ::RegisterWindowMessage(L"RHODES_WM_DATETIME_PICKER");
static UINT WM_BLUETOOTH_DISCOVER      = ::RegisterWindowMessage(L"RHODES_WM_BLUETOOTH_DISCOVER");
static UINT WM_BLUETOOTH_DISCOVERED    = ::RegisterWindowMessage(L"RHODES_WM_BLUETOOTH_DISCOVERED");
static UINT WM_BLUETOOTH_CALLBACK	   = ::RegisterWindowMessage(L"RHODES_WM_BLUETOOTH_CALLBACK");
static UINT WM_EXECUTE_COMMAND		   = ::RegisterWindowMessage(L"RHODES_WM_EXECUTE_COMMAND");
static UINT WM_EXECUTE_RUNNABLE		   = ::RegisterWindowMessage(L"RHODES_WM_EXECUTE_RUNNABLE");
static UINT WM_WINDOW_MINIMIZE 		   = ::RegisterWindowMessage(L"RHODES_WM_MINIMIZE");
static UINT WM_SHOW_LICENSE_WARNING	   = ::RegisterWindowMessage(L"RHOELEMENTS_WM_LICENSE_WARNING");


typedef struct _TCookieData {
    char* url;
    char* cookie;
} TCookieData;

namespace rho
{

class ITabbar;
class IToolbar;

enum EMainWindowType
{
    eQtWindow,
    eNativeWindow
};

class IMainWindow
{
public:
    virtual bool Initialize(const wchar_t* title, DWORD dwStyle) = 0;
    virtual HWND getWebViewHWND(int tabIdx) = 0;
    virtual HWND GetMainWindowHWND() = 0;
    virtual void UpdateWindow(int showCmd) = 0;
    virtual void initBrowserWindow() = 0;
    virtual void removeAllButtons() = 0;

    virtual IToolbar* getToolbar() = 0;
    virtual ITabbar*  getTabbar() = 0;
    virtual int       tabbarGetCurrent() = 0;

    virtual void setProxy() = 0;
    virtual void setProxy(const rho::String& host, const rho::String& port, const rho::String& login, const rho::String& password) = 0;

    virtual void performOnUiThread(rho::common::IRhoRunnable* pTask) = 0;
    virtual void Navigate2(BSTR URL, int index) = 0;
    virtual void DestroyUi() = 0;
    virtual void MessageLoop() = 0;
    virtual bool getFullScreen() = 0;

    virtual void windowSetFrame(int x, int y, int w, int h) = 0;
    virtual void windowSetPosition(int x, int y) = 0;
    virtual void windowSetSize(int width, int  height) = 0;
    virtual void windowLockSize(int x) = 0;
};

}
