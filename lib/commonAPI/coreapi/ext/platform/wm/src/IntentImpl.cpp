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
        
        virtual void send( const rho::Hashtable<rho::String, rho::String>& params, rho::apiGenerator::CMethodResult& oResult) {
			// TODO: support all other appropriate params
			String appName = params.get("appName");
			String data = params.get("data");
			if (appName.length() <= 0) {
				oResult.setError("\"appName\" can't be empty");
				return;
			}
			if (data.length() <= 0) {
				oResult.setError("\"data\" can't be empty");
				return;
			}
			CSystemFactory::getSystemSingletonS()->sendApplicationMessage(appName, data, oResult);
        } 

        virtual void startListening(rho::apiGenerator::CMethodResult& oResult) {
			CSystemFactory::getSystemSingletonS()->startApplicationMessageNotifications(oResult);
        } 

        virtual void stopListening(rho::apiGenerator::CMethodResult& oResult) {
			CSystemFactory::getSystemSingletonS()->stopApplicationMessageNotifications(oResult);
        } 

    };
    
    class CIntentImpl : public CIntentBase
    {
    public:
        virtual ~CIntentImpl() {}

        //methods

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