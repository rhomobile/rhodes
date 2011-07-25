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

#ifndef __APP_MENU__H__
#define __APP_MENU__H__

#define APP_MENU_ITEMS_MAX 32 //maximus number of items app menu can contain

#ifdef __cplusplus

#include "logging/RhoLog.h"

namespace rho {
namespace common {

struct CAppMenuItem 
{
    enum EnumMenuType { emtNone = 0, emtSeparator, emtUrl, emtRefresh, emtHome, emtBack, emtSync, emtOptions, emtLog, emtExit,
        emtClose, emtFullscreen };
	String m_strLabel;
	String m_strLink;
	EnumMenuType  m_eType;

    CAppMenuItem (const String &label, const String &link);
    boolean processCommand();
};

class CAppMenu
{
	DEFINE_LOGCLASS;

    common::CMutex m_mxAppMenu;
	Vector<CAppMenuItem> m_arAppMenuItems;

public:
    void setAppMenu(unsigned long valMenu);
    void copyMenuItems(Vector<CAppMenuItem>& arAppMenuItems);

//private:
    void addAppMenuItem( const String& strLabel, const String& strLink );

};

} //namespace common
} //namespace rho

#endif //__cplusplus

#endif //__WEB_VIEW__H__
