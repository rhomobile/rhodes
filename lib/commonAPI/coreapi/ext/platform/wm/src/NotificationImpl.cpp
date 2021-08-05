#include "../../../shared/generated/cpp/NotificationBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "NotificationLoader.h"
#include <algorithm>

extern "C" void alert_show_popup_ex(const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
extern "C" void alert_hide_popup();
extern "C" void alert_show_status_ex(const char* szTitle, const char* szMessage, const char* szHide, rho::apiGenerator::CMethodResult& oResult);
extern "C" void alert_vibrate(int duration_ms);
extern "C" void alert_play_file(char* file_name, char* media_type);

#define NOTIFICATIONS_DEFAULT_FREQUENCY 2000
#define NOTIFICATIONS_DEFAULT_VOLUME 1
#define NOTIFICATIONS_DEFAULT_DURATION 1000

namespace rho {

using namespace apiGenerator;
using namespace common;

class CNotificationImpl: public CNotificationSingletonBase
{
public:
	CNotificationImpl(): CNotificationSingletonBase() 
	{
#if defined(OS_WINCE)
		m_bNotificationsLoaded = FALSE;
		m_pNotifications = new CNotificationLoader();
		if (m_pNotifications)
			m_bNotificationsLoaded = m_pNotifications->LoadNotificationDLL();
		if (!m_pNotifications || !m_bNotificationsLoaded)
			LOG(INFO) + "SYMBOL specific notification functionality is not available on this device";
#endif
	}

    virtual void showPopup(const Hashtable<String, String>& propertyMap, CMethodResult& oResult)
	{
		alert_show_popup_ex(propertyMap, oResult);
	}

    virtual void hidePopup(CMethodResult& oResult)
	{
		alert_hide_popup();
	}

    virtual void showStatus(const String& title, const String& status_text, const String& hide_button_label, CMethodResult& oResult)
	{
		alert_show_status_ex(title.c_str(), status_text.c_str(), hide_button_label.c_str(), oResult);
	}

	virtual void playFile(const String& path, const String& media_type, CMethodResult& oResult)
	{
		alert_play_file((char*)path.c_str(), (char*)media_type.c_str());
	}

    virtual void beep(const Hashtable<String, String>& propertyMap, CMethodResult& oResult)
	{
#if defined(OS_WINCE)
		int iFrequency = NOTIFICATIONS_DEFAULT_FREQUENCY;
		int iVolume = NOTIFICATIONS_DEFAULT_VOLUME;
		int iDuration = NOTIFICATIONS_DEFAULT_DURATION;
		rho::String comparitor;
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			comparitor.assign(iterator->first);
			std::transform(comparitor.begin(), comparitor.end(), comparitor.begin(), ::tolower);
			if (comparitor.compare("frequency") == 0)
				iFrequency = atoi(iterator->second.c_str());
			else if (comparitor.compare("volume") == 0)
				iVolume = atoi(iterator->second.c_str());
			else if (comparitor.compare("duration") == 0)
				iDuration = atoi(iterator->second.c_str());
			else
				LOG(WARNING) + "Unrecognised parameter passed to beep: " + iterator->first;
		}
		BOOL bBeepSuccess = FALSE;
		if (m_pNotifications)
			bBeepSuccess = m_pNotifications->Beep(iFrequency, iVolume, iDuration);
		if (!bBeepSuccess)
			LOG(WARNING) + "Unable to use the device beeper";
#endif
	}

    virtual void vibrate(int duration, CMethodResult& oResult)
	{
#if defined(OS_WINCE)
		BOOL bVibrateSuccess = FALSE;
		if (m_pNotifications)
			bVibrateSuccess = m_pNotifications->Vibrate(duration);
		if (!bVibrateSuccess)
			LOG(WARNING) + "Unable to initiate the device vibrate function";
#else
		//  This call is having no effect on Symbol Devices, they use Ntfy API (directly above)
		alert_vibrate(duration);
#endif

	}

	virtual void setScheduler( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult){

	}
    virtual void removeScheduler(rho::apiGenerator::CMethodResult& oResult){
    	
    }

private:
#if defined(OS_WINCE)
	CNotificationLoader* m_pNotifications;
	BOOL m_bNotificationsLoaded;
#endif
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
