#include "AppMenu.h"
#include "common/RhodesApp.h"
#include "ruby/ext/rho/rhoruby.h"

namespace rho {
namespace common{

IMPLEMENT_LOGCLASS(CAppMenu, "AppMenu");

void CAppMenu::addAppMenuItem( const String& strLabel, const String& strLink )
{
    if ( strLabel.length() == 0 )
        return;

	m_arAppMenuItems.push_back(CAppMenuItem(strLabel, strLink));
    if ( strcasecmp( strLabel.c_str(), "back" )==0 && strcasecmp( strLink.c_str(), "back" )!=0 )
        RHODESAPP().setAppBackUrl(strLink);
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
	else
		m_eType = emtUrl;
}

} //namespace common
} //namespace rho
