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

#include "common/IRhoThreadImpl.h"
#include "ruby/ext/rho/rhoruby.h"

#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlmisc.h>

class CNativeTabbar
{
    DEFINE_LOGCLASS;

public:
    static const int MIN_TABBAR_HEIGHT = 120;

public:
    class CCreateTask: public rho::common::IRhoRunnable
    {
		int m_bartype;
        rho_param *m_param;
    public:
        CCreateTask(int bar_type, rho_param *p) : m_bartype(bar_type), m_param(rho_param_dup(p)){ }
        ~CCreateTask(){ rho_param_free(m_param); }
        virtual void runObject(){
            CNativeTabbar::getInstance().createTabbar(m_bartype, m_param);
        }
    };
    class CRemoveTask: public rho::common::IRhoRunnable
    {
    public:
        virtual void runObject(){
            CNativeTabbar::getInstance().removeTabbar();
        }
    };
    class CSwitchTask: public rho::common::IRhoRunnable
    {
        int m_index;
    public:
        CSwitchTask(int index) : m_index(index){ }
        ~CSwitchTask(){ }
        virtual void runObject(){
            CNativeTabbar::getInstance().tabbarSwitch(m_index);
        }
    };
    class CBadgeTask: public rho::common::IRhoRunnable
    {
        int m_index;
		char* m_badge;
    public:
        CBadgeTask(int idx, char* badge) : m_index(idx), m_badge(badge) { }
        ~CBadgeTask(){ }
        virtual void runObject(){
            CNativeTabbar::getInstance().tabbarBadge(m_index, m_badge);
        }
    };

public:
    CNativeTabbar(void);
    ~CNativeTabbar(void);

    static CNativeTabbar& getInstance();

    virtual void OnFinalMessage(HWND /*hWnd*/);

    int getHeight();
    bool isStarted();

private:
    void createTabbar(int bar_type, rho_param *param);
    void removeTabbar();
    void tabbarSwitch(int index);
    void tabbarBadge(int index, char* badge);
};
