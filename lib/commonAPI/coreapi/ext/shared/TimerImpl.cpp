//
//  TimerImpl.cpp
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "generated/cpp/TimerBase.h"
#include "logging/RhoLog.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;
    
    class CTimerSingletonImpl: public CTimerSingletonBase
    {
    public:
        
        CTimerSingletonImpl(): CTimerSingletonBase(){}
        
        //methods
        // create Create a timers objects. 
        virtual void create(rho::apiGenerator::CMethodResult& oResult) 
		{
            // RAWLOGC_INFO("create","Timer");
            
        } 
        // enumerate This is documentation 
        virtual void enumerate(rho::apiGenerator::CMethodResult& oResult) 
		{
            // RAWLOGC_INFO("enumerate","Timer");
            
        } 
    };
    
    class CTimerImpl : public CTimerBase
    {
    public:
        virtual ~CTimerImpl() {}

        //methods

        virtual void start( int interval, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void stop(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void isAlive(rho::apiGenerator::CMethodResult& oResult) {

        } 

    };
    
    ////////////////////////////////////////////////////////////////////////
    
    class CTimerFactory: public CTimerFactoryBase    {
    public:
        CTimerFactory(){}
        
        ITimerSingleton* createModuleSingleton()
        { 
            return new CTimerSingletonImpl();
        }
        
        virtual ITimer* createModuleByID(const rho::String& strID){ return new CTimerImpl(); };
        
    };
    
}

extern "C" void Init_Timer_extension()
{
    rho::CTimerFactory::setInstance( new rho::CTimerFactory() );
    rho::Init_Timer_API();
}