#include "../../../shared/generated/cpp/IntentBase.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"
#include "SystemImpl.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;
    
    class CIntentSingletonImpl: public CIntentSingletonBase
    {
    public:
        
        CIntentSingletonImpl(): CIntentSingletonBase(){}
        
        virtual void send( const Hashtable<String, String>& params, apiGenerator::CMethodResult& oResult) {
			String appName = params.get("appName");
			String data = params.get("data");
			String uri = params.get("uri");
			if (appName.length() > 0)
				CSystemFactory::getSystemSingletonS()->sendApplicationMessage(appName, data, oResult);
			else if (uri.length() > 0)
				CSystemFactory::getSystemSingletonS()->openUrl(uri, oResult);
			else
				oResult.setError("Intent sending error: no 'appName' or 'uri' specified");
        } 

        virtual void startListening(apiGenerator::CMethodResult& oResult) {
			CSystemFactory::getSystemSingletonS()->startApplicationMessageNotifications(oResult);
        } 

        virtual void stopListening(apiGenerator::CMethodResult& oResult) {
			CSystemFactory::getSystemSingletonS()->stopApplicationMessageNotifications(oResult);
        } 

    };
    
    class CIntentImpl : public CIntentBase
    {
    public:
        virtual ~CIntentImpl() {}
    };
    
    ////////////////////////////////////////////////////////////////////////
    
    class CIntentFactory: public CIntentFactoryBase    {
    public:
        CIntentFactory(){}
        
        IIntentSingleton* createModuleSingleton()
        { 
            return new CIntentSingletonImpl();
        }
        
        virtual IIntent* createModuleByID(const rho::String& strID){ return new CIntentImpl(); };
        
    };
    
}

extern "C" void Init_Intent()
{
    rho::CIntentFactory::setInstance( new rho::CIntentFactory() );
    rho::Init_Intent_API();
    
}