#pragma once

#include "common/IRhoThreadImpl.h"
#include "ruby/ext/rho/rhoruby.h"

#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlmisc.h>

class CNativeToolbar
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
            CNativeToolbar::getInstance().createToolbar(m_param);
        }
    };
    class CRemoveTask: public rho::common::IRhoRunnable
    {
    public:
        virtual void runObject(){
            CNativeToolbar::getInstance().removeToolbar(); 
        }
    };

public:
    CNativeToolbar(void);
    ~CNativeToolbar(void);

    static CNativeToolbar& getInstance();

    virtual void OnFinalMessage(HWND /*hWnd*/);

    int getHeight();
    bool isStarted();

private:
    void createToolbar(rho_param *param);
    void removeToolbar();
};
