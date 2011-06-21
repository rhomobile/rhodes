#ifndef NATIVETOOLBARIMPL_H
#define NATIVETOOLBARIMPL_H

#include "logging/RhoLog.h"
#include "common/IRhoThreadImpl.h"
#include "ruby/ext/rho/rhoruby.h"

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
    CNativeToolbar(void) {};
    ~CNativeToolbar(void) {};
    static CNativeToolbar& getInstance();
    int getHeight();
    bool isStarted();
private:
    void createToolbar(rho_param *param);
    void removeToolbar();
};

#endif // NATIVETOOLBARIMPL_H
