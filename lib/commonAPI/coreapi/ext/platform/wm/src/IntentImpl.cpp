#include "../../../shared/generated/cpp/IntentBase.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;
    
    class CIntentSingletonImpl: public CIntentSingletonBase
    {
    public:
        
        CIntentSingletonImpl(): CIntentSingletonBase(){}
        
        //methods
        // send Send intent. 
        virtual void send( const rho::Hashtable<rho::String, rho::String>& params, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("send","Intent");
            
        } 
        // startListening Start listening for custom intents 
        virtual void startListening(rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("startListening","Intent");
            
        } 
        // stopListening Stop listening for custom intents 
        virtual void stopListening(rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("stopListening","Intent");
            
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