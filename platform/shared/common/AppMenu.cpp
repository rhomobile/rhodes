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

#include "AppMenu.h"
#include "common/RhodesApp.h"
#include "ruby/ext/rho/rhoruby.h"
#include "rubyext/WebView.h"

extern "C" void rho_sync_doSyncAllSources(int show_status_popup, const char* query_params);
extern "C" void rho_conf_show_log();
extern "C" void rho_sys_app_exit();

namespace rho {
namespace common{

IMPLEMENT_LOGCLASS(CAppMenu, "AppMenu");

void CAppMenu::addAppMenuItem( const String& strLabel, const String& strLink )
{
    if ( strLabel.length() == 0 )
        return;

    if ( strcasecmp( strLabel.c_str(), "back" )==0 && strcasecmp( strLink.c_str(), "back" )!=0 )
        RHODESAPP().setAppBackUrl(strLink);
    else
    {
        synchronized(m_mxAppMenu)
	    {
        	m_arAppMenuItems.push_back(CAppMenuItem(strLabel, strLink));
        }
    }
}

extern "C" void
menu_iter(const char* szLabel, const char* szLink, void* pThis)
{
	((CAppMenu*)pThis)->addAppMenuItem(szLabel, szLink );
}

void CAppMenu::setAppMenu(unsigned long valMenu)
{
    synchronized(m_mxAppMenu)
	{
		m_arAppMenuItems.clear();
        RHODESAPP().setAppBackUrl("");
        rho_ruby_enum_strhash(valMenu, menu_iter, this);
    }
}

void CAppMenu::copyMenuItems(Vector<CAppMenuItem>& arAppMenuItems)
{
    synchronized(m_mxAppMenu)
	{
        arAppMenuItems = m_arAppMenuItems;
    }
}

CAppMenuItem::CAppMenuItem (const String& strLabel, const String& strLink)
{
	m_strLabel = strLabel;
	m_strLink  = strLink;

	if (strLabel == "separator")
		m_eType = emtSeparator;
	else if (strLink == "home")
		m_eType = emtHome;
	else if (strLink == "refresh")
		m_eType = emtRefresh;
	else if (strLink == "options")
		m_eType = emtOptions;
	else if (strLink == "log")
		m_eType = emtLog;
	else if (strLink == "sync")
		m_eType = emtSync;
	else if (strLink == "close")
		m_eType = emtClose;
	else if (strLink == "exit")
		m_eType = emtExit;
	else if (strLink == "back")
		m_eType = emtBack;
	else if (strLink == "fullscreen")
		m_eType = emtFullscreen;
	else
		m_eType = emtUrl;
}

boolean CAppMenuItem::processCommand()
{
    switch(m_eType)
    {
    case CAppMenuItem::emtUrl:
        rho_rhodesapp_load_url(m_strLink.c_str());
        break;
    case CAppMenuItem::emtRefresh:
        rho_webview_refresh(0);
        break;
    case CAppMenuItem::emtSync:
        rho_sync_doSyncAllSources(1, "");
        break;
    case CAppMenuItem::emtLog:
        rho_conf_show_log();
        break;
    case CAppMenuItem::emtExit:
    case CAppMenuItem::emtClose:
        rho_sys_app_exit();
        break;

    case CAppMenuItem::emtHome:
        rho_webview_navigate(RHODESAPP().getStartUrl().c_str(), 0);
        break;
    case CAppMenuItem::emtBack:
        RHODESAPP().navigateBack();
        break;
    case CAppMenuItem::emtOptions:
        rho_webview_navigate(RHODESAPP().getOptionsUrl().c_str(), 0);
        break;

    case CAppMenuItem::emtFullscreen:
        rho_webview_full_screen_mode(1);
        break;

    default:
        return false;
    }

    return true;
}

} //namespace common
} //namespace rho
