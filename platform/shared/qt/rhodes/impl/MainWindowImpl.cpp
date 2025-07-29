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

#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/StringConverter.h"
#include "common/RhoFilePath.h"
#include "rubyext/NativeToolbarExt.h"
#include "json/JSONIterator.h"
#include "MainWindowImpl.h"
//#undef null
#ifndef RHODES_VERSION_LIBRARY
#include <QString>
#ifndef OS_SAILFISH
#if QT_VERSION >= 0x060000
#include <QAction>
#elif QT_VERSION >= 0x050000 && QT_VERSION < 0x060000
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif
#endif
#include <QHash>
#include "../mainwindowinterface.h"


IMPLEMENT_LOGCLASS(CMainWindow,"MainWindow");
#endif

using namespace rho;
using namespace rho::common;
using namespace rho::json;

extern "C" void rho_geoimpl_turngpsoff();

#ifndef RHODES_VERSION_LIBRARY
int CMainWindow::m_screenWidth;
int CMainWindow::m_screenHeight;

bool CMainWindow::mainWindowClosed = false;
#endif

CMainWindow::CMainWindow()
#ifndef RHODES_VERSION_LIBRARY
    : QObject(), m_started(true), qtMainWindow(NULL)
#endif
{
#ifndef RHODES_VERSION_LIBRARY
    //int argc = 0;
    QCoreApplication::setOrganizationName("Rhomobile");
#ifndef RHODES_EMULATOR
    QCoreApplication::setApplicationName(RHODESAPP().getAppName().c_str());
#else
    QCoreApplication::setApplicationName("RhoSimulator");
#endif
#endif //RHODES_VERSION_LIBRARY
    //qtApplication = (void*)new QApplication(argc, 0);
}

CMainWindow::~CMainWindow()
{
#ifndef RHODES_VERSION_LIBRARY
    if (qtMainWindow) delete (QtMainWindow*)qtMainWindow;
    //if (qtApplication) delete (QApplication*)qtApplication;
#endif
}

CMainWindow* CMainWindow::getInstance(void)
{
    static CMainWindow* instance = new CMainWindow();
    return instance;
}

void CMainWindow::updateSizeProperties(int width, int height)
{
#ifndef RHODES_VERSION_LIBRARY
    m_screenWidth = width;
    m_screenHeight = height;
#endif
}

void CMainWindow::logEvent(const ::std::string& message)
{
    LOG(INFO) + message;
}

void CMainWindow::onWebViewUrlChanged(const ::std::string& url)
{
    RHODESAPP().keepLastVisitedUrl(url);
}

#ifndef RHODES_VERSION_LIBRARY
bool CMainWindow::Initialize(const wchar_t* title)
{
    bool ok = init(this, title);
#ifndef OS_SAILFISH
    rho_rhodesapp_callUiCreatedCallback();
#endif
    return ok;
}
#endif

void CMainWindow::createCustomMenu(void)
{
#ifndef RHODES_VERSION_LIBRARY
    RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);
#ifdef ENABLE_DYNAMIC_RHOBUNDLE
    String strIndexPage = CFilePath::join(RHODESAPP().getStartUrl(),"index"RHO_ERB_EXT);
    if ( RHODESAPP().getCurrentUrl().compare(RHODESAPP().getStartUrl()) == 0 ||
         RHODESAPP().getCurrentUrl().compare(strIndexPage) == 0 )
        m_arAppMenuItems.addElement(CAppMenuItem("Reload RhoBundle","reload_rhobundle"));
#endif //ENABLE_DYNAMIC_RHOBUNDLE
    //update UI with custom menu items
    menuClear();
    for ( unsigned int i = 0; i < m_arAppMenuItems.size(); i++)  {
        CAppMenuItem& oItem = m_arAppMenuItems.elementAt(i);
        if (oItem.m_eType == CAppMenuItem::emtSeparator) { menuAddSeparator();}
        else {
#if defined(OS_MACOSX)
            menuAddAction((oItem.m_strLink == "close" ? "Close" : oItem.m_strLabel.c_str()), i, oItem.m_isEnable);
#else
            menuAddAction((oItem.m_strLink == "close" ? "Exit" : oItem.m_strLabel.c_str()), i, oItem.m_isEnable);
#endif
        }
    }
#endif//RHODES_VERSION_LIBRARY
}

void CMainWindow::onCustomMenuItemCommand(int nItemPos)
{    
#ifndef RHODES_VERSION_LIBRARY
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
#endif
}

void CMainWindow::onWindowClose(void)
{
#ifndef RHODES_VERSION_LIBRARY
    mainWindowClosed = true;
#endif
}

#ifndef RHODES_VERSION_LIBRARY
void CMainWindow::minimizeWindow(void)
{
#ifndef OS_SAILFISH //TODO: FIX
    ((QtMainWindow*)qtMainWindow)->showMinimized();
#endif
}

void CMainWindow::restoreWindow(void)
{
#ifndef OS_SAILFISH //TODO: FIX
    ((QtMainWindow*)qtMainWindow)->showNormal();
#endif
}

void CMainWindow::unsetProxy()
{
    ((QtMainWindow*)qtMainWindow)->unsetProxy();
}

void CMainWindow::setProxy(const char* host, const char* port, const char* login, const char* password)
{
    ((QtMainWindow*)qtMainWindow)->setProxy(QString(host ? host : ""), QString(port ? port : ""), QString(login ? login : ""), QString(password ? password : ""));
}

void CMainWindow::DestroyUi(void)
{
    rho_rhodesapp_callUiDestroyedCallback();
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

bool CMainWindow::init(IMainWindowCallback* callback, const wchar_t* title)
{
    qtMainWindow = (void*)new QtMainWindow();
#ifndef OS_SAILFISH //TODO: FIX
    ((QtMainWindow*)qtMainWindow)->setWindowTitle(QString::fromWCharArray(title));
#endif
    ((QtMainWindow*)qtMainWindow)->setCallback(callback);
#ifndef OS_SAILFISH //TODO: FIX
    ((QtMainWindow*)qtMainWindow)->show();
#endif

    QObject::connect(this, SIGNAL(doExitCommand(void)),
        ((QtMainWindow*)qtMainWindow), SLOT(exitCommand(void)) );
    QObject::connect(this, SIGNAL(doNavigateBackCommand(void)),
        ((QtMainWindow*)qtMainWindow), SLOT(navigateBackCommand(void)) );
    QObject::connect(this, SIGNAL(doNavigateForwardCommand(void)),
        ((QtMainWindow*)qtMainWindow), SLOT(navigateForwardCommand(void)) );
    QObject::connect(this, SIGNAL(doWebviewNavigateBackCommand(int)),
        ((QtMainWindow*)qtMainWindow), SLOT(webviewNavigateBackCommand(int)) );
    QObject::connect(this, SIGNAL(doLogCommand(void)),
        ((QtMainWindow*)qtMainWindow), SLOT(logCommand(void)) );
    QObject::connect(this, SIGNAL(doRefreshCommand(int)),
        ((QtMainWindow*)qtMainWindow), SLOT(refreshCommand(int)) );
    QObject::connect(this, SIGNAL(doNavigateCommand(TNavigateData*)),
        ((QtMainWindow*)qtMainWindow), SLOT(navigateCommand(TNavigateData*)) );
    QObject::connect(this, SIGNAL(doExecuteJavaScriptCommand(TNavigateData*)),
        ((QtMainWindow*)qtMainWindow), SLOT(executeJavaScriptCommand(TNavigateData*)) );
    QObject::connect(this, SIGNAL(doTakePicture(char*)),
        ((QtMainWindow*)qtMainWindow), SLOT(takePicture(char*)) );
    QObject::connect(this, SIGNAL(doSelectPicture(char*)),
        ((QtMainWindow*)qtMainWindow), SLOT(selectPicture(char*)) );
    QObject::connect(this, SIGNAL(doAlertShowPopup(CAlertParams*)),
        ((QtMainWindow*)qtMainWindow), SLOT(alertShowPopup(CAlertParams*)) );
    QObject::connect(this, SIGNAL(doAlertHidePopup(void)),
        ((QtMainWindow*)qtMainWindow), SLOT(alertHidePopup(void)) );
    QObject::connect(this, SIGNAL(doDateTimePicker(CDateTimeMessage*)),
        ((QtMainWindow*)qtMainWindow), SLOT(dateTimePicker(CDateTimeMessage*)) );
    QObject::connect(this, SIGNAL(doExecuteCommand(RhoNativeViewRunnable*)),
        ((QtMainWindow*)qtMainWindow), SLOT(executeCommand(RhoNativeViewRunnable*)) );
    QObject::connect(this, SIGNAL(doExecuteRunnable(rho::common::IRhoRunnable*)),
        ((QtMainWindow*)qtMainWindow), SLOT(executeRunnable(rho::common::IRhoRunnable*)) );
    QObject::connect(this, SIGNAL(doTakeSignature(void*)), //TODO: Signature::Params*
        ((QtMainWindow*)qtMainWindow), SLOT(takeSignature(void*)) );
    QObject::connect(this, SIGNAL(doFullscreenCommand(int)),
        ((QtMainWindow*)qtMainWindow), SLOT(fullscreenCommand(int)) );
    QObject::connect(this, SIGNAL(doSetCookie(const char*, const char*)),
        ((QtMainWindow*)qtMainWindow), SLOT(setCookie(const char*, const char*)) );
    QObject::connect(this, SIGNAL(doSetFrame(int,int,int,int)),
        ((QtMainWindow*)qtMainWindow), SLOT(setFrame(int,int,int,int)) );
    QObject::connect(this, SIGNAL(doSetPosition(int,int)),
        ((QtMainWindow*)qtMainWindow), SLOT(setPosition(int,int)) );
    QObject::connect(this, SIGNAL(doSetSize(int,int)),
        ((QtMainWindow*)qtMainWindow), SLOT(setSize(int,int)) );
    QObject::connect(this, SIGNAL(doLockSize(int)),
        ((QtMainWindow*)qtMainWindow), SLOT(lockSize(int)) );
    QObject::connect(this, SIGNAL(doSetTitle(const char*)),
        ((QtMainWindow*)qtMainWindow), SLOT(setTitle(const char*)) );
    QObject::connect(this, SIGNAL(doMinimizeWindow(void)),
        this, SLOT(minimizeWindow(void)) );
    QObject::connect(this, SIGNAL(doRestoreWindow(void)),
        this, SLOT(restoreWindow(void)) );
    QObject::connect(this, SIGNAL(doCreateCustomMenu(void)),
        this, SLOT(createCustomMenuSlot(void)) );
    QObject::connect(this, SIGNAL(doUnsetProxy()),
        this, SLOT(unsetProxy()) );
    QObject::connect(this, SIGNAL(doSetProxy(const char*, const char*, const char*, const char*)),
        this, SLOT(setProxy(const char*, const char*, const char*, const char*)) );
    return true;
}

void CMainWindow::diconnectFromUI()
{
    QObject::disconnect(this, SIGNAL(doExitCommand(void)),
        ((QtMainWindow*)qtMainWindow), SLOT(exitCommand(void)) );
    QObject::disconnect(this, SIGNAL(doNavigateBackCommand(void)),
        ((QtMainWindow*)qtMainWindow), SLOT(navigateBackCommand(void)) );
    QObject::disconnect(this, SIGNAL(doNavigateForwardCommand(void)),
        ((QtMainWindow*)qtMainWindow), SLOT(navigateForwardCommand(void)) );
    QObject::disconnect(this, SIGNAL(doWebviewNavigateBackCommand(int)),
        ((QtMainWindow*)qtMainWindow), SLOT(webviewNavigateBackCommand(int)) );
    QObject::disconnect(this, SIGNAL(doLogCommand(void)),
        ((QtMainWindow*)qtMainWindow), SLOT(logCommand(void)) );
    QObject::disconnect(this, SIGNAL(doRefreshCommand(int)),
        ((QtMainWindow*)qtMainWindow), SLOT(refreshCommand(int)) );
    QObject::disconnect(this, SIGNAL(doNavigateCommand(TNavigateData*)),
        ((QtMainWindow*)qtMainWindow), SLOT(navigateCommand(TNavigateData*)) );
    QObject::disconnect(this, SIGNAL(doExecuteJavaScriptCommand(TNavigateData*)),
        ((QtMainWindow*)qtMainWindow), SLOT(executeJavaScriptCommand(TNavigateData*)) );
    QObject::disconnect(this, SIGNAL(doTakePicture(char*)),
        ((QtMainWindow*)qtMainWindow), SLOT(takePicture(char*)) );
    QObject::disconnect(this, SIGNAL(doSelectPicture(char*)),
        ((QtMainWindow*)qtMainWindow), SLOT(selectPicture(char*)) );
    QObject::disconnect(this, SIGNAL(doAlertShowPopup(CAlertParams*)),
        ((QtMainWindow*)qtMainWindow), SLOT(alertShowPopup(CAlertParams*)) );
    QObject::disconnect(this, SIGNAL(doAlertHidePopup(void)),
        ((QtMainWindow*)qtMainWindow), SLOT(alertHidePopup(void)) );
    QObject::disconnect(this, SIGNAL(doDateTimePicker(CDateTimeMessage*)),
        ((QtMainWindow*)qtMainWindow), SLOT(dateTimePicker(CDateTimeMessage*)) );
    QObject::disconnect(this, SIGNAL(doExecuteCommand(RhoNativeViewRunnable*)),
        ((QtMainWindow*)qtMainWindow), SLOT(executeCommand(RhoNativeViewRunnable*)) );
    QObject::disconnect(this, SIGNAL(doExecuteRunnable(rho::common::IRhoRunnable*)),
        ((QtMainWindow*)qtMainWindow), SLOT(executeRunnable(rho::common::IRhoRunnable*)) );
    QObject::disconnect(this, SIGNAL(doTakeSignature(void*)), //TODO: Signature::Params*
        ((QtMainWindow*)qtMainWindow), SLOT(takeSignature(void*)) );
    QObject::disconnect(this, SIGNAL(doFullscreenCommand(int)),
        ((QtMainWindow*)qtMainWindow), SLOT(fullscreenCommand(int)) );
    QObject::disconnect(this, SIGNAL(doSetCookie(const char*, const char*)),
        ((QtMainWindow*)qtMainWindow), SLOT(setCookie(const char*, const char*)) );
    QObject::disconnect(this, SIGNAL(doSetFrame(int,int,int,int)),
        ((QtMainWindow*)qtMainWindow), SLOT(setFrame(int,int,int,int)) );
    QObject::disconnect(this, SIGNAL(doSetPosition(int,int)),
        ((QtMainWindow*)qtMainWindow), SLOT(setPosition(int,int)) );
    QObject::disconnect(this, SIGNAL(doSetSize(int,int)),
        ((QtMainWindow*)qtMainWindow), SLOT(setSize(int,int)) );
    QObject::disconnect(this, SIGNAL(doLockSize(int)),
        ((QtMainWindow*)qtMainWindow), SLOT(lockSize(int)) );
    QObject::disconnect(this, SIGNAL(doSetTitle(const char*)),
        ((QtMainWindow*)qtMainWindow), SLOT(setTitle(const char*)) );
    QObject::disconnect(this, SIGNAL(doMinimizeWindow(void)),
        this, SLOT(minimizeWindow(void)) );
    QObject::disconnect(this, SIGNAL(doRestoreWindow(void)),
        this, SLOT(restoreWindow(void)) );
    QObject::disconnect(this, SIGNAL(doCreateCustomMenu(void)),
        this, SLOT(createCustomMenuSlot(void)) );
    QObject::disconnect(this, SIGNAL(doUnsetProxy()),
        this, SLOT(unsetProxy()) );
    QObject::disconnect(this, SIGNAL(doSetProxy(const char*, const char*, const char*, const char*)),
        this, SLOT(setProxy(const char*, const char*, const char*, const char*)) );
}

void CMainWindow::messageLoop(void)
{
    //((QApplication*)qtApplication)->exec();
}

bool CMainWindow::getFullScreen()
{
    return ((QtMainWindow*)qtMainWindow)->getFullScreen();
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

int CMainWindow::getLogicalDpiX()
{
    return ((QtMainWindow*)qtMainWindow)->getLogicalDpiX();
}

int CMainWindow::getLogicalDpiY()
{
    return ((QtMainWindow*)qtMainWindow)->getLogicalDpiY();
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
    if (!rho_rhodesapp_check_mode())
        return;

    std::auto_ptr<QColor> m_rgbBackColor (NULL);
    std::auto_ptr<QColor> m_rgbMaskColor (NULL);
    int m_nHeight = MIN_TOOLBAR_HEIGHT;

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
                        ((QtMainWindow*)qtMainWindow)->toolbarAddSeparator(nItemWidth);
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
                    } else {
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
                        strImagePath = strImagePath.length() > 0 ? CFilePath::join( RHOSIMCONF().getRhodesPath(), "lib/framework/" + strImagePath) : String();
#else
                        strImagePath = strImagePath.length() > 0 ? CFilePath::join( rho_native_reruntimepath() , "lib/" + strImagePath) : String();
#endif
                    }

                    ((QtMainWindow*)qtMainWindow)->toolbarAddAction(QIcon(QString(strImagePath.c_str())), QString(label), action, wasSeparator);
                }
            }
        }
    }
    ((QtMainWindow*)qtMainWindow)->setToolbarStyle(false, (m_rgbBackColor.get()!=NULL ? m_rgbBackColor->name() : ""), m_nHeight);
    ((QtMainWindow*)qtMainWindow)->toolbarShow();
    m_started = true;
}

bool charToBool(const char* str)
{
    return str && ((strcasecmp(str,"true")==0) || (strcasecmp(str,"yes")==0) || (atoi(str)==1));
}

void CMainWindow::createTabbarEx(const rho::Vector<rho::String>& tabbarElements, const rho::Hashtable<rho::String, rho::String>& tabBarProperties, rho::apiGenerator::CMethodResult& oResult)
{
    if (!rho_rhodesapp_check_mode())
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
    }

    if (oResult.hasCallback())
        ((QtMainWindow*)qtMainWindow)->tabbarSetSwitchCallback(oResult);

    ((QtMainWindow*)qtMainWindow)->tabbarShow();

    m_started = true;
}

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
    //((QtMainWindow*)qtMainWindow)->tabbarSetBadge(index, badge); TODO: fix it
}

int CMainWindow::tabbarGetCurrent()
{
    return ((QtMainWindow*)qtMainWindow)->tabbarGetCurrent();
}
void *CMainWindow::getQtMainWindow() const
{
    return qtMainWindow;
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

void CMainWindow::menuAddAction(const char* label, int item, bool enabled)
{
    ((QtMainWindow*)qtMainWindow)->menuAddAction(QString(label), item, enabled);
}
#endif
// Handlers
void CMainWindow::onActivate(int active)
{
    rho_rhodesapp_callAppActiveCallback(active);
    if (!active)
        rho_geoimpl_turngpsoff();
}

#ifndef RHODES_VERSION_LIBRARY
// Commands
void CMainWindow::exitCommand()
{
    emit doExitCommand();
}

void CMainWindow::navigateBackCommand()
{
    emit doNavigateBackCommand();
}

void CMainWindow::navigateForwardCommand()
{
    emit doNavigateForwardCommand();
}

void CMainWindow::webviewNavigateBackCommand(int tab_index)
{
    emit doWebviewNavigateBackCommand(tab_index);
}

void CMainWindow::logCommand()
{
    emit doLogCommand();
}

void CMainWindow::refreshCommand(int tab_index)
{
    emit doRefreshCommand(tab_index);
}

void CMainWindow::navigateCommand(TNavigateData* nd)
{
    emit doNavigateCommand(nd);
}

void CMainWindow::executeJavaScriptCommand(TNavigateData* nd)
{
    emit doExecuteJavaScriptCommand(nd);
}

void CMainWindow::takePicture(char* callbackUrl)
{
    emit doTakePicture(callbackUrl);
}

void CMainWindow::selectPicture(char* callbackUrl)
{
    emit doSelectPicture(callbackUrl);
}

void CMainWindow::alertShowPopup(CAlertParams *params)
{
    emit doAlertShowPopup(params);
}

void CMainWindow::alertHidePopup()
{
    emit doAlertHidePopup();
}

void CMainWindow::dateTimePicker(CDateTimeMessage * msg)
{
    emit doDateTimePicker(msg);
}

void CMainWindow::executeCommand(RhoNativeViewRunnable* runnable)
{
    emit doExecuteCommand(runnable);
}

void CMainWindow::executeRunnable(rho::common::IRhoRunnable* pTask)
{
    emit doExecuteRunnable(pTask);
}

void CMainWindow::takeSignature(void* params) //TODO: Signature::Params*
{
    emit doTakeSignature(params);
}

void CMainWindow::fullscreenCommand(int enable)
{
    emit doFullscreenCommand(enable);
}

void CMainWindow::setCookie(const char* url, const char* cookie)
{
    emit doSetCookie(url, cookie);
}

void CMainWindow::bringToFront()
{
    emit doBringToFront();
}

void CMainWindow::setFrame(int x, int y, int width, int height)
{
    emit doSetFrame(x, y, width, height);
}

void CMainWindow::setPosition(int x, int y)
{
    emit doSetPosition(x, y);
}

void CMainWindow::setSize(int width, int height)
{
    emit doSetSize(width, height);
}

void CMainWindow::lockSize(int locked)
{
    emit doLockSize(locked);
}

void CMainWindow::setTitle(const char* title)
{
    emit doSetTitle(title);
}

void CMainWindow::createCustomMenuCommand(void)
{
    emit doCreateCustomMenu();
}

void CMainWindow::minimizeWindowCommand(void)
{
    emit doMinimizeWindow();
}

void CMainWindow::restoreWindowCommand(void)
{
    emit doRestoreWindow();
}

void CMainWindow::unsetProxyCommand()
{
    emit doUnsetProxy();
}

void CMainWindow::setProxyCommand(const char* host, const char* port, const char* login, const char* password)
{
    emit doSetProxy(host, port, login, password);
}

void CMainWindow::createCustomMenuSlot(void)
{
    createCustomMenu();
}
#endif

extern "C" void rho_os_impl_performOnUiThread(rho::common::IRhoRunnable* pTask)
{
#ifndef RHODES_VERSION_LIBRARY
    CMainWindow::getInstance()->executeRunnable(pTask);
#else
    pTask->runObject();
#endif
}

#if defined(RHODES_QT_PLATFORM) && defined(OS_MACOSX)
extern "C" void rho_qt_sys_minimize()
{
    CMainWindow::getInstance()->minimizeWindowCommand();
}
extern "C" void rho_qt_sys_restore_window()
{
    CMainWindow::getInstance()->restoreWindowCommand();
}
#endif
