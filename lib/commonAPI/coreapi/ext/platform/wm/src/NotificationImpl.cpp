#include "../../../shared/generated/cpp/NotificationBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"

extern "C" void alert_show_popup_ex(const rho::Hashtable<rho::String, rho::String>& propertyMap);
extern "C" void alert_hide_popup();
extern "C" void alert_show_status(const char* szTitle, const char* szMessage, const char* szHide);
extern "C" void alert_vibrate(int duration_ms);

namespace rho {

using namespace apiGenerator;
using namespace common;

class CNotificationImpl: public CNotificationSingletonBase
{
public:
	CNotificationImpl(): CNotificationSingletonBase() {}

    virtual void showPopup(const Hashtable<String, String>& propertyMap, CMethodResult& oResult)
	{
		alert_show_popup_ex(propertyMap);
	}

    virtual void hidePopup(CMethodResult& oResult)
	{
		alert_hide_popup();
	}

    virtual void showStatus(const String& title, const String& status_text, const String& hide_button_label, CMethodResult& oResult)
	{
		alert_show_status(title.c_str(), status_text.c_str(), hide_button_label.c_str());
	}

    virtual void beep(const Hashtable<String, String>& propertyMap, CMethodResult& oResult)
	{
		// TODO: implement
	}

    virtual void vibrate(int duration, CMethodResult& oResult)
	{
		alert_vibrate(duration);
	}
};

class CNotificationFactory: public CNotificationFactoryBase
{
public:
    ~CNotificationFactory(){}

    INotificationSingleton* createModuleSingleton()
    { 
        return new CNotificationImpl(); 
    }
};

}

extern "C" void Init_Notification()
{
    rho::CNotificationFactory::setInstance( new rho::CNotificationFactory() );
    rho::Init_Notification_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoNotificationApi");

}