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

#ifdef OS_WP8
extern "C" void createMenu();
#endif
#ifdef OS_UWP
extern "C" void createMenu();
#endif
#ifdef RHODES_QT_PLATFORM
extern "C" void rho_symimpl_createMenu();
#endif

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

const String CAppMenu::g_actionTag  = "action";
const String CAppMenu::g_labelTag   = "label";
const String CAppMenu::g_disableTag = "disabled";

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

/*void CAppMenu::getMenuItems(rho::Vector< Hashtable<String, String> >& arRes )
{
    rho::Vector<rho::common::CAppMenuItem> arAppMenuItems;
    copyMenuItems(arAppMenuItems, false);

    for ( int i = 0; i < (int)arAppMenuItems.size(); i++)
    {
        Hashtable<String, String> hash;
        hash[arAppMenuItems[i].m_strLabel] = arAppMenuItems[i].m_strLink;
        arRes.addElement(hash);
    }
}*/

void CAppMenu::getMenuItemsEx(rho::Vector< Hashtable<String, String> >& arRes, bool bLeftMenu )
{
    rho::Vector<rho::common::CAppMenuItem> arAppMenuItems;
    copyMenuItems(arAppMenuItems, bLeftMenu);

    for ( int i = 0; i < (int)arAppMenuItems.size(); i++)
    {
        Hashtable<String, String> hash;
        hash[g_labelTag]   = arAppMenuItems[i].m_strLabel;
        hash[g_actionTag]  = arAppMenuItems[i].m_strLink;
        hash[g_disableTag] = arAppMenuItems[i].m_isEnable == false ? "true" : "false";
        arRes.addElement(hash);
    }
}
void CAppMenu::getMenuButtonEx(Hashtable<String, String>& hashRes, bool bLeftItem/* = false*/)
{
    CAppMenuItem oItem = bLeftItem ? getLeftButton() : getRightButton();

    hashRes.put(g_labelTag, oItem.m_strLabel);
    hashRes.put(g_actionTag, oItem.m_strLink);
    hashRes.put(g_disableTag, oItem.m_isEnable == false ? "true" : "false");
}

void CAppMenu::changeButtonInfo(CAppMenuItem& button, const rho::Hashtable<rho::String, rho::String>& hashButton)
{
    typedef rho::Hashtable<rho::String, rho::String>::const_iterator citerator_type;

    synchronized(m_mxAppMenu) 
	{
        String strLabel, strAction, strDisable;
        citerator_type it = hashButton.end();
     
        it = hashButton.find(g_labelTag);

        if (it != hashButton.end())
            strLabel = it->second;
        else
            return;

        it = hashButton.find(g_actionTag);

        if (it != hashButton.end())
            strAction = it->second;
        else
            return;

        it = hashButton.find(g_disableTag);

        if (it != hashButton.end())
            strDisable = it->second;
        else
            return;

        button = CAppMenuItem(strLabel, strAction);

        if (strDisable == "true")
            button.m_isEnable = false;
        else
            button.m_isEnable = true;
    }
}

void CAppMenu::setLeftButton( const rho::Hashtable<rho::String, rho::String>& hashButton )
{
    synchronized(m_mxAppMenu) 
	{
        changeButtonInfo(m_oLeftItem, hashButton);
    }
}

void CAppMenu::setRightButton( const rho::Hashtable<rho::String, rho::String>& hashButton )
{
    synchronized(m_mxAppMenu) 
	{
        changeButtonInfo(m_oRightItem, hashButton);
    }
}

CAppMenuItem CAppMenu::getLeftButton()
{
    synchronized(m_mxAppMenu) 
	{
        return m_oLeftItem;
    }
}

CAppMenuItem CAppMenu::getRightButton()
{
    synchronized(m_mxAppMenu) 
	{
        return m_oRightItem;
    }
}

void CAppMenu::setAppMenuJSONItemsEx( const rho::Vector<rho::String>& arMenu, bool bLeftMenu/* = false*/ )
{
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

            String label, action, disable;

            while(!oIter.isEnd())
            {
                String strKey = oIter.getCurKey();
                String strValue = oIter.getCurValue().isNull() ? "" : oIter.getCurString();

                if (strKey == g_labelTag)
                    label = strValue;

                if (strKey == g_actionTag)
                    action = strValue;

                if (strKey == g_disableTag)
                    disable = strValue;

                oIter.next();
            }

            addAppMenuItem( label, action, bLeftMenu );

            bool isDisable = (bool)(disable == "true");
            setEnableMenuItem(label, !isDisable, bLeftMenu);

        }
#if defined(OS_WP8) || defined (OS_UWP)
        createMenu();
#endif
#ifdef RHODES_QT_PLATFORM
        rho_symimpl_createMenu();
#endif
    }
}

void CAppMenu::setAppBackUrlWithJSONItems( const rho::Vector<rho::String>& arMenu )
{
    RHODESAPP().setAppBackUrl("");
    
    for (int i = 0; i < (int)arMenu.size(); i++)
    {
        rho::json::CJSONStructIterator oIter(arMenu[i].c_str());
        String label, action;
        
        
        while(!oIter.isEnd())
        {
            String strKey = oIter.getCurKey();
            String strValue = oIter.getCurValue().isNull() ? "" : oIter.getCurString();
            
            if (strKey == g_labelTag)
                label = strValue;
            
            if (strKey == g_actionTag)
                action = strValue;
            
            oIter.next();
        }
        
        if ( strcasecmp( label.c_str(), "back" )==0 && strcasecmp( action.c_str(), "back" )!=0 ) {
            RHODESAPP().setAppBackUrl(action);
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

CAppMenuItem::CAppMenuItem (const String& strLabel, const String& strLink):
    m_eType(emtNone)
{
	m_strLabel = strLabel;
	m_strLink  = strLink;
    m_isEnable = true;
	
	if (strLabel == "separator" || strLink == "separator")
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
