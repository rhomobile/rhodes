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

#include "IToolbar.h"

class CNativeToolbarQt : public IToolbar
{
    DEFINE_LOGCLASS;

public:
    static const int  MIN_TOOLBAR_HEIGHT = 60;

public:
    class CCreateTask: public rho::common::IRhoRunnable
    {
        rho_param *m_param;
    public:
        CCreateTask(rho_param *p) : m_param(rho_param_dup(p)){ }
        ~CCreateTask(){ rho_param_free(m_param); }
        virtual void runObject(){
            CNativeToolbarQt::getInstance().createToolbar(m_param);
        }
    };

    class CRemoveTask: public rho::common::IRhoRunnable
    {
    public:
        virtual void runObject(){
            CNativeToolbarQt::getInstance().removeToolbar(); 
        }
    };

public:
    CNativeToolbarQt(void);
    ~CNativeToolbarQt(void);

    static CNativeToolbarQt& getInstance();

    virtual void OnFinalMessage(HWND /*hWnd*/);

    int getHeight();
    bool isStarted();

private:
    void createToolbar(rho_param *param);
    void removeToolbar();
};
