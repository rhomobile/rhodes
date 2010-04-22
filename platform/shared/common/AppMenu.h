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
        emtClose };
	String m_strLabel;
	String m_strLink;
	EnumMenuType  m_eType;

    CAppMenuItem (const String &label, const String &link);
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
