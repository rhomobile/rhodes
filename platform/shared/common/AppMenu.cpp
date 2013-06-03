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
#include "sync/RhoconnectClientManager.h"
#include "json/JSONIterator.h"

#include <algorithm>
#include <functional>

namespace
{

bool isCurrentMenuItem(const rho::common::CAppMenuItem itemInfo, const rho::String findLabel) 
{
    if (itemInfo.m_strLabel == findLabel)
        return true;

    return false;
}

}

namespace rho {
namespace common{

IMPLEMENT_LOGCLASS(CAppMenu, "AppMenu");

void CAppMenu::setEnableMenuItem( const String& strLabel, bool enableItem, bool bLeftMenu )
{
    synchronized(m_mxAppMenu) 
	{
        Vector<CAppMenuItem>::iterator findIt;

        if (bLeftMenu)
        {
            findIt = std::find_if(m_arAppLeftMenuItems.begin(), m_arAppLeftMenuItems.end(), 
                std::bind2nd(std::ptr_fun(&isCurrentMenuItem), strLabel));

            if (findIt != m_arAppLeftMenuItems.end())
            {
                findIt->m_isEnable = enableItem;
            }
        }
        else
        {
            findIt = std::find_if(m_arAppMenuItems.begin(), m_arAppMenuItems.end(), 
                std::bind2nd(std::ptr_fun(&isCurrentMenuItem), strLabel));

            if (findIt != m_arAppMenuItems.end())
            {
                findIt->m_isEnable = enableItem;
            }
        }
    }
}

void CAppMenu::addAppMenuItem( const String& strLabel, const String& strLink, bool bLeftMenu )
{
    if ( strLabel.length() == 0 )
        return;

    if ( strcasecmp( strLabel.c_str(), "back" )==0 && strcasecmp( strLink.c_str(), "back" )!=0 )
        RHODESAPP().setAppBackUrl(strLink);
    else if ( bLeftMenu )
        m_arAppLeftMenuItems.push_back(CAppMenuItem(strLabel, strLink));
    else
      	m_arAppMenuItems.push_back(CAppMenuItem(strLabel, strLink));
}

void CAppMenu::getMenuItemsEx(rho::Vector< Hashtable<String, String> >& arRes, bool bLeftMenu )
{
    rho::Vector<rho::common::CAppMenuItem> arAppMenuItems;
    copyMenuItems(arAppMenuItems, bLeftMenu);

    for ( int i = 0; i < (int)arAppMenuItems.size(); i++)
    {
        Hashtable<String, String> hash;
        hash[arAppMenuItems[i].m_strLabel] = arAppMenuItems[i].m_strLink;
        arRes.addElement(hash);
    }
}

void CAppMenu::getMenuItemEx(Hashtable<String, String>& hashRes, bool bLeftItem/* = false*/)
{
    CAppMenuItem oItem = bLeftItem ? getLeftItem() : getRightItem();

    hashRes.put(oItem.m_strLabel, oItem.m_strLink);
}

void CAppMenu::setLeftItem( const String& strLabel, const String& strLink )
{
    synchronized(m_mxAppMenu) 
	{
        m_oLeftItem = CAppMenuItem(strLabel, strLink );
    }
}

void CAppMenu::setRightItem( const String& strLabel, const String& strLink )
{
    synchronized(m_mxAppMenu) 
	{
        m_oRightItem = CAppMenuItem(strLabel, strLink );
    }
}

CAppMenuItem CAppMenu::getLeftItem()
{
    synchronized(m_mxAppMenu) 
	{
        return m_oLeftItem;
    }
}

CAppMenuItem CAppMenu::getRightItem()
{
    synchronized(m_mxAppMenu) 
	{
        return m_oRightItem;
    }
}

void CAppMenu::setEnableLeftItem( bool isEnable )
{
    synchronized(m_mxAppMenu) 
	{
        m_oLeftItem.m_isEnable = isEnable;
    }
}

void CAppMenu::setEnableRightItem( bool isEnable )
{
    synchronized(m_mxAppMenu) 
	{
        m_oRightItem.m_isEnable = isEnable;
    }
}

void CAppMenu::setAppMenuJSONItems( const rho::Vector<rho::String>& arMenu, bool bLeftMenu/* = false*/ )
{
    //rho::Vector< Hashtable<String, String> > arRes;

    synchronized(m_mxAppMenu) 
	{
        if ( bLeftMenu )
		    m_arAppLeftMenuItems.clear();
        else
            m_arAppMenuItems.clear();

        RHODESAPP().setAppBackUrl("");
        for (int i = 0; i < (int)arMenu.size(); i++)
        {
            rho::json::CJSONStructIterator oIter(arMenu[i].c_str());
            String strKey = oIter.getCurKey();
            String strValue = oIter.getCurValue().isNull() ? "" : oIter.getCurString();
            addAppMenuItem( strKey, strValue, bLeftMenu );
        }
    }
}

void CAppMenu::copyMenuItems(Vector<CAppMenuItem>& arAppMenuItems, bool bLeftMenu)
{
    synchronized(m_mxAppMenu) 
	{
        if (bLeftMenu)
            arAppMenuItems = m_arAppLeftMenuItems; 
        else
            arAppMenuItems = m_arAppMenuItems; 
    }
}

CAppMenuItem::CAppMenuItem (const String& strLabel, const String& strLink)
{
	m_strLabel = strLabel;
	m_strLink  = strLink;
    m_isEnable = true;
	
	if (strLabel == "separator")
		m_eType = emtSeparator;
/*	else if (strLink == "home")
		m_eType = emtHome;
	else if (strLink == "refresh")
		m_eType = emtRefresh;
	else if (strLink == "options")
		m_eType = emtOptions;
	else if (strLink == "log")
		m_eType = emtLog;
	else if (strLink == "sync")
		m_eType = emtSync;*/
	else if (strLink == "close")
		m_eType = emtClose;
	else if (strLink == "exit")
		m_eType = emtExit;
	/*else if (strLink == "back")
		m_eType = emtBack;
	else if (strLink == "fullscreen")
		m_eType = emtFullscreen;
	else
		m_eType = emtUrl;*/
}

void CAppMenuItem::processCommand()
{
    if ( m_eType != emtSeparator )
        RHODESAPP().loadUrl(m_strLink.c_str());

/*
    switch(m_eType)
    {
    case CAppMenuItem::emtUrl:
        rho_rhodesapp_load_url(m_strLink.c_str());
        break;
    case CAppMenuItem::emtRefresh:
        rho_webview_refresh(0);
        break;
    case CAppMenuItem::emtSync:
		if ( sync::RhoconnectClientManager::haveRhoconnectClientImpl() ) {
			sync::RhoconnectClientManager::doSyncAllSources(1, "", 0);
		}
        break;
    case CAppMenuItem::emtLog:
        rho_conf_show_log();
        break;
    case CAppMenuItem::emtExit:
    case CAppMenuItem::emtClose:
        rho_sys_app_exit();
        break;

    case CAppMenuItem::emtHome:
        rho_webview_navigate(RHODESAPP().getStartUrl().c_str(), -1);
        break;
    case CAppMenuItem::emtBack:
        RHODESAPP().navigateBack();
        break;
    case CAppMenuItem::emtOptions:
        rho_webview_navigate(RHODESAPP().getOptionsUrl().c_str(), -1);
        break;

    case CAppMenuItem::emtFullscreen:
        rho_webview_full_screen_mode(1);
        break;

    default:
        return false;
    }

    return true;
*/
}

} //namespace common
} //namespace rho
