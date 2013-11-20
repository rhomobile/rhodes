//
//  EventSourceImpl.cpp
#include "generated/cpp/EventSourceBase.h"
#include "EventSource.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;
    
    class CEventSourceSingletonImpl: public CEventSourceSingletonBase
    {
    public:
        
        CEventSourceSingletonImpl(): CEventSourceSingletonBase(){}
        
        virtual void create( const rho::String& url, rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceSingletonImpl::create: + " + url;
        }
      

    };
    
    class CEventSourceImpl : public CEventSourceBase, public EventSource::IEventSourceReceiver
    {
        rho::common::CAutoPtr<EventSource> m_es;
        
//        rho::apiGenerator::IMethodResult* m_messageCallback;
    public:
        virtual ~CEventSourceImpl() {}
        
        virtual void create( const rho::String& url, rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::create: + " + url;
            m_es = EventSource::create(url, this);
        }
        
        virtual void onOpen() {}
        virtual void onError( const String& error ) {}
        virtual void onMessage( const String& message) {}
    };
    
    ////////////////////////////////////////////////////////////////////////
    
    class CEventSourceFactory: public CEventSourceFactoryBase    {
    public:
        CEventSourceFactory(){}
        
        IEventSourceSingleton* createModuleSingleton()
        { 
            return new CEventSourceSingletonImpl();
        }
        
        virtual IEventSource* createModuleByID(const rho::String& strID){ return new CEventSourceImpl(); };
        
    };
    
}

extern "C" void Init_EventSource_API();

extern "C" void Init_EventSource_extension()
{
    rho::CEventSourceFactory::setInstance( new rho::CEventSourceFactory() );
    Init_EventSource_API();
    
}