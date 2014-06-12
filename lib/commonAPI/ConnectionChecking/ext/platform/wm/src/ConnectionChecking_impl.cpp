#include <common/RhodesApp.h>
#include "../../../shared/generated/cpp/ConnectionCheckingBase.h"
#include "HostTracker.h"


//extern UINT WM_BROWSER_ONNAVIGATECOMPLETE;

namespace rho {

using namespace apiGenerator;
using namespace common;


#define EXTN_NAME "ConnectionChecking"
extern "C" HWND getMainWnd();

class CConnectionCheckingImpl: public IRhoExtension,  public CConnectionCheckingSingletonBase
{
private:
	CHostTracker* m_pHostTracker;
public:
	CConnectionCheckingImpl()
	{
		
		/*wchar_t tempdata[100];
		mbstowcs(tempdata,strID.c_str(),(sizeof(wchar_t) * strlen(strID.c_str())));*/
		//MessageBox(NULL,tempdata,tempdata,MB_OK);
		LOG(INFO) + "Initialising interface for ConnectingMessage "; //+ strID; 
		/*::MessageBox(getMainWnd(),L"test", L"connecting", MB_OK);*/
		/*CRhodesApp& ptr = */
		//RHODESAPP().getExtManager().registerExtension(strID, this );
		m_pHostTracker  = HostTrackerFactory::createHostTracker();
		
	}
	~CConnectionCheckingImpl()
	{
		LOG(INFO) + "Shutting down connection checker "; 
		if (m_pHostTracker){
			m_pHostTracker->StopNetworkChecking();
			delete m_pHostTracker;
			m_pHostTracker = NULL;	
		}
	}
	virtual void onNavigateComplete(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData){
		m_pHostTracker->resumeNetworkChecking(szUrlBeingNavigatedTo);
		//PostThreadMessage(m_pTracker->getThreadID(),WM_BROWSER_ONNAVIGATECOMPLETE,NULL,NULL);
	}
	virtual void OnAppActivate(bool bActivate, const CRhoExtData& oExtData){
		
	}
	void StartNetworkChecking()
	{
		m_pHostTracker->InitConfig();	
		if(m_pHostTracker->isFeatureEnabled())
		{
			m_pHostTracker->StartNetworkChecking();	
			LOG(INFO) + "Host tracker started";
		}
		else
		{
			LOG(INFO) + "Skipping Host tracker as it is not enabled in config.xml";
		}
	}
	virtual void getHostURL(rho::apiGenerator::CMethodResult& oResult)
	{
	}
	virtual void getTrackConnection(rho::apiGenerator::CMethodResult& oResult)
	{
	}
	virtual void getTimeout(rho::apiGenerator::CMethodResult& oResult)
	{
	}
	virtual void getPollInterval(rho::apiGenerator::CMethodResult& oResult)
	{
	}
	virtual void getMessage(rho::apiGenerator::CMethodResult& oResult)
	{
	}

	
};


class CConnectionCheckingSingleton: public CConnectionCheckingSingletonBase 
{
    ~CConnectionCheckingSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CConnectionCheckingFactory: public CConnectionCheckingFactoryBase
{
    ~CConnectionCheckingFactory(){}
    virtual IConnectionCheckingSingleton* createModuleSingleton();
    virtual IConnectionChecking* createModuleByID(const rho::String& strID);
};

extern "C" void Init_ConnectionChecking_extension()
{
	::MessageBox(getMainWnd(),L"test", L"connecting", MB_OK);

    CConnectionCheckingFactory::setInstance( new CConnectionCheckingFactory() );
    Init_ConnectionChecking_API();
	//create the object now and forget it
	CConnectionCheckingImpl* ptr = (CConnectionCheckingImpl*)CConnectionCheckingFactory::getInstance()->getModuleByID(EXTN_NAME);
	ptr->StartNetworkChecking();

}

IConnectionChecking* CConnectionCheckingFactory::createModuleByID(const rho::String& strID)
{
    //return new CConnectionCheckingImpl(strID);
	return NULL;
}

IConnectionCheckingSingleton* CConnectionCheckingFactory::createModuleSingleton()
{
    return new CConnectionCheckingImpl();
}

void CConnectionCheckingSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement(EXTN_NAME);
    oResult.set(arIDs);
}

rho::String CConnectionCheckingSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}