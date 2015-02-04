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

#include "stdafx.h"

#include "NativeTabbar.h"
#include "MainWindow.h"
#include "rubyext/WebView.h"
#include "json/JSONIterator.h"

IMPLEMENT_LOGCLASS(CNativeTabbar,"NativeTabbar");

extern CMainWindow& getAppWindow();

using namespace rho::json;

CNativeTabbar::CNativeTabbar()
{
    m_nCurrentTab = -1;
    m_bTabCreated = false;
}

static int getColorFromString(const char* szColor)
{
    if ( !szColor || !*szColor )
        return RGB(0, 0, 0);

	int c = atoi(szColor);

	int cR = (c & 0xFF0000) >> 16;
	int cG = (c & 0xFF00) >> 8;
	int cB = (c & 0xFF);

    return RGB(cR, cG, cB);
}

void CNativeTabbar::CreateTabbarEx(const rho::Vector<rho::String>& tabbarElements, const rho::Hashtable<rho::String, rho::String>& tabBarProperties, rho::apiGenerator::CMethodResult& oResult)
{
    if (!rho_rhodesapp_check_mode() )
        return;

    RemoveTabbar();

	COLORREF   rgbBackColor;
    bool bHiddenTabs = false, bCreateOnInit = false;

    for ( Hashtable<rho::String, rho::String>::const_iterator it = tabBarProperties.begin(); it != tabBarProperties.end(); ++it )
    {
        const char *name = (it->first).c_str();
        const char *value = (it->second).c_str();
        if (strcasecmp(name, "backgroundColor") == 0) 
            rgbBackColor = getColorFromString(value);
        if (strcasecmp(name, "hiddenTabs") == 0) 
            bHiddenTabs = strcasecmp(value, "true") == 0;
        if (strcasecmp(name, "createOnInit") == 0) 
            bCreateOnInit = strcasecmp(value, "true") == 0;

    }

    if ( !bHiddenTabs )
    {
        LOG(WARNING) + "Illegal argument for create_nativebar: hiddenTabs should be true for Windows Mobile and Windows CE.";
        bHiddenTabs = true;
    }

    int nStartTab = -1;

    for (int i = 0; i < (int)tabbarElements.size(); ++i) 
    {
        const char *label = NULL;
        const char *action = NULL;
        const char *icon = NULL;
        const char *reload = NULL;
        const char *colored_icon = NULL;
        
    	COLORREF selected_color;
        const char *disabled = NULL;
		COLORREF web_bkg_color;
        const char* use_current_view_for_tab = NULL;
        bool bUseCurrentViewForTab = false, bReloadPage = false, bPerishable = false, bCreateOnInitTab = false;
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
        {
            reload = oEntry.getString("reload");
            if (strcasecmp(reload, "true") == 0)
                bReloadPage = true;
        }
        if ( oEntry.hasName("perishable") )
        {
            const char* perishable = oEntry.getString("perishable");
            if (strcasecmp(perishable, "true") == 0)
                bPerishable = true;
        }
        if ( oEntry.hasName("createOnInit") )
        {
            const char* createOnInit = oEntry.getString("createOnInit");
            if (strcasecmp(createOnInit, "true") == 0)
                bCreateOnInitTab = true;
        }

        if ( oEntry.hasName("selectedColor") )
            selected_color = getColorFromString(oEntry.getString("selectedColor"));
        if ( oEntry.hasName("disabled") )
            disabled = oEntry.getString("disabled");
        if ( oEntry.hasName("useCurrentViewForTab") )
        {
            use_current_view_for_tab = oEntry.getString("useCurrentViewForTab");
            if (strcasecmp(use_current_view_for_tab, "true") == 0)
                bUseCurrentViewForTab = true;
        }
        if (oEntry.hasName("backgroundColor")) 
            web_bkg_color = getColorFromString(oEntry.getString("backgroundColor"));

        if (label == NULL)
            label = "";
        if (action == NULL)
            action = "";
        
        m_arTabs.addElement(CTabBarItem(action, label, bUseCurrentViewForTab, bReloadPage, bPerishable, bCreateOnInitTab));

        if (m_strStartTabName.length()>0 && m_strStartTabName == label)
            nStartTab = i;

    }

    if ( oResult.hasCallback() )
        m_oCallback = oResult;

    for ( int i = 0; i < (int)m_arTabs.size(); i++ )
    {
        if ( m_arTabs[i].m_bCreateOnInit || bCreateOnInit )
            SwitchTab(i, true);
    }

    if (m_strStartTabName.length()>0&&nStartTab>=0)
    {
        //getAppWindow().SetTimer( TABBAR_TIMER_ID, 1000 );
        SwitchTabByName( m_strStartTabName.c_str(), false );
        m_strStartTabName = "";
    }else if ( m_arTabs.size() ) //&& !m_arTabs[0].m_bUseCurrentViewForTab )
        SwitchTab(0);

    m_bTabCreated = true;
}

LRESULT CNativeTabbar::OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SwitchTabByName( m_strStartTabName.c_str(), false );
    m_strStartTabName = "";
    getAppWindow().KillTimer( TABBAR_TIMER_ID ); 

    return 0;
}

void CNativeTabbar::RemoveTabbar()
{
    getAppWindow().getWebKitEngine()->SwitchTab(0); //Switch to main view

    for ( int i = 0; i < (int)m_arTabs.size(); i++ )
    {
        if (m_arTabs[i].m_nTabID > 0 ) //do not remove tab 0
            getAppWindow().getWebKitEngine()->CloseTab(m_arTabs[i].m_nTabID);
    }

    m_arTabs.removeAllElements();
    m_bTabCreated = false;
    m_nCurrentTab = -1;
}

void CNativeTabbar::RemoveTab(int index)
{
    if (index >= 0 && index < (int)m_arTabs.size() )
    {
        if (m_arTabs[index].m_nTabID > 0 ) //do not remove tab 0
        {
            LOG(INFO) + "Close Tab: " + index;
            getAppWindow().getWebKitEngine()->CloseTab(m_arTabs[index].m_nTabID);

            m_arTabs[index].m_nTabID = -1;
            m_arTabs[index].m_hwndTab = 0;
        }
    }
}

void CNativeTabbar::SwitchTabByName(const char* szTabName, bool bExecuteJS)
{
    LOG(INFO) + "tabbarSwitchByName: " + szTabName;

    if ( !szTabName || !*szTabName)
        return;

    for ( int i = 0; i < (int)m_arTabs.size(); i++ )
    {
        if ( m_arTabs[i].m_strLabel == szTabName )
        {
            /*if ( bExecuteJS )
            {
                String strJS = "Rho.NativeTabbar.switchTab(";
                strJS += convertToStringA(i);
                strJS += ");";

                rho_webview_execute_js( strJS.c_str(), GetCurrentTabIndex() );
            }
            else*/
                SwitchTab(i, false, true);
            break;
        }
    }
}

void CNativeTabbar::raiseTabEvent( const char* szEventName, int nOldTab, int nNewTab )
{
    if ( m_oCallback.hasCallback() )
    {
        LOG(INFO) + "raiseTabEvent: " + szEventName + "; " + nOldTab + "; " + nNewTab;

        Hashtable<String,String> mapRes;
        mapRes["tab_index"] = convertToStringA(nNewTab);
        mapRes["newTabIndex"] = convertToStringA(nNewTab);
        mapRes["oldTabIndex"] = convertToStringA(nOldTab);
        mapRes["tabEvent"] = szEventName;
        m_oCallback.set(mapRes);
    }
}

//as Newtab is a blocking function we have a timeout. 
#define NEWTAB_IPC_ERR	-1
#define NEWTAB_TIMEOUT	-2	// the new tab call timed out  
#define NEWTAB_LOW_MEM	-3 // A tab cannot be created due to hitting the memory limit m_iMemLimit
#define NEWTAB_MAX_TABS -4 // we have hit the #defined max tabs value defined in Engine2Core; MAX_TABS

bool CNativeTabbar::removePerishableTab()
{
    int nTabToClose = -1;
    for ( int i = 0; i < (int)m_arTabs.size(); i++ )
    {
        if ( m_arTabs[i].m_nTabID > 0 && m_arTabs[i].m_bPerishable && i != m_nCurrentTab )
        {
            nTabToClose = i;
            break;
        }
    }
    
    if ( nTabToClose == -1 && m_nCurrentTab > 0 )
        nTabToClose = m_nCurrentTab;

    if ( nTabToClose == -1 )
        return false;

    RemoveTab(nTabToClose);
    return true;
}

void CNativeTabbar::SwitchTab(int index, bool bCreateOnly/*=false*/, bool bByName /*= false*/)
{
    if (!getAppWindow().getWebKitEngine())
        return;

    if (( index >= (int)m_arTabs.size() )||(index<0))
    {
        LOG(ERROR) + "Invalid Tab Index: " + index;
        return;
    }
    
    bool bNewTab = !m_arTabs[index].m_hwndTab;
    if ( !m_arTabs[index].m_hwndTab )
    {
        if ( m_arTabs[index].m_bUseCurrentViewForTab)
        {
            m_arTabs[index].m_nTabID = 0;
            m_arTabs[index].m_hwndTab = getAppWindow().getWebKitEngine()->GetHTMLWND(0);

            bNewTab = false;
        }else
        {
            //raiseTabEvent( "onTabNewRequest", m_nCurrentTab, -1 );

            m_arTabs[index].m_nTabID = getAppWindow().getWebKitEngine()->NewTab();

            if ( m_arTabs[index].m_nTabID < 0 )
            {
                LOG(ERROR) + "New Tab failed with error: " + m_arTabs[index].m_nTabID + "; Tab Index: " + index;

                if ( /*!bCreateOnly &&*/ (m_arTabs[index].m_nTabID == NEWTAB_LOW_MEM || m_arTabs[index].m_nTabID == NEWTAB_MAX_TABS) )
                {
                    LOG(INFO) + "Try to close perishable tab.";
                    removePerishableTab();
                    m_arTabs[index].m_nTabID = getAppWindow().getWebKitEngine()->NewTab();

                    if ( m_arTabs[index].m_nTabID < 0 )
                    {
                        LOG(ERROR) + "New Tab failed with error: " + m_arTabs[index].m_nTabID + "; Tab Index: " + index;
                        raiseTabEvent( "onTabNewError", m_nCurrentTab, index );
                        return;
                    }
                }
            }
            else
                m_arTabs[index].m_hwndTab = getAppWindow().getWebKitEngine()->GetHTMLWND(m_arTabs[index].m_nTabID);

            //raiseTabEvent( "onTabNew", m_nCurrentTab, index );
        }
    }

    if ( !m_arTabs[index].m_hwndTab )
    {
        LOG(ERROR) + "Unable to create Tab. Cannot get Tab Window.";
        return;
    }

    if ( m_nCurrentTab != index && !bCreateOnly)
    {
		if(m_nCurrentTab != -1)
			m_arTabs[m_nCurrentTab].m_strTitle = RHOCONF().getString("title_text");

		getAppWindow().getWebKitEngine()->SwitchTab(m_arTabs[index].m_nTabID);        
        int nOldTab = m_nCurrentTab; 
        m_nCurrentTab = index;

        if (bNewTab || bByName)
            Sleep(200);

        if(m_nCurrentTab != -1)
		{
			RHOCONF().setString("title_text", m_arTabs[index].m_strTitle.c_str(), false);
			PostMessage(getAppWindow().m_hWnd, WM_COMMAND, ID_TITLECHANGE, 0);
		}

		raiseTabEvent( "onTabFocus", nOldTab, m_nCurrentTab );
    }

    if (  m_arTabs[index].m_strAction.length() > 0 && (m_arTabs[index].m_bReloadPage || bNewTab) )
    {
        LOG(INFO) + "loadUrl";
        RHODESAPP().loadUrl( m_arTabs[index].m_strAction, index );
    }
    
}

void CNativeTabbar::SetBadge(int index, const char* badge)
{
    //Not implemented
}

int CNativeTabbar::GetCurrentTabID()
{
    if (m_arTabs.size()>0 && m_nCurrentTab < (int)m_arTabs.size() && m_nCurrentTab >= 0 )
	    return m_arTabs[m_nCurrentTab].m_nTabID;

    return 0;
}

int CNativeTabbar::GetTabID(int nIndex)
{
    if ( nIndex < 0 )
        nIndex = m_nCurrentTab;

    if (m_arTabs.size()>0 && nIndex < (int)m_arTabs.size() && nIndex >= 0 )
	    return m_arTabs[nIndex].m_nTabID;

    return 0;
}

int CNativeTabbar::GetCurrentTabIndex()
{
    if (m_arTabs.size()>0 && m_nCurrentTab < (int)m_arTabs.size() && m_nCurrentTab >= 0 )
	    return m_nCurrentTab;

    return 0;
}

bool CNativeTabbar::IsTabBarStarted()
{
    return m_bTabCreated;
}
