#include "stdafx.h"

#include "NativeToolbar.h"
#include "common/rhoparams.h"
#include "MainWindow.h"

#define TOOLBAR_TYPE		0
#define TABBAR_TYPE			1
#define NOBAR_TYPE			2
#define VTABBAR_TYPE		3

extern CMainWindow& getAppWindow();
IMPLEMENT_LOGCLASS(CNativeToolbar,"NativeToolbar");

CNativeToolbar::CNativeToolbar(void)
{
}

CNativeToolbar::~CNativeToolbar(void)
{
}

/*static*/ CNativeToolbar& CNativeToolbar::getInstance()
{
    return getAppWindow().getToolbar();
}

void CNativeToolbar::removeAllButtons()
{
    int nCount = GetButtonCount();
    for( int i = 0; i < nCount; i++)
        DeleteButton(0);

    m_arLabels.removeAllElements();
    m_arActions.removeAllElements();
}

void CNativeToolbar::createToolbar(rho_param *p)
{
    if (!rho_rhodesapp_check_mode())
        return;

    int bar_type = TOOLBAR_TYPE;
	const char* background_color = NULL;
	
	rho_param *params = NULL;
    switch (p->type) 
    {
        case RHO_PARAM_ARRAY:
            params = p;
            break;
        case RHO_PARAM_HASH: 
            {
                for (int i = 0, lim = p->v.hash->size; i < lim; ++i) 
                {
                    const char *name = p->v.hash->name[i];
                    rho_param *value = p->v.hash->value[i];
                    
                    if (strcasecmp(name, "background_color") == 0) 
					    background_color = value->v.string;
    				
                    if (strcasecmp(name, "buttons") == 0 || strcasecmp(name, "tabs") == 0) 
                        params = value;
                }
            }
            break;
        default: {
            LOG(ERROR) + "Unexpected parameter type for create_nativebar, should be Array or Hash";
            return;
        }
    }
    
    if (!params) {
        LOG(ERROR) + "Wrong parameters for create_nativebar";
        return;
    }
    
	if (background_color) 
    {
		int c = atoi(background_color);

		int cR = (c & 0xFF0000) >> 16;
		int cG = (c & 0xFF00) >> 8;
		int cB = (c & 0xFF);

        //TODO: set back color
    }

    int size = params->v.array->size;
    if ( size == 0 )
    {
        removeToolbar();
        return;
    }

    m_nHeight = 60;
    if ( m_hWnd )
    {
        removeAllButtons();
        
    }else
    {
        RECT rcToolbar;
        rcToolbar.left = 0;
        rcToolbar.right = 0;
        rcToolbar.top = 0;
        rcToolbar.bottom = m_nHeight;
        Create(getAppWindow().m_hWnd, rcToolbar, NULL, WS_CHILD|CCS_NOPARENTALIGN|CCS_NORESIZE|CCS_NOMOVEY|CCS_BOTTOM|CCS_NODIVIDER |
            TBSTYLE_FLAT |TBSTYLE_LIST|TBSTYLE_AUTOSIZE  );

        SetButtonStructSize();
        SetButtonSize(0, m_nHeight-15);
        SetBitmapSize(0, m_nHeight-15);
    }

    for (int i = 0; i < size; ++i) 
    {
        rho_param *hash = params->v.array->value[i];
        if (hash->type != RHO_PARAM_HASH) {
            LOG(ERROR) + "Unexpected type of array item for create_nativebar, should be Hash";
            return;
        }
        
        const char *label = NULL;
        const char *action = NULL;
        const char *icon = NULL;
        const char *colored_icon = NULL;
		
        for (int j = 0, lim = hash->v.hash->size; j < lim; ++j) 
        {
            const char *name = hash->v.hash->name[j];
            rho_param *value = hash->v.hash->value[j];
            if (value->type != RHO_PARAM_STRING) {
                LOG(ERROR) + "Unexpected '" + name + "' type, should be String";
                return;
            }
            
            if (strcasecmp(name, "label") == 0)
                label = value->v.string;
            else if (strcasecmp(name, "action") == 0)
                action = value->v.string;
            else if (strcasecmp(name, "icon") == 0)
                icon = value->v.string;
            else if (strcasecmp(name, "colored_icon") == 0)
                colored_icon = value->v.string;
        }
        
        if (label == NULL && bar_type == TOOLBAR_TYPE)
            label = "";
        
        if ( label == NULL || action == NULL) {
            LOG(ERROR) + "Illegal argument for create_nativebar";
            return;
        }

        addToolbarButton(label, action, icon, colored_icon);
	}

    AutoSize();
    ShowWindow(SW_SHOW);

#if defined (OS_WINDOWS)
    RECT rcWnd;
    getAppWindow().GetWindowRect(&rcWnd);
    getAppWindow().SetWindowPos( 0, 0,0,rcWnd.right-rcWnd.left-1,rcWnd.bottom-rcWnd.top, SWP_NOMOVE|SWP_NOZORDER|SWP_FRAMECHANGED);
    getAppWindow().SetWindowPos( 0, 0,0,rcWnd.right-rcWnd.left,rcWnd.bottom-rcWnd.top, SWP_NOMOVE|SWP_NOZORDER|SWP_FRAMECHANGED);
#else
    getAppWindow().SetWindowPos( 0, 0,0,0,0, SWP_NOMOVE|SWP_NOZORDER|SWP_NOSIZE|SWP_FRAMECHANGED);
#endif
}

void CNativeToolbar::addToolbarButton(const char *label, const char *action, const char *icon, const char *colored_icon)
{
    LOG(INFO) + "addToolbarButton: " + label + ";" + action + ";" + (icon?icon:"") + ";" + (colored_icon?colored_icon:"");

    if ( action && strcasecmp( action, "separator" ) == 0 )
        return;

    TBBUTTON btn = {0};

    StringW wLabel;
    if ( label )
        convertToStringW( label, wLabel );
    m_arLabels.addElement(wLabel);
    btn.iString = (INT_PTR)m_arLabels.elementAt(m_arLabels.size()-1).c_str();

    btn.fsStyle = TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE;
    btn.fsState = TBSTATE_ENABLED;
    btn.idCommand = ID_CUSTOM_TOOLBAR_ITEM_FIRST+m_arActions.size();
    m_arActions.addElement(action);

    BOOL bRes = AddButton(&btn);

    if ( !bRes )
        LOG(ERROR) + "Error";
}

void CNativeToolbar::processCommand(int nItemPos)
{
    if ( nItemPos >= 0 && nItemPos < (int)m_arActions.size() )
        RHODESAPP().loadUrl(m_arActions.elementAt(nItemPos));
}

void CNativeToolbar::removeToolbar()
{
    if ( m_hWnd )
    {
        ShowWindow(SW_HIDE);
        m_nHeight = 0;

#if defined (OS_WINDOWS)
        RECT rcWnd;
        getAppWindow().GetWindowRect(&rcWnd);
        getAppWindow().SetWindowPos( 0, 0,0,rcWnd.right-rcWnd.left-1,rcWnd.bottom-rcWnd.top, SWP_NOMOVE|SWP_NOZORDER|SWP_FRAMECHANGED);
        getAppWindow().SetWindowPos( 0, 0,0,rcWnd.right-rcWnd.left,rcWnd.bottom-rcWnd.top, SWP_NOMOVE|SWP_NOZORDER|SWP_FRAMECHANGED);
#else
        getAppWindow().SetWindowPos( 0, 0,0,0,0, SWP_NOMOVE|SWP_NOZORDER|SWP_NOSIZE|SWP_FRAMECHANGED);
#endif
    }
}

extern "C"
{
void remove_native_toolbar();
void create_native_toolbar(int bar_type, rho_param *p) 
{
    if ( bar_type == NOBAR_TYPE )
        remove_native_toolbar();
    else if ( bar_type == TOOLBAR_TYPE )
    {
        getAppWindow().performOnUiThread(new CNativeToolbar::CCreateTask(p) );
    }else
    {
    	RAWLOGC_ERROR("NativeBar", "Only Toolbar control is supported.");
    }
}

void create_nativebar(int bar_type, rho_param *p) 
{
	RAWLOGC_INFO("NativeBar", "NativeBar.create() is DEPRECATED. Use Rho::NativeToolbar.create() or Rho::NativeTabbar.create().");
    create_native_toolbar(bar_type, p);
}

void remove_native_toolbar() 
{
    getAppWindow().performOnUiThread(new CNativeToolbar::CRemoveTask() );
}

void remove_nativebar() 
{
	RAWLOGC_INFO("NativeBar", "NativeBar.remove() is DEPRECATED API ! Please use Rho::NativeToolbar.remove() or Rho::NativeTabbar.remove().");
	remove_native_toolbar();
}

VALUE nativebar_started() 
{
    //TODO: nativebar_started
    return rho_ruby_create_boolean(0);
}

//Tabbar
void create_native_tabbar(int bar_type, rho_param *p) {
}

void remove_native_tabbar() {
}

void native_tabbar_switch_tab(int index) {
}

void native_tabbar_set_tab_badge(int index,char *val) {
}

void nativebar_set_tab_badge(int index,char* val)
{
}

void nativebar_switch_tab(int index) {
	//TODO: Implement me!
}

//NavBar - iphone only
void create_navbar(rho_param *p)
{
}

void remove_navbar()
{
}

VALUE navbar_started()
{
    return rho_ruby_create_boolean(0);
}

}