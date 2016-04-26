#include "../../../shared/generated/cpp/SignalIndicatorsBase.h"
#include "Indicator.h"
#include "common/RhodesApp.h"
#include <algorithm>

namespace rho {

using namespace apiGenerator;
using namespace common;

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Signal"
class CSignalIndicatorsImpl: public CSignalIndicatorsSingletonBase
{
private:
	CSignalIndicators* m_pSignal;
	CRhoExtData rhodesData;
	int m_bSignalStatusRegistrar;
public:
    CSignalIndicatorsImpl(): CSignalIndicatorsSingletonBase()
	{
		m_pSignal = NULL;
		m_bSignalStatusRegistrar = false;
		rhodesData = RHODESAPP().getExtManager().makeExtData();
	}
	void createObject()
	{
		m_pSignal = new CSignalIndicators((HINSTANCE)rhodesData.m_hInstance, rhodesData.m_hBrowserWnd);	
		if (!m_pSignal->OpenWLAN())
    {
			m_pSignal = NULL;		
			LOG(ERROR) + "WLAN support not available";
    }
	}
    virtual void getRefreshInterval(rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pSignal)
			createObject();
		if (!m_pSignal){return;}
		oResult.set((int)m_pSignal->GetInterval());
    }

    virtual void setRefreshInterval( int refreshInterval, rho::apiGenerator::CMethodResult& oResult)
	{
		if (!m_pSignal)
			createObject();
		if (!m_pSignal){return;}
		m_pSignal->SetInterval(refreshInterval);	
    }
    
	virtual void wlanStatus( rho::apiGenerator::CMethodResult& oResult) 
	{
		if (!m_pSignal)
			createObject();
		if (!m_pSignal){return;}
		if (oResult.hasCallback())
		{
			m_pSignal->SetCallback(oResult);
			if (!m_bSignalStatusRegistrar)
			{
				m_pSignal->AddListener();
				m_bSignalStatusRegistrar = false;
    }
			m_pSignal->Refresh(oResult);
		}
		else
		{
			m_pSignal->Refresh(oResult);
		}
	}

    virtual void stopWlanStatus(rho::apiGenerator::CMethodResult& oResult)
{
		if (!m_pSignal)
			createObject();
		if (!m_pSignal){return;}
		rho::apiGenerator::CMethodResult blank;
		m_pSignal->SetCallback(blank);
		m_pSignal->RemoveListener();
		m_bSignalStatusRegistrar = false;
	}

    virtual void showIcon( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
		if (!m_pSignal)
			createObject();
		if (!m_pSignal){return;}
		m_pSignal->IconInitialization((HINSTANCE)rhodesData.m_hInstance, rhodesData.m_hBrowserWnd);
		rho::String comparitor;
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			comparitor.assign(iterator->first);
			std::transform(comparitor.begin(), comparitor.end(), comparitor.begin(), ::tolower);
			if (comparitor.compare("left") == 0)
				m_pSignal->SetLeft(atoi(iterator->second.c_str()));
			else if (comparitor.compare("top") == 0)
				m_pSignal->SetTop(atoi(iterator->second.c_str()));
			else if (comparitor.compare("layout") == 0)
				m_pSignal->SetLayout(convertToStringW(iterator->second).c_str());
			else if (comparitor.compare("color") == 0)
				m_pSignal->SetColour(convertToStringW(iterator->second).c_str());
			else if (comparitor.compare("refreshinterval") == 0)
				m_pSignal->SetInterval(atoi(iterator->second.c_str()));
			else
				LOG(WARNING) + "Unrecognised parameter passed to showIcon (Signal): " + iterator->first;
		}

		if (m_pSignal->GetIconShown())
{
			LOG(INFO) + "Did not show icon as it is already shown";
}
		else
{
			m_pSignal->Show();
			m_pSignal->AddListener();
}
		oResult.set("true");
	}

    virtual void hideIcon(rho::apiGenerator::CMethodResult& oResult)
{
		if (!m_pSignal)
			createObject();
		if (!m_pSignal){return;}
		if (!m_pSignal->GetIconShown())
		{
			LOG(INFO) + "Did not hide icon as it is already hidden";
}
		else
{
			m_pSignal->Hide();
			m_pSignal->RemoveListener();
		}

		oResult.set("true");
}

};
class CSignalIndicatorsSingleton: public CSignalIndicatorsSingletonBase
{
    ~CSignalIndicatorsSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};
class CSignalIndicatorsFactory: public CSignalIndicatorsFactoryBase
{
    ~CSignalIndicatorsFactory(){}
    virtual ISignalIndicatorsSingleton* createModuleSingleton()
	{
		return new CSignalIndicatorsImpl();
	}
};
        
extern "C" void Init_SignalIndicators_extension()
{
    CSignalIndicatorsFactory::setInstance( new CSignalIndicatorsFactory() );
    Init_SignalIndicators_API();
}

}