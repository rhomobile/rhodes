#include "AppMenu.h"

namespace rho {
namespace common{

IMPLEMENT_LOGCLASS(CAppMenu, "AppMenu");

CAppMenu::Item::Item (const String &label, const String &link)  : m_id (-1)
{
	m_label = label;
	m_link  = link;
	
	//TODO: table
	if (label == "separator") {
		m_type = TYPE_SEPARATOR;
	} else if (link == "home") {
		m_type = TYPE_CMD_HOME;
	} else if (link == "refresh") {
		m_type = TYPE_CMD_REFRESH;
	} else if (link == "options") {
		m_type = TYPE_CMD_OPTIONS;
	} else if (link == "log") {
		m_type = TYPE_CMD_LOG;
	} else if (link == "sync") {
		m_type = TYPE_CMD_SYNC;
	} else if (link == "close") {
		m_type = TYPE_CMD_CLOSE;
	} else if (link == "exit") {
		m_type = TYPE_CMD_EXIT;
	} else {
		m_type = TYPE_URL;
	}
}

CAppMenu::Item::Item (const String &label, const String &link, int id)
{
	m_label = label;
	m_link  = link;
	m_id    = id;
	
	//TODO: table
	if (label == "separator") {
		m_type = TYPE_SEPARATOR;
	} else if (link == "home") {
		m_type = TYPE_CMD_HOME;
	} else if (link == "refresh") {
		m_type = TYPE_CMD_REFRESH;
	} else if (link == "options") {
		m_type = TYPE_CMD_OPTIONS;
	} else if (link == "log") {
		m_type = TYPE_CMD_LOG;
	} else if (link == "sync") {
		m_type = TYPE_CMD_SYNC;
	} else if (link == "close") {
		m_type = TYPE_CMD_CLOSE;
	} else if (link == "exit") {
		m_type = TYPE_CMD_EXIT;
	} else {
		m_type = TYPE_URL;
	}
}

CAppMenu::CAppMenu () 
{
	m_type = MENU_TYPE_SYSTEM;
}

CAppMenu::~CAppMenu () {}

bool CAppMenu::setMenuType (int type)
{	
	bool ret = true;
	
	switch (type) {
	case MENU_TYPE_SYSTEM:
		removeAllItems();
		m_type = MENU_TYPE_SYSTEM;
		break;
	case MENU_TYPE_CUSTOM:
		removeAllItems();
		if (m_customMenuItems.size() > 0)
			m_type = MENU_TYPE_CUSTOM;	
		else
			//there are no items to load
			ret = false;
		break;
	default:
		LOG(ERROR) + "invalid menu type";
		ret = false;
		break;
	}
	
	return ret;
}	

bool CAppMenu::getItems (Vector<CAppMenu::Item> &items)
{
	if (m_type == MENU_TYPE_CUSTOM) {
		items = m_customMenuItems;
		return true;
	}
	return false;
}

bool CAppMenu::setItems (Vector<CAppMenu::Item> &items)
{
	if (m_type == MENU_TYPE_CUSTOM) {
		m_customMenuItems = items;
		return true;
	}
	return false;
}

bool CAppMenu::getItem (int id, CAppMenu::Item &item) 
{
	for (Vector<Item>::iterator itr = m_customMenuItems.begin(); 
		itr != m_customMenuItems.end(); ++itr) 
	{
		if ((itr)->getId() == id) {
			item = *itr;
			return true;
		}
	}
	return false;
}

bool CAppMenu::getItem (String &link, CAppMenu::Item &item) 
{
	for (Vector<Item>::iterator itr = m_customMenuItems.begin(); 
		itr != m_customMenuItems.end(); ++itr) 
	{
		if ((itr)->getLink() == link) {
			item = *itr;
			return true;
		}
	}
	return false;
}

void CAppMenu::addItem (const String &label, const String &link)
{
	m_type = MENU_TYPE_CUSTOM;
	m_customMenuItems.push_back(Item(label, link));
}

void CAppMenu::removeAllItems (void)
{
	m_customMenuItems.clear();
}

} //namespace common
} //namespace rho
