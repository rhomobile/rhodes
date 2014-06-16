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
			oResult.set(static_cast<unsigned long>(m_pHostTracker->getDailogTimeout()));
		}
		virtual void getPollInterval(rho::apiGenerator::CMethodResult& oResult)
		{
			oResult.set(static_cast<unsigned long>(m_pHostTracker->getPollInterval()));
		}
		virtual void getMessage(rho::apiGenerator::CMethodResult& oResult)
		{
			oResult.set(m_pHostTracker->getMessage());
		}

		//overides IRhoExtention
		virtual void onNavigateComplete(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData){
			m_pHostTracker->resumeNetworkChecking(szUrlBeingNavigatedTo);			
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
				LOG(INFO) + "Skipping connection check as it is not enabled in config.xml";
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
		ptr->StartNetworkChecking();
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