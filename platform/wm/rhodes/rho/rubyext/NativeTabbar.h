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

#pragma once

#include "common/RhodesApp.h"

using namespace rho;

class CNativeTabbar
{
    DEFINE_LOGCLASS;

    int  m_nCurrentTab;
    bool m_bTabCreated;
    rho::apiGenerator::CMethodResult m_oCallback;

    struct CTabBarItem
    {
        String m_strAction;
        String m_strLabel;
        bool m_bUseCurrentViewForTab;
        bool m_bReloadPage;
        bool m_bPerishable;
        HWND m_hwndTab;
        int m_nTabID;

        CTabBarItem(const String& strAction, const String& strLabel, bool bUseCurrentViewForTab, bool bReloadPage, bool bPerishable ): m_bUseCurrentViewForTab(bUseCurrentViewForTab), m_bReloadPage(bReloadPage), 
            m_strAction(strAction), m_strLabel(strLabel), m_hwndTab(0), m_nTabID(-1), m_bPerishable(bPerishable){}
    };

    Vector<CTabBarItem> m_arTabs;
    String m_strStartTabName;

public:

    static const int  TABBAR_TIMER_ID = 111;

    CNativeTabbar();

    void CreateTabbarEx(const rho::Vector<rho::String>& tabbarElements, const rho::Hashtable<rho::String, rho::String>& tabBarProperties, rho::apiGenerator::CMethodResult& oResult);
    void RemoveTabbar();
    void RemoveTab(int index);
    void SwitchTab(int index);
    void SetBadge(int index, const char* badge);
    int  GetCurrentTabIndex();
    bool IsTabBarStarted();
    void SwitchTabByName(const char* szTabName, bool bExecuteJS);
    void SetStartTabName(const String& strTabName){m_strStartTabName = strTabName;}

    LRESULT OnTimer (UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

    void raiseTabEvent( const char* szEventName, int nOldTab, int nNewTab );
};
