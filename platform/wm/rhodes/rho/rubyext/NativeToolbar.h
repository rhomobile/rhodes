#pragma once

#include "common/IRhoThreadImpl.h"
#include "ruby/ext/rho/rhoruby.h"

class CNativeToolbar : public CWindowImpl<CNativeToolbar, CToolBarCtrl>
{
    DEFINE_LOGCLASS;

    //CImageList m_imageList;
    int        m_nHeight;
    rho::Vector<rho::String> m_arActions;
public:
    class CCreateTask: public rho::common::IRhoRunnable
    {
        rho_param *m_param;
    public:
        CCreateTask(rho_param *p) : m_param(rho_param_dup(p)){ }
        ~CCreateTask(){ rho_param_free(m_param); }
        virtual void runObject(){ CNativeToolbar::getInstance().createToolbar(m_param); }
    };
    class CRemoveTask: public rho::common::IRhoRunnable
    {
    public:
        virtual void runObject(){ CNativeToolbar::getInstance().removeToolbar(); }
    };

    CNativeToolbar(void);
    ~CNativeToolbar(void);

    static CNativeToolbar& getInstance();

	BEGIN_MSG_MAP(CNativeToolbar)
		//MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
        //MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()
	
    int getHeight(){ return m_nHeight;}

    void processCommand(int nItemPos);
private:

    void addToolbarButton(const char *label, const char *action, const char *icon, const char *colored_icon);
    void removeAllButtons();

    void createToolbar(rho_param *param);
    void removeToolbar();
};
