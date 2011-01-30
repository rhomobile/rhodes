#pragma once

#include "common/IRhoThreadImpl.h"
#include "ruby/ext/rho/rhoruby.h"

class CNativeToolbar : public CWindowImpl<CNativeToolbar, CToolBarCtrl>
{
    DEFINE_LOGCLASS;

    static const int  MIN_TOOLBAR_HEIGHT = 60;
    static const int  MIN_TOOLBAR_IDENT = 12;

    int        m_nHeight;
    COLORREF   m_rgbBackColor, m_rgbMaskColor;
    CImageList m_listImages;

    struct CToolbarBtn
    {
        rho::StringW m_strLabelW;
        rho::String m_strAction;
        HBITMAP     m_hImage;
        CSize       m_sizeImage;
        int         m_nItemWidth;

        CToolbarBtn( const char *label, const char *action, const char *icon, int nItemWidth );
        ~CToolbarBtn();

        rho::String toString();
        rho::String getDefaultImagePath(const rho::String& strAction);
        bool isSeparator();
    };

    rho::VectorPtr<CToolbarBtn*> m_arButtons;
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
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
        //MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
    virtual void OnFinalMessage(HWND /*hWnd*/);

    int getHeight(){ return m_nHeight;}

    void processCommand(int nItemPos);
    bool isStarted();
private:

    void addToolbarButton(CToolbarBtn& oButton, int nPos);
    void removeAllButtons();
    CSize getMaxImageSize();
    void alignSeparatorWidth();

    void createToolbar(rho_param *param);
    void removeToolbar();
};
