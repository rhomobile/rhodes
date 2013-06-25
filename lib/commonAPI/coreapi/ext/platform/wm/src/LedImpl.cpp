#include <common/RhodesApp.h>
#include "../../../shared/generated/cpp/LedBase.h"
#include "NotificationLoader.h"
namespace rho {
	using namespace apiGenerator;
	using namespace common;
	using namespace notification;

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Led"

#define NOTIFY_LED_DEFAULT_LED_ON_DURATION 1000
#define NOTIFY_LED_DEFAULT_LED_OFF_DURATION 1000
#define NOTIFY_LED_DEFAULT_LED_NUMBER_CYCLES 1

class CLedImpl: public IRhoExtension, public CLedBase
{
public: 
	CLedImpl(const rho::String& strID): CLedBase()
    {
		RHODESAPP().getExtManager().registerExtension(("NFYLED" + strID), this );
		m_bAppHasFocus = true;
		m_iID = atoi(strID.c_str());
		m_bNotificationsLoaded = FALSE;
		m_pNotifications = new CNotificationLoader();
		m_eCurrentState = -1;
		if (m_pNotifications)
			m_bNotificationsLoaded = m_pNotifications->LoadNotificationDLL();
		if (!m_pNotifications || !m_bNotificationsLoaded)
			LOG(ERROR) + "Notification LED API is unavailable on this device, library could not be loaded";
		//  The id is the index in the vector of notification objects available in the EMDK
		//  find our index
		if (m_pNotifications && m_bNotificationsLoaded)
		{
			NOTIFY_FINDINFO notifyObject;
			HANDLE	hFindHandle = NULL;
			SI_ALLOC_ALL(&notifyObject);
			notifyObject.StructInfo.dwUsed = 0;
			if(m_pNotifications->lpfn_Notify_FindFirst(&notifyObject, &hFindHandle) == E_NTFY_SUCCESS)
			{
				int iObCount = 0;
				do 
				{
					if(iObCount == m_iID)
					{
						//  This is our LED
						m_szName = new WCHAR[wcslen(notifyObject.szObjectName) + 1];
						wcscpy(m_szName, notifyObject.szObjectName);
						break;
					}
					iObCount++;
					SI_ALLOC_ALL(&notifyObject);
					notifyObject.StructInfo.dwUsed = 0;
				} while(m_pNotifications->lpfn_Notify_FindNext(&notifyObject, hFindHandle) == E_NTFY_SUCCESS);
			}
			m_pNotifications->lpfn_Notify_FindClose(hFindHandle);
		}
    }
	   
	~CLedImpl()
	{
		delete m_szName;
	}

	void getName(rho::apiGenerator::CMethodResult& oResult)
	{oResult.set(m_szName);}

    void illuminate(rho::apiGenerator::CMethodResult& oResult)
	{
		m_eCurrentState = NOTIFY_STATE_ON;
		if (m_bAppHasFocus)
		{
			DWORD dwResult = m_pNotifications->lpfn_Notify_SetState(m_iID, NOTIFY_STATE_ON);
			if (dwResult != E_NTFY_SUCCESS)
				LOG(WARNING) + "Unable to illuminate LED " + m_szName + " due to error " + dwResult;
		}
	}

    void extinguish(rho::apiGenerator::CMethodResult& oResult)
	{
		m_eCurrentState = NOTIFY_STATE_OFF;	
		if (m_bAppHasFocus)
		{
			DWORD dwResult = m_pNotifications->lpfn_Notify_SetState(m_iID, NOTIFY_STATE_OFF);
			if (dwResult != E_NTFY_SUCCESS)
				LOG(WARNING) + "Unable to extinguish LED " + m_szName + " due to error " + dwResult;
		}
	}

    void flash( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{
		m_eCurrentState = NOTIFY_STATE_CYCLE;
		CYCLE_INFO cycleInfo;
		SI_INIT(&cycleInfo);
		DWORD dwResult = m_pNotifications->lpfn_Notify_GetCycleInfo(m_iID, &cycleInfo);
		if (dwResult != E_NTFY_SUCCESS)
		{
			LOG(WARNING) + "Unable to flash LED " + m_szName + " due to error " + dwResult;
			return;
		}
		//  Set Defaults
		cycleInfo.ObjectTypeSpecific.LedSpecific.dwOnDuration = NOTIFY_LED_DEFAULT_LED_ON_DURATION;
		cycleInfo.ObjectTypeSpecific.LedSpecific.dwOffDuration = NOTIFY_LED_DEFAULT_LED_OFF_DURATION;
		cycleInfo.ObjectTypeSpecific.LedSpecific.dwCount = NOTIFY_LED_DEFAULT_LED_NUMBER_CYCLES;
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			DEBUGMSG(TRUE, (L"Setting LED Flash Property %s to value %s\n", convertToStringW(iterator->first).c_str(), convertToStringW(iterator->second).c_str()));
			if (_stricmp("onDuration", iterator->first.c_str()) == 0)
				cycleInfo.ObjectTypeSpecific.LedSpecific.dwOnDuration = atoi(iterator->second.c_str());
			else if (_stricmp("offDuration", iterator->first.c_str()) == 0)
				cycleInfo.ObjectTypeSpecific.LedSpecific.dwOffDuration = atoi(iterator->second.c_str());
			else if (_stricmp("numberOfCycles", iterator->first.c_str()) == 0)
				cycleInfo.ObjectTypeSpecific.LedSpecific.dwCount = atoi(iterator->second.c_str());
		}
		//  Remember the Cycle information in case we lose focus.
		memcpy(&mCycleInfoStore, &cycleInfo, sizeof(CYCLE_INFO));
		if (m_bAppHasFocus)
		{
			DWORD dwResult = m_pNotifications->lpfn_Notify_SetCycleInfo(m_iID, &cycleInfo);
			if (dwResult == E_NTFY_SUCCESS)
				dwResult = m_pNotifications->lpfn_Notify_SetState(m_iID, NOTIFY_STATE_CYCLE); 
			if (dwResult != E_NTFY_SUCCESS)
				LOG(WARNING) + "Unable to flash LED " + m_szName + " due to error " + dwResult;
		}
	}
	//  Virtual Methods overridden from IRhoExtension
	void OnAppActivate(bool bActivate, const CRhoExtData& oExtData)
	{
		//  We have either gone to the background or the foreground
		if (bActivate && !m_bAppHasFocus)
		{
			//  We have gained focus whereas we previously did not have focus
			m_bAppHasFocus = true;
			//  If the LED was previously cycling or had no state then take no action.
			//  Turn the LED on / off depending on its previous state
			rho::apiGenerator::CMethodResult oResult;
			if (m_eCurrentState == NOTIFY_STATE_ON)
			{
				LOG(INFO) + "Application has gained state, illuminating LED " + m_szName + " as that was the previous state";
				illuminate(oResult);
			}
			else if (m_eCurrentState == NOTIFY_STATE_OFF)
			{
				LOG(INFO) + "Application has gained state, extinguishing LED " + m_szName + " as that was the previous state";
				extinguish(oResult);
			}
		}
		else if (!bActivate)
		{
			//  We have lost focus
			m_bAppHasFocus = false;
		}
	}

private:
	int m_iID;
	WCHAR* m_szName;
	int m_eCurrentState;
	CNotificationLoader* m_pNotifications;
	BOOL m_bNotificationsLoaded;
	bool m_bAppHasFocus;
	CYCLE_INFO mCycleInfoStore;
};

class CLedSingleton: public CLedSingletonBase
{
public:
	CLedSingleton(): CLedSingletonBase()
	{
		m_bNotificationsLoaded = FALSE;
		m_pNotifications = new CNotificationLoader();
		if (m_pNotifications)
			m_bNotificationsLoaded = m_pNotifications->LoadNotificationDLL();
		if (!m_pNotifications || !m_bNotificationsLoaded)
			LOG(ERROR) + "Notification LED API is unavailable on this device, library could not be loaded";
	}
	~CLedSingleton()
	{
		if (m_pNotifications)
			delete m_pNotifications;
		m_bNotificationsLoaded = FALSE;
	}
    void enumerate(rho::apiGenerator::CMethodResult& oResult);
private:
	CNotificationLoader* m_pNotifications;
	BOOL m_bNotificationsLoaded;
};

class CLedFactory: public CLedFactoryBase
{
	~CLedFactory(){}
    virtual ILedSingleton* createModuleSingleton();
    virtual ILed* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Led_extension()
{
    CLedFactory::setInstance( new CLedFactory() );
    Init_Led_API();
}

ILed* CLedFactory::createModuleByID(const rho::String& strID)
{
    return new CLedImpl(strID);
}

ILedSingleton* CLedFactory::createModuleSingleton()
{
    CLedSingleton* pLedSingleton = new CLedSingleton();
	return pLedSingleton;
}

void CLedSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
	if (m_pNotifications && m_bNotificationsLoaded)
	{
		//  Initialise the LEDs
		NOTIFY_FINDINFO notifyObject;
		HANDLE	hFindHandle = NULL;
		SI_ALLOC_ALL(&notifyObject);
		notifyObject.StructInfo.dwUsed = 0;
		if(m_pNotifications->lpfn_Notify_FindFirst(&notifyObject, &hFindHandle) == E_NTFY_SUCCESS)
		{
			int iObCount = 0;
			do 
			{
				if(notifyObject.dwObjectType == NOTIFY_TYPE_LED)
				{
					//  Found an LED
					arIDs.addElement(convertToStringA(iObCount));
				}
				iObCount++;
				SI_ALLOC_ALL(&notifyObject);
				notifyObject.StructInfo.dwUsed = 0;
			} while(m_pNotifications->lpfn_Notify_FindNext(&notifyObject, hFindHandle) == E_NTFY_SUCCESS);
		}
		m_pNotifications->lpfn_Notify_FindClose(hFindHandle);
	}	
	oResult.set(arIDs);
}


extern "C" void Init_Led()
{
    rho::CLedFactory::setInstance( new rho::CLedFactory() );
    rho::Init_Led_API();
}


}