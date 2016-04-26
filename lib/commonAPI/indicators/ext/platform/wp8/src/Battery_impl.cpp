#include "../../../shared/generated/cpp/BatteryBase.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Battery"


#include <windows.h>

using namespace Windows::Graphics::Display;
using namespace Windows::Phone::Devices::Power;

#include "common/RhodesApp.h"
#include <cmath>
#include <algorithm>
#include <sstream>

namespace rho {
using namespace apiGenerator;
using namespace common;

Battery^ b;  //  The devices Battery interface
rho::apiGenerator::CMethodResult m_batteryCallback;
	void callBatteryCallback(rho::apiGenerator::CMethodResult& oResult, int iRemainingCharge, long long lRemainingTime)
	{
		rho::Hashtable<rho::String, rho::String> batteryData;
		if (lRemainingTime == (pow(2.0, 63) - 1))
			batteryData.put("acLineStatus", "true");
		else
			batteryData.put("acLineStatus", "false");
		std::stringstream s;
		s << iRemainingCharge;
		batteryData.put("batteryLifePercent", s.str());
		batteryData.put("backupBatteryLifePercent", "undefined");
		batteryData.put("trigger", "undefined");
		batteryData.put("batteryLifeKnown", "undefined");
		batteryData.put("backupBatteryLifeKnown", "undefined");

		oResult.set(batteryData);
	}

class CBatteryImpl: public CBatterySingletonBase
{
public:

//  Class to receive battery callbacks must be a ref type
ref class CBatteryController
{
public:
	void OnRemainingChargePercentChanged(Platform::Object^ sender, Platform::Object^ e)
	{
		//  Handle Battery Percentage Change
		long long remainingTime = b->RemainingDischargeTime.Duration;
		long long lRemaingMinutes = (b->RemainingDischargeTime.Duration / 10000000) / 60;
		bool bAcPower = false;
		if (remainingTime == (pow(2.0, 63) - 1))
		{
			//  Device is on AC power
			bAcPower = true;
		}
		LOG(INFO) + "Battery Callback Called.  Percent Remaining: " + b->RemainingChargePercent + ". Remaining Minutes: " + lRemaingMinutes;
		callBatteryCallback(m_batteryCallback, b->RemainingChargePercent, b->RemainingDischargeTime.Duration);
	}

};

private:
	bool m_bHasRegistrar;
	CBatteryController^ batController;
    Windows::Foundation::EventHandler<Platform::Object^>^ handler;
	Windows::Foundation::EventRegistrationToken m_token;
public:
	CBatteryImpl(): CBatterySingletonBase() 
	{
		batController = ref new CBatteryController();
		handler = ref new Windows::Foundation::EventHandler<Platform::Object^>(batController, &CBatteryController::OnRemainingChargePercentChanged);
		b = Battery::GetDefault();
		m_bHasRegistrar = false;
	}

	void getRefreshInterval(rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(WARNING) + "Refresh Interval is not supported on WP8 or WE8H";
	}
    void setRefreshInterval( int refreshInterval, rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(WARNING) + "Refresh Interval is not supported on WP8 or WE8H";
	}

	virtual void batteryStatus( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) 
	{
		rho::String comparitor;
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			comparitor.assign(iterator->first);
			std::transform(comparitor.begin(), comparitor.end(), comparitor.begin(), ::tolower);
			if (comparitor.compare("trigger") == 0)
				LOG(INFO) + "Trigger property to battery Status will always be 'system' for WP8 or WE8H";
			else if (comparitor.compare("refreshInterval") == 0)
				LOG(WARNING) + "Refresh Interval parameter ignored, WP8 and WE8H use system events";
			else
				LOG(WARNING) + "Unrecognised parameter passed to batteryStatus: " + iterator->first;
		}

		//  Refresh interval is set earlier
		if (oResult.hasCallback())
		{
			m_batteryCallback = oResult;
			if (!m_bHasRegistrar)
			{
				m_bHasRegistrar = true;
				//b->RemainingChargePercent;
				m_token = b->RemainingChargePercentChanged += handler;
				//b->RemainingChargePercentChanged += &rho::CBatteryImpl::OnRemainingChargePercentChanged;
				//LOG(INFO) + "Remaining Charge Percent: " + b->RemainingChargePercent;
			}
			else
				LOG(WARNING) + "There is already a status event defined for the battery";
			callBatteryCallback(m_batteryCallback, b->RemainingChargePercent, b->RemainingDischargeTime.Duration);
		}
		else
		{
			//  Synchronous Return
			callBatteryCallback(m_batteryCallback, b->RemainingChargePercent, b->RemainingDischargeTime.Duration);
		}
	}

	virtual void stopBatteryStatus(rho::apiGenerator::CMethodResult& oResult)
	{
		if (m_bHasRegistrar)
		{
			m_bHasRegistrar = false;
			b->RemainingChargePercentChanged -= m_token;
		}
		else
		{
			LOG(WARNING) + "Unable to stop battery status notifications as they have not been started";
		}
	}

	virtual void smartBatteryStatus(rho::apiGenerator::CMethodResult& oResult) 
	{
		LOG(WARNING) + "Smart Battery is not supported on WP8 or WE8H";
	}

	virtual void showIcon( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) 
	{
		LOG(WARNING) + "The Battery icon is not supported on WP8 or WE8H";
	}

	virtual void hideIcon(rho::apiGenerator::CMethodResult& oResult) 
	{
		LOG(WARNING) + "The Battery icon is not supported on WP8 or WE8H";
	}

	/////////////////////////////////////////////////////
	//  MC18 Specific Calls
	/////////////////////////////////////////////////////

	virtual void getTripDuration(rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(WARNING) + "The tripDuration property is not supported on WP8 or WE8H";
	}
	virtual void setTripDuration( int tripDuration, rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(WARNING) + "The tripDuration property is not supported on WP8 or WE8H";
	}
	virtual void getAverageCurrentConsumption(rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(WARNING) + "The averageCurrentConsumption property is not supported on WP8 or WE8H";
	}
	void setAverageCurrentConsumption( int averageCurrentConsumption, rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(WARNING) + "The averageCurrentConsumption property is not supported on WP8 or WE8H";
	}
	virtual void batteryDiagnostics(rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(WARNING) + "The batteryDiagnostics method is not supported on WP8 or WE8H";
	}

};

class CBatterySingleton: public CBatterySingletonBase
{
    ~CBatterySingleton(){}
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
    rho::CBatteryFactory::setInstance( new CBatteryFactory() );
    rho::Init_Battery_API();
	//RHODESAPP().getExtManager().requireRubyFile("REBatteryApi");
}


}
