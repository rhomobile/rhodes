#include "../../../shared/generated/cpp/BatteryBase.h"
#include "Indicator.h"
#include "common/RhodesApp.h"
#include <algorithm>

namespace rho {
using namespace apiGenerator;
using namespace common;

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Battery"

class CBatteryImpl: public CBatterySingletonBase
{
private:
	CBattery* m_pBattery;
	CRhoExtData rhodesData;
	bool m_bBatteryStatusRegistrar;
public:
    CBatteryImpl(): CBatterySingletonBase() 
	{
		m_pBattery = NULL;
		m_bBatteryStatusRegistrar = false;
		rhodesData = RHODESAPP().getExtManager().makeExtData();
	}

	void createObject()
	{
		m_pBattery = new CBattery((HINSTANCE)rhodesData.m_hInstance, rhodesData.m_hBrowserWnd);		
	}
	virtual void getRefreshInterval(rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pBattery)
			createObject();

		oResult.set((int)m_pBattery->GetInterval());
	}
    virtual void setRefreshInterval( int refreshInterval, rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pBattery)
			createObject();

		m_pBattery->SetInterval(refreshInterval);	
	}

	virtual void batteryStatus( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pBattery)
			createObject();

		rho::String comparitor;
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			comparitor.assign(iterator->first);
			std::transform(comparitor.begin(), comparitor.end(), comparitor.begin(), ::tolower);
			if (comparitor.compare("trigger") == 0)
				m_pBattery->SetTrigger(iterator->second);
			else if (comparitor.compare("refreshinterval") == 0)
				m_pBattery->SetInterval(atoi(iterator->second.c_str()));
			else
				LOG(WARNING) + "Unrecognised parameter passed to batteryStatus: " + iterator->first;
		}

		//  Refresh interval is set earlier
		if ((oResult.hasCallback()) && (m_pBattery->GetInterval()))
		{
			m_pBattery->SetCallback(oResult);
			if (!m_bBatteryStatusRegistrar)
			{
				m_pBattery->AddListener();
				m_bBatteryStatusRegistrar = true;
			}
			m_pBattery->Refresh(oResult);
		}
		else
		{
			//  Synchronous Return
			m_pBattery->Refresh(oResult);
		}
	}

	virtual void stopBatteryStatus(rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pBattery)
			createObject();
		rho::apiGenerator::CMethodResult blank;
		m_pBattery->SetCallback(blank);
		m_pBattery->RemoveListener();
		m_bBatteryStatusRegistrar = false;
	}

	virtual void smartBatteryStatus(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pBattery)
			createObject();
		//  This method can only be called synchronously
		m_pBattery->GetSmartBatteryStatus(oResult);
	}

	virtual void showIcon( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pBattery)
			createObject();
		m_pBattery->IconInitialization((HINSTANCE)rhodesData.m_hInstance, rhodesData.m_hBrowserWnd);
	
		rho::String comparitor;
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			comparitor.assign(iterator->first);
			std::transform(comparitor.begin(), comparitor.end(), comparitor.begin(), ::tolower);
			if (comparitor.compare("left") == 0)
				m_pBattery->SetLeft(atoi(iterator->second.c_str()));
			else if (comparitor.compare("top") == 0)
				m_pBattery->SetTop(atoi(iterator->second.c_str()));
			else if (comparitor.compare("layout") == 0)
				m_pBattery->SetLayout(convertToStringW(iterator->second).c_str());
			else if (comparitor.compare("color") == 0)
				m_pBattery->SetColour(convertToStringW(iterator->second).c_str());
			else if (comparitor.compare("refreshinterval") == 0)
				m_pBattery->SetInterval(atoi(iterator->second.c_str()));
			else
				LOG(WARNING) + "Unrecognised parameter passed to showIcon (Battery): " + iterator->first;
		}

		if (m_pBattery->GetIconShown())
		{
			LOG(INFO) + "Did not show icon as it is already shown";
		}
		else
		{
			m_pBattery->Show();
			m_pBattery->AddListener();
		}
		oResult.set("true");	
	}

	virtual void hideIcon(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pBattery)
			createObject();

		if (!m_pBattery->GetIconShown())
		{
			LOG(INFO) + "Did not hide icon as it is already hidden";
		}
		else
		{
			m_pBattery->Hide();
			m_pBattery->RemoveListener();
		}

		oResult.set("true");	
	}

	/////////////////////////////////////////////////////
	//  MC18 Specific Calls
	/////////////////////////////////////////////////////

	virtual void getTripDuration(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pBattery)
			createObject();
		oResult.set((int)m_pBattery->GetTripDuration());
	}
	virtual void setTripDuration( int tripDuration, rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pBattery)
			createObject();
		if (tripDuration >= 0)
			m_pBattery->SetTripDuration(tripDuration);
		else
			LOG(WARNING) + "Trip Duration must be positive.  Attempted to set to " + tripDuration;
		oResult.set(true);
	}
	virtual void getAverageCurrentConsumption(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pBattery)
			createObject();
		oResult.set((int)m_pBattery->GetAverageCurrentConsumption());
	}
	void setAverageCurrentConsumption( int averageCurrentConsumption, rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pBattery)
			createObject();
		if (averageCurrentConsumption >= 0)
			m_pBattery->SetAverageCurrentConsumption(averageCurrentConsumption);
		else
			LOG(WARNING) + "Average Current Consumption must be positive.  Attempted to set to " + averageCurrentConsumption;
		oResult.set(true);
	}
	virtual void batteryDiagnostics(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pBattery)
			createObject();
		m_pBattery->GetBatteryDiagnostics(oResult);
	}


};

class CBatterySingleton: public CBatterySingletonBase
{
    ~CBatterySingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CBatteryFactory: public CBatteryFactoryBase
{
    ~CBatteryFactory(){}
    virtual IBatterySingleton* createModuleSingleton()
	{
		return new CBatteryImpl();
	}
};

extern "C" void Init_Battery_extension()
{
    CBatteryFactory::setInstance( new CBatteryFactory() );
    Init_Battery_API();
}


}
