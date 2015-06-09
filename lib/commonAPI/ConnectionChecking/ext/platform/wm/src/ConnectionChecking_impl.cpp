#include <common/RhodesApp.h>
#include "../../../shared/generated/cpp/ConnectionCheckingBase.h"
#include "HostTracker.h"
namespace rho {

#define EXTN_NAME "ConnectionChecking";
	using namespace apiGenerator;
	using namespace common;

	class CConnectionCheckingImpl: public CConnectionCheckingSingletonBase
	{

	private:
		CHostTracker* m_pHostTracker;
	public:
		CConnectionCheckingImpl(const rho::String& strID):CConnectionCheckingSingletonBase()
		{
			LOG(INFO) + "Initialising interface for ConnectingMessage ";
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
		virtual void getHostURL(rho::apiGenerator::CMethodResult& oResult)
		{
			oResult.set(m_pHostTracker->getHostURL());
		}
		virtual void getTrackConnection(rho::apiGenerator::CMethodResult& oResult)
		{
			oResult.set(m_pHostTracker->isFeatureEnabled());
		}
		virtual void getTimeout(rho::apiGenerator::CMethodResult& oResult)
		{
			oResult.set(m_pHostTracker->getDailogTimeout());
		}
		virtual void getPollInterval(rho::apiGenerator::CMethodResult& oResult)
		{
			oResult.set(m_pHostTracker->getPollInterval());
		}
		virtual void getMessage(rho::apiGenerator::CMethodResult& oResult)
		{
			oResult.set(m_pHostTracker->getMessage());
		}

		void InitNetworkChecking()
		{
			m_pHostTracker->InitConfig();	
			if(m_pHostTracker->isFeatureEnabled())
			{
				m_pHostTracker->StartNetworkChecking();		
			}
		}


		//overides IRhoExtention
		virtual void onNavigateComplete(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData)
		{
			if(m_pHostTracker->isFeatureEnabled())
			{
				m_pHostTracker->SetNavigatedUrl(szUrlBeingNavigatedTo);
				m_pHostTracker->fireEvent(eNavigateCompleteEventIndex);			
			}			
		}	
		virtual void OnLicenseScreen(bool bActivate, const CRhoExtData& oExtData)
		{
			if(m_pHostTracker->isFeatureEnabled())
			{
				if(bActivate)
				{
					m_pHostTracker->fireEvent(eLicenseScreenPopupEventIndex);	

				}
				else
				{
					m_pHostTracker->fireEvent(eLicenseScreenHidesEventIndex);

				}
			}
		}	
		
	};

	class CConnectionCheckingSingleton: public CConnectionCheckingSingletonBase
	{
		~CConnectionCheckingSingleton(){}
	};

	class CConnectionCheckingFactory: public CConnectionCheckingFactoryBase
	{
		~CConnectionCheckingFactory(){}   
		virtual IConnectionChecking* createModuleByID(const rho::String& strID);
		virtual IConnectionCheckingSingleton* createModuleSingleton();
	};

	extern "C" void Init_ConnectionChecking_extension()
	{
		CConnectionCheckingFactory::setInstance( new CConnectionCheckingFactory() );
		Init_ConnectionChecking_API();
		CConnectionCheckingImpl* ptr = (CConnectionCheckingImpl*)CConnectionCheckingFactory::getConnectionCheckingSingletonS();
		ptr->InitNetworkChecking();
	}

	IConnectionChecking* CConnectionCheckingFactory::createModuleByID(const rho::String& strID)
	{
		//unused
		return NULL;
	}
	IConnectionCheckingSingleton* CConnectionCheckingFactory::createModuleSingleton()
	{
		rho::String szExtnName = EXTN_NAME;
		return  new CConnectionCheckingImpl(szExtnName);
	}



}