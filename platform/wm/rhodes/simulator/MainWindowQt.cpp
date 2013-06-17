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

#pragma warning(disable:4996)

#include "stdafx.h"
#include <sstream>
#include "MainWindowQt.h"
#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/StringConverter.h"
#include "camera/Camera.h"
#include "common/RhoFilePath.h"
#include "NativeToolbarQt.h"
#include "rubyext/NativeToolbarExt.h"
#include "AppManager.h"
#include "json/JSONIterator.h"
#undef null
#include <QString>
#include <QApplication>
#include <QtGui/QAction>
#include <QHash>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkProxyFactory>
#include "qt/rhodes/QtMainWindow.h"

IMPLEMENT_LOGCLASS(CMainWindow,"MainWindow");

#include "DateTimePicker.h"

extern "C" int rho_wmsys_has_touchscreen();

using namespace rho;
using namespace rho::common;
using namespace rho::json;

extern "C" void rho_geoimpl_turngpsoff();

int CMainWindow::m_screenWidth;
int CMainWindow::m_screenHeight;

bool CMainWindow::mainWindowClosed = false;
HINSTANCE CMainWindow::rhoApplicationHINSTANCE = 0;

CMainWindow::CMainWindow():
    m_started(true),
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

void CMainWindow::onWebViewUrlChanged(const ::std::string& url)
{
    rho::String sUrl = url;
    RHODESAPP().keepLastVisitedUrl(sUrl);
}

void CMainWindow::Navigate2(BSTR URL, int index) {
    String cleared_url = convertToStringA(OLE2CT(URL));
    if (!cleared_url.empty()) {
        StringW cw = convertToStringW(cleared_url);
        navigate(cw.c_str(), index);
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

void CMainWindow::RhoSetFullScreen(bool bFull, bool bDestroy /*= false*/)
{
    if (qtMainWindow)
        ((QtMainWindow*)qtMainWindow)->fullscreenCommand(bFull ? 1 : 0);
}

bool CMainWindow::getFullScreen()
{
    if (qtMainWindow)
        return ((QtMainWindow*)qtMainWindow)->getFullScreen();

    return false;
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
            menuAddAction((oItem.m_strLink == "close" ? "Exit" : oItem.m_strLabel.c_str()), i);
        }
    }
}

void CMainWindow::onCustomMenuItemCommand(int nItemPos)
{    
    if ( nItemPos < 0 || nItemPos >= (int)m_arAppMenuItems.size() )
        return;

    CAppMenuItem& oMenuItem = m_arAppMenuItems.elementAt(nItemPos);
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

void CMainWindow::navigate(const wchar_t* url, int index)
{
    LOG(INFO) + "navigate: '"+url+"'";
    ((QtMainWindow*)qtMainWindow)->navigate(QString::fromWCharArray(url), index);
}

void CMainWindow::setCallback(IMainWindowCallback* callback)
{
    ((QtMainWindow*)qtMainWindow)->setCallback(callback);
}

void* CMainWindow::init(IMainWindowCallback* callback, const wchar_t* title)
{
    int argc = 0;
    QCoreApplication::setOrganizationName("Rhomobile");
#ifndef RHODES_EMULATOR
	QCoreApplication::setApplicationName(RHODESAPP().getAppName().c_str());
#else
    QCoreApplication::setApplicationName("RhoSimulator");
#endif
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

void CMainWindow::GoBack(int index)
{
    LOG(INFO) + "back";
    ((QtMainWindow*)qtMainWindow)->GoBack(index);
}

void CMainWindow::GoForward(void)
{
    LOG(INFO) + "forward";
    ((QtMainWindow*)qtMainWindow)->GoForward();
}

void CMainWindow::Refresh(int index)
{
    LOG(INFO) + "refresh";
    ((QtMainWindow*)qtMainWindow)->Refresh(index);
}

bool CMainWindow::isStarted()
{
    return ((QtMainWindow*)qtMainWindow)->isStarted();
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

void CMainWindow::createToolbarEx( const rho::Vector<rho::String>& toolbarElements,  const rho::Hashtable<rho::String, rho::String>& toolBarProperties)
{
    if (!rho_rhodesapp_check_mode() || !rho_wmsys_has_touchscreen() )
        return;

    //int bar_type = TOOLBAR_TYPE;
	std::auto_ptr<QColor> m_rgbBackColor (NULL);
    std::auto_ptr<QColor> m_rgbMaskColor (NULL);
    int m_nHeight = CNativeToolbar::MIN_TOOLBAR_HEIGHT;

    for ( Hashtable<rho::String, rho::String>::const_iterator it = toolBarProperties.begin(); it != toolBarProperties.end(); ++it )
    {
        const char *name = (it->first).c_str();
        const char *value = (it->second).c_str();
        if (strcasecmp(name, "backgroundColor") == 0) 
            m_rgbBackColor.reset(new QColor(getColorFromString(value)));
        else if (strcasecmp(name, "maskColor") == 0) 
            m_rgbMaskColor.reset(new QColor(getColorFromString(value)));
        else if (strcasecmp(name, "viewHeight") == 0) 
            m_nHeight = atoi(value);
    }

    if ( toolbarElements.size() == 0 )
    {
        removeToolbar();
        return;
    }

    removeAllButtons();

    int nSeparators = 0;
    bool wasSeparator = false;
    for (int ipass=0; ipass < 2; ++ipass) 
    {
        for (int i = 0; i < (int)toolbarElements.size(); ++i) 
        {
            const char *label = NULL;
            const char *action = NULL;
            const char *icon = NULL;
            const char *colored_icon = NULL;
            int  nItemWidth = 0;

            CJSONEntry oEntry(toolbarElements[i].c_str());

            if ( oEntry.hasName("label") )
                label = oEntry.getString("label");
            if ( oEntry.hasName("action") )
                action = oEntry.getString("action");
            if ( oEntry.hasName("icon") )
                icon = oEntry.getString("icon");
            if ( oEntry.hasName("coloredIcon") )
                colored_icon = oEntry.getString("coloredIcon");
            if ( oEntry.hasName("width") )
                nItemWidth = oEntry.getInt("width");

            if (label == NULL)
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
                    {
#ifndef RHODES_EMULATOR
                        strImagePath = rho::common::CFilePath::join( RHODESAPP().getRhoRootPath(), "/apps" );
                        strImagePath = rho::common::CFilePath::join( strImagePath, icon );

#else
                        strImagePath = rho::common::CFilePath::join( RHODESAPP().getRhoRootPath(), icon );
#endif

                    }else {
#if defined(RHODES_EMULATOR)
#define RHODES_EMULATOR_PLATFORM_STR ".win32"
#elif defined(RHO_SYMBIAN)
#define RHODES_EMULATOR_PLATFORM_STR ".sym"
#else
#define RHODES_EMULATOR_PLATFORM_STR
#endif
                        if ( strcasecmp(action, "options")==0 )
                            strImagePath = "res/options_btn" RHODES_EMULATOR_PLATFORM_STR ".png";
                        else if ( strcasecmp(action, "home")==0 )
                            strImagePath = "res/home_btn" RHODES_EMULATOR_PLATFORM_STR ".png";
                        else if ( strcasecmp(action, "refresh")==0 )
                            strImagePath = "res/refresh_btn" RHODES_EMULATOR_PLATFORM_STR ".png";
                        else if ( strcasecmp(action, "back")==0 )
                            strImagePath = "res/back_btn" RHODES_EMULATOR_PLATFORM_STR ".png";
                        else if ( strcasecmp(action, "forward")==0 )
                            strImagePath = "res/forward_btn" RHODES_EMULATOR_PLATFORM_STR ".png";
                        else if ( strcasecmp(action, "minimize")==0 )
                            strImagePath = "res/minimize_btn" RHODES_EMULATOR_PLATFORM_STR ".png";

#undef RHODES_EMULATOR_PLATFORM_STR
#ifdef RHODES_EMULATOR
                        strImagePath = strImagePath.length() > 0 ? CFilePath::join( RHOSIMCONF().getRhoRuntimePath(), "lib/framework/" + strImagePath) : String();
#else
                        strImagePath = strImagePath.length() > 0 ? CFilePath::join( rho_native_reruntimepath() , "lib/" + strImagePath) : String();
#endif
                    }

                    ((QtMainWindow*)qtMainWindow)->toolbarAddAction(QIcon(QString(strImagePath.c_str())), QString(label), action, wasSeparator);
                }
            }
        }
    }

	((QtMainWindow*)qtMainWindow)->setToolbarStyle(false, (m_rgbBackColor.get()!=NULL ? m_rgbBackColor->name() : ""));
    ((QtMainWindow*)qtMainWindow)->toolbarShow();
    m_started = true;
}
/*
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
                    {
#ifndef RHODES_EMULATOR
                        strImagePath = rho::common::CFilePath::join( RHODESAPP().getRhoRootPath(), "/apps" );
                        strImagePath = rho::common::CFilePath::join( strImagePath, icon );

#else
                        strImagePath = rho::common::CFilePath::join( RHODESAPP().getRhoRootPath(), icon );
#endif

                    }else {
#if defined(RHODES_EMULATOR)
#define RHODES_EMULATOR_PLATFORM_STR ".win32"
#elif defined(RHO_SYMBIAN)
#define RHODES_EMULATOR_PLATFORM_STR ".sym"
#else
#define RHODES_EMULATOR_PLATFORM_STR
#endif
                        if ( strcasecmp(action, "options")==0 )
                            strImagePath = "res/options_btn" RHODES_EMULATOR_PLATFORM_STR ".png";
                        else if ( strcasecmp(action, "home")==0 )
                            strImagePath = "res/home_btn" RHODES_EMULATOR_PLATFORM_STR ".png";
                        else if ( strcasecmp(action, "refresh")==0 )
                            strImagePath = "res/refresh_btn" RHODES_EMULATOR_PLATFORM_STR ".png";
                        else if ( strcasecmp(action, "back")==0 )
                            strImagePath = "res/back_btn" RHODES_EMULATOR_PLATFORM_STR ".png";
                        else if ( strcasecmp(action, "forward")==0 )
                            strImagePath = "res/forward_btn" RHODES_EMULATOR_PLATFORM_STR ".png";
#undef RHODES_EMULATOR_PLATFORM_STR
#ifdef RHODES_EMULATOR
                        strImagePath = strImagePath.length() > 0 ? CFilePath::join( RHOSIMCONF().getRhoRuntimePath(), "lib/framework/" + strImagePath) : String();
#else
                        strImagePath = strImagePath.length() > 0 ? CFilePath::join( rho_native_reruntimepath() , "lib/" + strImagePath) : String();
#endif
                    }

                    ((QtMainWindow*)qtMainWindow)->toolbarAddAction(QIcon(QString(strImagePath.c_str())), QString(label), action, wasSeparator);
                }
            }
        }
    }
	((QtMainWindow*)qtMainWindow)->setToolbarStyle(false, (m_rgbBackColor.get()!=NULL ? m_rgbBackColor->name() : ""));
    ((QtMainWindow*)qtMainWindow)->toolbarShow();
    //removeTabbar();
    m_started = true;
}*/

bool charToBool(const char* str)
{
    return str && ((stricmp(str,"true")==0) || (stricmp(str,"yes")==0) || (atoi(str)==1));
}

void CMainWindow::createTabbarEx(const rho::Vector<rho::String>& tabbarElements, const rho::Hashtable<rho::String, rho::String>& tabBarProperties, rho::apiGenerator::CMethodResult& oResult)
{
    if (!rho_rhodesapp_check_mode() || !rho_wmsys_has_touchscreen() )
        return;

	std::auto_ptr<QColor> background_color (NULL);

    for ( Hashtable<rho::String, rho::String>::const_iterator it = tabBarProperties.begin(); it != tabBarProperties.end(); ++it )
    {
        const char *name = (it->first).c_str();
        const char *value = (it->second).c_str();
        if (strcasecmp(name, "backgroundColor") == 0) 
            background_color.reset(new QColor(getColorFromString(value)));
    }

    ((QtMainWindow*)qtMainWindow)->tabbarInitialize();
    int nStartTab = -1;
    for (int i = 0; i < (int)tabbarElements.size(); ++i) 
    {
        const char *label = NULL;
        const char *action = NULL;
        const char *icon = NULL;
        const char *reload = NULL;
        const char *colored_icon = NULL;
        
    	std::auto_ptr<QColor> selected_color (NULL);
        const char *disabled = NULL;
		std::auto_ptr<QColor> web_bkg_color (NULL);
        const char* use_current_view_for_tab = NULL;

        CJSONEntry oEntry(tabbarElements[i].c_str());

        if ( oEntry.hasName("label") )
            label = oEntry.getString("label");
        if ( oEntry.hasName("action") )
            action = oEntry.getString("action");
        if ( oEntry.hasName("icon") )
            icon = oEntry.getString("icon");
        if ( oEntry.hasName("coloredIcon") )
            colored_icon = oEntry.getString("coloredIcon");
        if ( oEntry.hasName("reload") )
            reload = oEntry.getString("reload");
        if ( oEntry.hasName("selectedColor") )
            selected_color.reset(new QColor(getColorFromString(oEntry.getString("selectedColor"))));
        if ( oEntry.hasName("disabled") )
            disabled = oEntry.getString("disabled");
        if ( oEntry.hasName("useCurrentViewForTab") )
        {
            use_current_view_for_tab = oEntry.getString("useCurrentViewForTab");
            if (strcasecmp(use_current_view_for_tab, "true") == 0) {
                action = "none";
            }
        }
        if (oEntry.hasName("backgroundColor")) 
            web_bkg_color.reset(new QColor(getColorFromString(oEntry.getString("backgroundColor"))));

        if (label == NULL)
            label = "";
        
        if ( label == NULL || action == NULL) {
            RAWLOG_ERROR("Illegal argument for create_nativebar");
            return;
        }

        QtMainWindow::QTabBarRuntimeParams tbrp;
        tbrp["label"] = QString(label);
        tbrp["action"] = QString(action);
        tbrp["reload"] = charToBool(reload);
        tbrp["use_current_view_for_tab"] = charToBool(use_current_view_for_tab);
        tbrp["background_color"] = background_color.get() != NULL ? background_color->name() : QString("");
		tbrp["selected_color"] = selected_color.get() != NULL ? selected_color->name() : QString("");
        String strIconPath = icon ? CFilePath::join( RHODESAPP().getAppRootPath(), icon) : String();

        ((QtMainWindow*)qtMainWindow)->tabbarAddTab(QString(label), icon ? strIconPath.c_str() : NULL, charToBool(disabled), web_bkg_color.get(), tbrp);

        if (m_strStartTabName.length()>0 && m_strStartTabName == label)
            nStartTab = i;
    }

    if (oResult.hasCallback())
        ((QtMainWindow*)qtMainWindow)->tabbarSetSwitchCallback(oResult);

    ((QtMainWindow*)qtMainWindow)->tabbarShow();

    if (m_strStartTabName.length()>0&&nStartTab>=0)
    {
        tabbarSwitch(nStartTab);
        m_strStartTabName = "";
    }

    m_started = true;
}

/*
void CMainWindow::createTabbar(int bar_type, rho_param *p)
{
    if (!rho_rhodesapp_check_mode() || !rho_wmsys_has_touchscreen() )
        return;

	std::auto_ptr<QColor> background_color (NULL);
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
                        background_color.reset(new QColor(getColorFromString(value->v.string)));
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
        RAWLOG_ERROR("Wrong parameters for create_tabbar");
        return;
    }
    
    ((QtMainWindow*)qtMainWindow)->tabbarInitialize();

    int size = params->v.array->size;

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
        
    	std::auto_ptr<QColor> selected_color (NULL);
        const char *disabled = NULL;
		std::auto_ptr<QColor> web_bkg_color (NULL);
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
                background_color.reset(new QColor(getColorFromString(value->v.string)));
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
                selected_color.reset(new QColor(getColorFromString(value->v.string)));
            }    
            else if (strcasecmp(name, "disabled") == 0)
                disabled = value->v.string;
            else if (strcasecmp(name, "web_bkg_color") == 0)
                web_bkg_color.reset(new QColor(getColorFromString(value->v.string)));
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
            QtMainWindow::QTabBarRuntimeParams tbrp;
            tbrp["label"] = QString(label);
            tbrp["action"] = QString(action);
            tbrp["reload"] = charToBool(reload);
            tbrp["use_current_view_for_tab"] = charToBool(use_current_view_for_tab);
            tbrp["background_color"] = background_color.get() != NULL ? background_color->name() : QString("");
			tbrp["selected_color"] = selected_color.get() != NULL ? selected_color->name() : QString("");
            tbrp["on_change_tab_callback"] = QString(on_change_tab_callback != NULL ? on_change_tab_callback : "");
            String strIconPath = icon ? CFilePath::join( RHODESAPP().getAppRootPath(), icon) : String();
            ((QtMainWindow*)qtMainWindow)->tabbarAddTab(QString(label), icon ? strIconPath.c_str() : NULL, charToBool(disabled), web_bkg_color.get(), tbrp);
        }
    }

    ((QtMainWindow*)qtMainWindow)->tabbarShow();

	//removeToolbar();
    m_started = true;
}*/

int CMainWindow::getTabbarHeight()
{
    return ((QtMainWindow*)qtMainWindow)->tabbarGetHeight();
}

void CMainWindow::removeTabbar()
{
    ((QtMainWindow*)qtMainWindow)->tabbarHide();
}

void CMainWindow::removeAllTabs(bool restore)
{
    ((QtMainWindow*)qtMainWindow)->tabbarRemoveAllTabs(restore);
}

void CMainWindow::tabbarSwitch(int index)
{
    ((QtMainWindow*)qtMainWindow)->tabbarSwitch(index);
}

void CMainWindow::tabbarBadge(int index, const char* badge)
{
    ((QtMainWindow*)qtMainWindow)->tabbarSetBadge(index, badge);
}

int CMainWindow::tabbarGetCurrent()
{
	return ((QtMainWindow*)qtMainWindow)->tabbarGetCurrent();
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

// Window frame
void CMainWindow::setFrame(int x, int y, int width, int height)
{
	((QtMainWindow*)qtMainWindow)->setFrame(x, y, width, height);
}

void CMainWindow::setPosition(int x, int y)
{
    ((QtMainWindow*)qtMainWindow)->setPosition(x, y);
}

void CMainWindow::setSize(int width, int height)
{
	((QtMainWindow*)qtMainWindow)->setSize(width, height);
}

void CMainWindow::lockSize(int locked)
{
	((QtMainWindow*)qtMainWindow)->lockSize(locked);
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

LRESULT CMainWindow::OnNavigateBackCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    GoBack((int)hWndCtl);
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

LRESULT CMainWindow::OnRefreshCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    Refresh((int)hWndCtl);
    return 0;
}

LRESULT CMainWindow::OnNavTimeout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    return 0;
}

LRESULT CMainWindow::OnNavigateCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    TNavigateData* nd = (TNavigateData*)hWndCtl;
    if (nd) {
        LPTSTR wcurl = (LPTSTR)(nd->url);
        if (wcurl) {
            Navigate2(wcurl, nd->index);
        }
        delete nd;
    }
    return 0;
}

LRESULT CMainWindow::OnExecuteJS(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    TNavigateData* nd = (TNavigateData*)hWndCtl;
    if (nd) {
        LPTSTR wcurl = (LPTSTR)(nd->url);
        if (wcurl) {

            StringW strUrlW;
            if(_memicmp(wcurl,L"JavaScript:",11*2) != 0)
                strUrlW = L"javascript:";

            strUrlW += wcurl;

            Navigate2((LPWSTR)strUrlW.c_str(), nd->index);
        }
        delete nd;
    }
    return 0;
}

LRESULT CMainWindow::OnFullscreenCommand (WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    LOG(INFO) + "OnFullscreenCommand";
    ((QtMainWindow*)qtMainWindow)->fullscreenCommand((int)hWndCtl);
    return 0;
};

LRESULT CMainWindow::OnSetCookieCommand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
    TCookieData* cd = (TCookieData*)hWndCtl;
    if (cd) {
        if (cd->url && cd->cookie) {
            ((QtMainWindow*)qtMainWindow)->setCookie(cd->url, cd->cookie);
        }
        delete cd;
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
			CAlertDialog::Params::CAlertButton* cbtn = &(params->m_buttons[nBtn]);
			rho::Hashtable<rho::String, rho::String> mapRes;
			std::ostringstream sBtnIndex;
			sBtnIndex << nBtn;
			mapRes["button_index"] = sBtnIndex.str();
			mapRes["button_id"] = cbtn->m_strID;
			mapRes["button_title"] = cbtn->m_strCaption;
			params->m_callback_ex.set(mapRes);
			//RHODESAPP().callPopupCallback(params->m_callback, params->m_buttons[nBtn].m_strID, params->m_buttons[nBtn].m_strCaption);
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

LRESULT CMainWindow::OnLicenseWarning (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    ::MessageBoxW( m_hWnd, L"Please provide RhoElements license key.", L"Motorola License", MB_ICONERROR | MB_OK);

    return 0;
}

class CRhodesProxyFactory: public QNetworkProxyFactory
{
private:
	static CRhodesProxyFactory* _instance;
	QNetworkProxy _proxy;
	QList<QNetworkProxy>* _proxyList;
	QList<QNetworkProxy>* _defaultProxyList;
	bool _useProxy;
	CRhodesProxyFactory(): QNetworkProxyFactory(), _proxyList(NULL), _useProxy(false)
	{
		_defaultProxyList = new QList<QNetworkProxy>();
		*_defaultProxyList << QNetworkProxy(QNetworkProxy::DefaultProxy);
	}
public:
	static CRhodesProxyFactory* getInstance()
	{
		if (_instance == NULL) {
			_instance = new CRhodesProxyFactory();
			QNetworkProxyFactory::setUseSystemConfiguration(false);
			QNetworkProxyFactory::setApplicationProxyFactory(_instance);
		}
		return _instance;
	}
	virtual QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery& query = QNetworkProxyQuery())
	{
		if ((query.peerHostName().compare("localhost", Qt::CaseInsensitive) == 0) ||
			(query.peerHostName().compare("127.0.0.1") == 0))
		{
			return *_defaultProxyList;
		} else if (_useProxy) {
			return *_proxyList;
		} else {
			return QNetworkProxyFactory::systemProxyForQuery(query);
		}
	}
	void setProxy(QNetworkProxy& proxy)
	{
		_useProxy = true;
		_proxy = proxy;
		if (_proxyList)
			delete _proxyList;
		_proxyList = new QList<QNetworkProxy>();
		*_proxyList << QNetworkProxy(_proxy);
	}
	void unsetProxy()
	{
		_useProxy = false;
		if (_proxyList)
			delete _proxyList;
		_proxyList = NULL;
	}
};
CRhodesProxyFactory* CRhodesProxyFactory::_instance = NULL;

void CMainWindow::setProxy(const rho::String& host, const rho::String& port, const rho::String& login, const rho::String& password)
{
	if (host.length()) {
		QNetworkProxy proxy;
		proxy.setType(QNetworkProxy::HttpCachingProxy);
		proxy.setHostName(host.c_str());
		if (port.length())
			proxy.setPort(atoi(port.c_str()));
		if (login.length())
			proxy.setUser(login.c_str());
		if (password.length())
			proxy.setPassword(password.c_str());
		CRhodesProxyFactory::getInstance()->setProxy(proxy);
	}
}

void CMainWindow::setProxy()
{
	CRhodesProxyFactory::getInstance()->unsetProxy();
}
