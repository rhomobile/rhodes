#ifndef __APP_MENU__H__
#define __APP_MENU__H__

#define APP_MENU_ITEMS_MAX 32 //maximus number of items app menu can contain

#ifdef __cplusplus

#include "logging/RhoLog.h"

namespace rho {
namespace common {

class CAppMenu
{
	DEFINE_LOGCLASS;

public:
	enum 
	{
		MENU_TYPE_SYSTEM,
		MENU_TYPE_CUSTOM
	};
	
	class Item {
	public:
		enum 
		{
			TYPE_UNKNOWN          = 0,
			TYPE_SEPARATOR,
			TYPE_URL,
			TYPE_CMD_REFRESH,
			TYPE_CMD_HOME,
			TYPE_CMD_BACK,
			TYPE_CMD_SYNC,
			TYPE_CMD_OPTIONS,
			TYPE_CMD_LOG,
			TYPE_CMD_EXIT,
			TYPE_CMD_CLOSE
		};

		Item () : m_id(-1) {};
		Item (const String &label, const String &link);
		Item (const String &label, const String &link, int id);
		Item (const Item &i) : m_label(i.m_label),  m_link(i.m_link), 
								m_type(i.m_type), m_id(i.m_id) {}
		
		~Item () {}

		const String& getLabel () { return m_label; }
		const String& getLink  () { return m_link;  }
		int           getType  () { return m_type;  }
		int   		  getId    () { return m_id;    }
		
		void setId (int id) {m_id = id;}
				
	private:
		String m_label;
		String m_link;
		int m_type;
		int m_id;
	};

public:
	CAppMenu  ();
	~CAppMenu ();

	int  getMenuType (void) { return m_type; }
	bool setMenuType (int type);
	
	bool getItems (Vector<Item> &items);
	bool setItems (Vector<Item> &items);
	
	bool getItem (int id, Item &item);
	bool getItem (String &label, Item &item);
	
	
	void addItem (const String &label, const String &link);
	void removeAllItems (void);
		
private:
	Vector<Item> m_customMenuItems;
	int m_type;
};

} //namespace common
} //namespace rho

#endif //__cplusplus

#endif //__WEB_VIEW__H__
