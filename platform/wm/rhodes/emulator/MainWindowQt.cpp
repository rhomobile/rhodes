// MainWindow.cpp: Defines main window for this application.

#pragma warning(disable:4996)

#include "stdafx.h"
#include "MainWindowQt.h"
#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/StringConverter.h"
#include "camera/Camera.h"
#include "common/RhoFilePath.h"
#include "NativeToolbarQt.h"
#include "rho/rubyext/NativeToolbarExt.h"
#include "AppManager.h"
#undef null
#include <QString>
#include <QApplication>
#include <QtGui/QAction>
#include "QtMainWindow.h"

IMPLEMENT_LOGCLASS(CMainWindow,"MainWindow");

#include "DateTimePicker.h"

extern "C" int rho_wmsys_has_touchscreen();

using namespace rho;
using namespace rho::common;

extern "C" void rho_geoimpl_turngpsoff();

int CMainWindow::m_screenWidth;
int CMainWindow::m_screenHeight;

bool CMainWindow::mainWindowClosed = false;

CMainWindow::CMainWindow():
    qtApplication(NULL),
    qtMainWindow(NULL)
{
}

CMainWindow::~CMainWindow()
{
    if (qtMainWindow) delete (QtMainWindow*)qtMainWindow;
    if (qtApplication) delete (QApplication*)qtApplication;
}

void CMainWindow::updateSizeProperties(int width, int height)
{
    m_screenWidth = width;
    m_screenHeight = height;
    LOGCONF().setLogView(&m_logView);
}

void CMainWindow::logEvent(const ::std::string& message)
{
    LOG(INFO) + message;
}

void CMainWindow::Navigate2(BSTR URL) {
    String cleared_url = convertToStringA(OLE2CT(URL));
    if (!cleared_url.empty()) {
        StringW cw = convertToStringW(cleared_url);
        navigate(cw.c_str());
    }
}

HWND CMainWindow::Initialize(const wchar_t* title)
{
    HWND hWnd = (HWND)init(this, title);
    SubclassWindow(hWnd);

    //rho_rhodesapp_callAppActiveCallback(1);
    rho_rhodesapp_callUiCreatedCallback();
    return hWnd;
}

HWND CMainWindow::getWebViewHWND(int index)
{
    // TODO
    return 0;
}

void CMainWindow::MessageLoop(void)
{
    messageLoop();
}

void CMainWindow::createCustomMenu(void)
{
    RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);
    //createCustomMenu();
#ifdef ENABLE_DYNAMIC_RHOBUNDLE
    String strIndexPage = CFilePath::join(RHODESAPP().getStartUrl(),"index"RHO_ERB_EXT);
    if ( RHODESAPP().getCurrentUrl().compare(RHODESAPP().getStartUrl()) == 0 ||
         RHODESAPP().getCurrentUrl().compare(strIndexPage) == 0 )
        m_arAppMenuItems.addElement(CAppMenuItem("Reload RhoBundle","reload_rhobundle"));
#endif //ENABLE_DYNAMIC_RHOBUNDLE

    //update UI with custom menu items
    menuClear();
    for ( unsigned int i = 0; i < m_arAppMenuItems.size(); i++)
    {
        CAppMenuItem& oItem = m_arAppMenuItems.elementAt(i);
        if (oItem.m_eType == CAppMenuItem::emtSeparator) 
            menuAddSeparator();
        else
        {
            menuAddAction((oItem.m_eType == CAppMenuItem::emtClose ? "Exit" : oItem.m_strLabel.c_str()), i);
        }
    }
}

void CMainWindow::onCustomMenuItemCommand(int nItemPos)
{    
    if ( nItemPos < 0 || nItemPos >= (int)m_arAppMenuItems.size() )
        return;

    CAppMenuItem& oMenuItem = m_arAppMenuItems.elementAt(nItemPos);
    if ( oMenuItem.m_eType == CAppMenuItem::emtUrl )
    {
        if ( oMenuItem.m_strLink == "reload_rhobundle" )
        {
        #ifdef ENABLE_DYNAMIC_RHOBUNDLE
            if ( RHODESAPP().getRhobundleReloadUrl().length()>0 ) {
                CAppManager::ReloadRhoBundle(m_hWnd,RHODESAPP().getRhobundleReloadUrl().c_str(), NULL);
            } else {
                MessageBox(_T("Path to the bundle is not defined."),_T("Information"), MB_OK | MB_ICONINFORMATION );
            }
        #endif
            return;
        }
    }

    oMenuItem.processCommand();
}

void CMainWindow::DestroyUi(void)
{
    rho_rhodesapp_callUiDestroyedCallback();
}

void CMainWindow::onWindowClose(void)
{
     mainWindowClosed = true;
}


// **************************************************************************
//
// proxy methods:
//
// **************************************************************************

void CMainWindow::navigate(const wchar_t* url)
{
    LOG(INFO) + "navigate: '"+url+"'";
    ((QtMainWindow*)qtMainWindow)->navigate(QString::fromWCharArray(url));
}

void CMainWindow::setCallback(IMainWindowCallback* callback)
{
    ((QtMainWindow*)qtMainWindow)->setCallback(callback);
}

void* CMainWindow::init(IMainWindowCallback* callback, const wchar_t* title)
{
    int argc = 0;
	QCoreApplication::setOrganizationName("Rhomobile");
	QCoreApplication::setApplicationName("RhoSimulator");
    qtApplication = (void*)new QApplication(argc, 0);
    qtMainWindow = (void*)new QtMainWindow();
    ((QtMainWindow*)qtMainWindow)->setWindowTitle(QString::fromWCharArray(title));
    ((QtMainWindow*)qtMainWindow)->setCallback(callback);
    ((QtMainWindow*)qtMainWindow)->show();

    return (void*)((QtMainWindow*)qtMainWindow)->winId();
}

void CMainWindow::messageLoop(void)
{
    qApp->exec();
}

void CMainWindow::GoBack(void)
{
    LOG(INFO) + "back";
    ((QtMainWindow*)qtMainWindow)->GoBack();
}

void CMainWindow::GoForward(void)
{
    LOG(INFO) + "forward";
    ((QtMainWindow*)qtMainWindow)->GoForward();
}

void CMainWindow::Refresh(void)
{
    LOG(INFO) + "refresh";
    ((QtMainWindow*)qtMainWindow)->Refresh();
}

bool CMainWindow::isStarted()
{
    return true;
}

int CMainWindow::getToolbarHeight()
{
    return ((QtMainWindow*)qtMainWindow)->toolbarGetHeight();
}

void CMainWindow::removeToolbar()
{
    ((QtMainWindow*)qtMainWindow)->toolbarHide();
}

void CMainWindow::removeAllButtons()
{
    ((QtMainWindow*)qtMainWindow)->toolbarRemoveAllButtons();
}

static QColor getColorFromString(const char* szColor)
{
    if ( !szColor || !*szColor )
        return QColor(0, 0, 0);

    int c = atoi(szColor);

    int cR = (c & 0xFF0000) >> 16;
    int cG = (c & 0xFF00) >> 8;
    int cB = (c & 0xFF);

    return QColor(cR, cG, cB);
}

void CMainWindow::createToolbar(rho_param *p)
{
    if (!rho_rhodesapp_check_mode() || !rho_wmsys_has_touchscreen() )
        return;

    int bar_type = TOOLBAR_TYPE;
	std::auto_ptr<QColor> m_rgbBackColor (NULL);
    std::auto_ptr<QColor> m_rgbMaskColor (NULL);
    int m_nHeight = CNativeToolbar::MIN_TOOLBAR_HEIGHT;

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
                        m_rgbBackColor.reset(new QColor(getColorFromString(value->v.string)));
                    else if (strcasecmp(name, "mask_color") == 0) 
                        m_rgbMaskColor.reset(new QColor(getColorFromString(value->v.string)));
                    else if (strcasecmp(name, "view_height") == 0) 
                        m_nHeight = atoi(value->v.string);
                    else if (strcasecmp(name, "buttons") == 0 || strcasecmp(name, "tabs") == 0) 
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

    int size = params->v.array->size;
    if ( size == 0 )
    {
        removeToolbar();
        return;
    }

    removeAllButtons();

    int nSeparators = 0;
    bool wasSeparator = false;
    for (int ipass=0; ipass < 2; ++ipass) {
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
            int  nItemWidth = 0;

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
                else if (strcasecmp(name, "width") == 0)
                    nItemWidth = atoi(value->v.string);
            }
            
            if (label == NULL && bar_type == TOOLBAR_TYPE)
                label = "";
            
            if ( label == NULL || action == NULL) {
                LOG(ERROR) + "Illegal argument for create_nativebar";
                return;
            }
            if ( strcasecmp(action, "forward") == 0 && rho_conf_getBool("jqtouch_mode") )
                continue;

            if (!action) action = "";

            if (ipass==0) {
                if (strcasecmp(action, "separator")==0)
                    ++nSeparators;
            } else {
                LOG(INFO) + "addToolbarButton: Label: '"+label+"';Action: '"+action+"'";
                if (strcasecmp(action, "separator")==0) {
                    if (nSeparators!=1)
                        ((QtMainWindow*)qtMainWindow)->toolbarAddSeparator();
                    else
                        wasSeparator = true;
                } else {
                    String strImagePath;
                    if ( icon && *icon )
                        strImagePath = rho::common::CFilePath::join( RHODESAPP().getRhoRootPath(), icon );
                    else {
                        if ( strcasecmp(action, "options")==0 )
                            strImagePath = "res/options_btn.wm.png";
                        else if ( strcasecmp(action, "home")==0 )
                            strImagePath = "res/home_btn.wm.png";
                        else if ( strcasecmp(action, "refresh")==0 )
                            strImagePath = "res/refresh_btn.wm.png";
                        else if ( strcasecmp(action, "back")==0 )
                            strImagePath = "res/back_btn.wm.png";
                        else if ( strcasecmp(action, "forward")==0 )
                            strImagePath = "res/forward_btn.wm.png";
                        strImagePath = strImagePath.length() > 0 ? CFilePath::join( RHODESAPP().getRhodesPath(), "lib/framework/" + strImagePath) : String();
                    }

                    ((QtMainWindow*)qtMainWindow)->toolbarAddAction(QIcon(QString(strImagePath.c_str())), QString(label), action, wasSeparator);
                }
            }
        }
    }
	((QtMainWindow*)qtMainWindow)->setToolbarStyle(false, (m_rgbBackColor.get()!=NULL ? m_rgbBackColor->name() : ""));
    ((QtMainWindow*)qtMainWindow)->toolbarShow();
}

void CMainWindow::createTabbar(int bar_type, rho_param *p)
{
    // TODO: implement tabbar creation

    if (!rho_rhodesapp_check_mode() || !rho_wmsys_has_touchscreen() )
        return;

    const char* background_color = NULL;
    const char* background_color_enable = NULL;
    const char* on_change_tab_callback = NULL;
    
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
                    if (strcasecmp(name, "background_color") == 0) {
                        background_color = value->v.string;
                        background_color_enable = "true";
                    } else if (strcasecmp(name, "on_change_tab_callback") == 0) {
                        on_change_tab_callback = value->v.string;
                    } else if (strcasecmp(name, "buttons") == 0 || strcasecmp(name, "tabs") == 0) {
                        params = value;
                    }
                }
            }
            break;
        default: {
            RAWLOG_ERROR("Unexpected parameter type for create_nativebar, should be Array or Hash");
            return;
        }
    }
    
    if (!params) {
        RAWLOG_ERROR("Wrong parameters for create_nativebar");
        return;
    }
    
    int size = params->v.array->size;

    //NSMutableDictionary* main_properties = [NSMutableDictionary dictionaryWithCapacity:2];
    //NSMutableDictionary* properties = [NSMutableDictionary dictionaryWithCapacity:1];
    //[main_properties setObject:properties forKey:NATIVE_BAR_PROPERTIES];
    //NSMutableArray* items = [NSMutableArray arrayWithCapacity:size];
    //[main_properties setObject:items forKey:NATIVE_BAR_ITEMS];
    
    for (int i = 0; i < size; ++i) {
        rho_param *hash = params->v.array->value[i];
        if (hash->type != RHO_PARAM_HASH) {
            RAWLOG_ERROR("Unexpected type of array item for create_nativebar, should be Hash");
            return;
        }
        
        const char *label = NULL;
        const char *action = NULL;
        const char *icon = NULL;
        const char *reload = NULL;
        const char *colored_icon = NULL;
        
        const char *selected_color = NULL;
        const char *disabled = NULL;
        const char* web_bkg_color = NULL;
        const char* use_current_view_for_tab = NULL;
        
        bool skip_item = false;
        for (int j = 0, lim = hash->v.hash->size; j < lim; ++j) {
            const char *name = hash->v.hash->name[j];
            rho_param *value = hash->v.hash->value[j];
            if (value->type != RHO_PARAM_STRING) {
                RAWLOGC_ERROR("Unexpected '%s' type, should be String", name);
                return;
            }
            if (strcasecmp(name, "background_color") == 0) {
                background_color = value->v.string;
                skip_item = true;
            }
            
            if (strcasecmp(name, "label") == 0)
                label = value->v.string;
            else if (strcasecmp(name, "action") == 0)
                action = value->v.string;
            else if (strcasecmp(name, "icon") == 0)
                icon = value->v.string;
            else if (strcasecmp(name, "reload") == 0)
                reload = value->v.string;
            else if (strcasecmp(name, "colored_icon") == 0)
                colored_icon = value->v.string;
            else if (strcasecmp(name, "selected_color") == 0){
                selected_color = value->v.string;
            }    
            else if (strcasecmp(name, "disabled") == 0)
                disabled = value->v.string;
            else if (strcasecmp(name, "web_bkg_color") == 0)
                web_bkg_color = value->v.string;
            else if (strcasecmp(name, "use_current_view_for_tab") == 0) {
                use_current_view_for_tab = value->v.string;
                if (strcasecmp(use_current_view_for_tab, "true") == 0) {
                    action = "none";
                }
            }
        }
        
        if (label == NULL && bar_type == TOOLBAR_TYPE)
            label = "";
        
        if ((label == NULL || (action == NULL)) && (!skip_item)) {
            RAWLOG_ERROR("Illegal argument for create_nativebar");
            return;
        }
        if (!skip_item) {
            //NSMutableDictionary* item = [NSMutableDictionary dictionaryWithCapacity:10];    
            //[item setObject:[NSString stringWithUTF8String:label] forKey:NATIVE_BAR_ITEM_LABEL];
            //[item setObject:[NSString stringWithUTF8String:action] forKey:NATIVE_BAR_ITEM_ACTION];
            //[item setObject:[NSString stringWithUTF8String:(icon ? icon : "")] forKey:NATIVE_BAR_ITEM_ICON];
            //[item setObject:[NSString stringWithUTF8String:(reload ? reload : "false")] forKey:NATIVE_BAR_ITEM_RELOAD];
            //[item setObject:[NSString stringWithUTF8String:(colored_icon ? colored_icon : "false")] forKey:NATIVE_BAR_ITEM_COLORED_ICON];
            if (selected_color != NULL) {
                //[item setObject:[NSString stringWithUTF8String:selected_color] forKey:NATIVE_BAR_ITEM_SELECTED_COLOR];
            }
            //[item setObject:[NSString stringWithUTF8String:(disabled ? disabled : "false")] forKey:NATIVE_BAR_ITEM_DISABLED];
            if (web_bkg_color != NULL) {
                //[item setObject:[NSString stringWithUTF8String:web_bkg_color] forKey:NATIVE_BAR_ITEM_WEB_BACKGROUND_COLOR];
            }
            //[item setObject:[NSString stringWithUTF8String:(use_current_view_for_tab ? use_current_view_for_tab : "false")] forKey:NATIVE_BAR_ITEM_USE_CURRENT_VIEW_FOR_TAB];
            //[items addObject:item];
        }
        
    }
    if (background_color != NULL) {
        //[properties setObject:[NSString stringWithUTF8String:background_color] forKey:NATIVE_BAR_BACKGOUND_COLOR];    
    }
    if (on_change_tab_callback != NULL) {
        //[properties setObject:[NSString stringWithUTF8String:on_change_tab_callback] forKey:NATIVE_BAR_ON_CHANGE_TAB_CALLBACK];    
    }
    //id runnable = [RhoNativeBarCreateTask class];
    //id arg1 = [NSValue valueWithBytes:&bar_type objCType:@encode(int)];
    //[Rhodes performOnUiThread:runnable arg:arg1 arg:main_properties wait:NO];
}

int CMainWindow::getTabbarHeight()
{
    return ((QtMainWindow*)qtMainWindow)->tabbarGetHeight();
}

void CMainWindow::removeTabbar()
{
    ((QtMainWindow*)qtMainWindow)->tabbarHide();
}

void CMainWindow::removeAllTabs()
{
    ((QtMainWindow*)qtMainWindow)->tabbarRemoveAllTabs();
}

void CMainWindow::tabbarSwitch(int index)
{
    // TODO: Implement!

    if (!rho_rhodesapp_check_mode())
        return;
    //id runnable = [RhoNativeBarSwitchTabTask class];
    //id arg = [NSValue valueWithBytes:&index objCType:@encode(int)];
    //[Rhodes performOnUiThread:runnable arg:arg wait:NO];
}

void CMainWindow::tabbarBadge(int index, char* badge)
{
    // TODO: Implement!

	//RAWLOG_INFO2("set_tab_badge called: %d : %s",index,val);
    //id runnable = [RhoNativeBarSetTabBadgeTask class];
    //id arg1 = [NSValue valueWithBytes:&index objCType:@encode(int)];
    //id arg2 = [NSValue valueWithBytes:&val objCType:@encode(char*)];
    //[Rhodes performOnUiThread:runnable arg:arg1 arg:arg2 wait:YES];
    //RAWLOG_INFO("set_tab_badge done");
}

int CMainWindow::tabbarGetCurrent()
{
    // TODO: Implement!

    //Rhodes *r = [Rhodes sharedInstance];
    //return [[r mainView] activeTab];
	//TODO: Implement me!
	return 0;
}

// Menu
void CMainWindow::menuClear()
{
    ((QtMainWindow*)qtMainWindow)->menuClear();
}

void CMainWindow::menuAddSeparator()
{
    ((QtMainWindow*)qtMainWindow)->menuAddSeparator();
}

void CMainWindow::menuAddAction(const char* label, int item)
{
    ((QtMainWindow*)qtMainWindow)->menuAddAction(QString(label), item);
}


// **************************************************************************
//
// WM_xxx handlers
//
// **************************************************************************

void CMainWindow::performOnUiThread(rho::common::IRhoRunnable* pTask)
{
    PostMessage(WM_EXECUTE_RUNNABLE, 0, (LPARAM)pTask);
}
LRESULT CMainWindow::OnExecuteRunnable(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) 
{
    rho::common::IRhoRunnable* pTask = (rho::common::IRhoRunnable*)lParam;
    if (pTask)
    {
        pTask->runObject();
        delete pTask;
    }
    return 0;
}

LRESULT CMainWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    if(m_logView.IsWindow()) {
        m_logView.DestroyWindow();
    }
    LOGCONF().setLogView(NULL);

    if (m_alertDialog && m_alertDialog->IsWindow() )
        m_alertDialog->DestroyWindow();
    if (m_SyncStatusDlg && m_SyncStatusDlg->IsWindow() )
        m_SyncStatusDlg->DestroyWindow();

    PostQuitMessage(0);

    bHandled = FALSE; // Allow ATL's default processing (e.g. NULLing m_hWnd)
    return 0;
}

void CMainWindow::onActivate(int active)
{
    rho_rhodesapp_callAppActiveCallback(active);
    if (!active)
        rho_geoimpl_turngpsoff();
}


// **************************************************************************
//
// WM_COMMAND handlers
//
// **************************************************************************

LRESULT CMainWindow::OnExitCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    SendMessage(WM_CLOSE);
    return 0;
}

LRESULT CMainWindow::OnNavigateBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    GoBack();
    return 0;
}

LRESULT CMainWindow::OnNavigateForwardCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    GoForward();
    return 0;
}

LRESULT CMainWindow::OnBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    RHODESAPP().navigateBack();
    return 0;
}

LRESULT CMainWindow::OnLogCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if ( !m_logView.IsWindow() ) {
        LoadLibrary(_T("riched20.dll"));
        m_logView.Create(NULL);
    }
    m_logView.ShowWindow(SW_SHOWNORMAL);
    return 0;
}

LRESULT CMainWindow::OnRefreshCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    Refresh();
    return 0;
}

LRESULT CMainWindow::OnNavigateCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    LPTSTR wcurl = (LPTSTR)hWndCtl;
    if (wcurl) {
        Navigate2(wcurl);
        free(wcurl);
    }
    return 0;
}

LRESULT CMainWindow::OnTakePicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) 
{
    TCHAR image_uri[MAX_PATH];
    HRESULT status;
    //TODO: show browse file dialog
    wsprintf( image_uri, L"%s", L"dashboard.PNG");
    status = S_OK;

    RHODESAPP().callCameraCallback( (const char*)lParam, rho::common::convertToStringA(image_uri),
        (status!= S_OK && status != S_FALSE ? "Error" : ""), status == S_FALSE);

    free ((void *)lParam);
    return 0;
}

LRESULT CMainWindow::OnSelectPicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) 
{
    TCHAR image_uri[MAX_PATH];
    HRESULT status = S_OK;
    Camera camera;
    status = camera.selectPicture(this->m_hWnd,image_uri);

    RHODESAPP().callCameraCallback( (const char*)lParam, rho::common::convertToStringA(image_uri),
        (status!= S_OK && status != S_FALSE ? "Error" : ""), status == S_FALSE);
    
    free ((void *)lParam);
    
    return 0;
}

LRESULT CMainWindow::OnAlertShowPopup (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    /* TODO */
    StringW strAppName = RHODESAPP().getAppNameW();
    CAlertDialog::Params *params = (CAlertDialog::Params *)lParam;

    if (params->m_dlgType == CAlertDialog::Params::DLG_STATUS) 
    {
        if (m_SyncStatusDlg == NULL) 
            m_SyncStatusDlg = new CSyncStatusDlg();
        m_SyncStatusDlg->setStatusText(convertToStringW(params->m_message).c_str());
        m_SyncStatusDlg->setTitle( convertToStringW(params->m_title).c_str() );
        if ( !m_SyncStatusDlg->m_hWnd )
            m_SyncStatusDlg->Create(m_hWnd, 0);
        else
        {
            m_SyncStatusDlg->ShowWindow(SW_SHOW);
            m_SyncStatusDlg->BringWindowToTop();
        }
    } else if (params->m_dlgType == CAlertDialog::Params::DLG_DEFAULT) {
        MessageBox(convertToStringW(params->m_message).c_str(), strAppName.c_str(), MB_ICONWARNING | MB_OK);
    } else if (params->m_dlgType == CAlertDialog::Params::DLG_CUSTOM) 
    {
        if ( params->m_buttons.size() == 1 && strcasecmp(params->m_buttons[0].m_strCaption.c_str(), "ok") == 0)
            MessageBox(convertToStringW(params->m_message).c_str(), convertToStringW(params->m_title).c_str(), MB_ICONWARNING | MB_OK);
        else if (params->m_buttons.size() == 2 && strcasecmp(params->m_buttons[0].m_strCaption.c_str(), "ok") == 0 &&
            strcasecmp(params->m_buttons[1].m_strCaption.c_str(), "cancel") == 0)
        {
            int nRes = MessageBox(convertToStringW(params->m_message).c_str(), convertToStringW(params->m_title).c_str(), 
                    MB_ICONWARNING | MB_OKCANCEL);
            int nBtn = nRes == IDCANCEL ? 1 : 0;
            RHODESAPP().callPopupCallback(params->m_callback, params->m_buttons[nBtn].m_strID, params->m_buttons[nBtn].m_strCaption);
        }
        else
        {
            if (m_alertDialog == NULL) 
            {
                m_alertDialog = new CAlertDialog(params);
                m_alertDialog->DoModal();
                delete m_alertDialog;
                m_alertDialog = NULL;
            } else {
                LOG(WARNING) + "Trying to show alert dialog while it exists.";
            }
        }
    }

    delete params;
    /*  */
    return 0;
}

LRESULT CMainWindow::OnAlertHidePopup (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    /* TODO */
    if (m_alertDialog != NULL) {
        m_alertDialog->EndDialog(0);
        m_alertDialog = NULL;
    }
    /*  */
    return 0;
}

LRESULT CMainWindow::OnExecuteCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
    RhoNativeViewRunnable* command = (RhoNativeViewRunnable*)wParam;
    if (command != NULL) {
        command->run();
    }
    return 0;
}    

LRESULT CMainWindow::OnDateTimePicker (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    CDateTimeMessage *msg = (CDateTimeMessage *)lParam;
    int retCode    = -1;
    time_t ret_time = 0;

    if (msg->m_format == CDateTimeMessage::FORMAT_TIME) {
        CTimePickerDialog timeDialog(msg);
        retCode = timeDialog.DoModal(m_hWnd);
        ret_time = timeDialog.GetTime();
    } else {
        CDateTimePickerDialog dateTimeDialog(msg);
        retCode = dateTimeDialog.DoModal(m_hWnd);
        ret_time = dateTimeDialog.GetTime();
    }

    rho_rhodesapp_callDateTimeCallback( msg->m_callback, 
                                        retCode == IDOK ? (long )ret_time : 0,
                                        msg->m_data,
                                        retCode == IDOK ? 0 : 1);
    delete msg;

    return 0;
}
