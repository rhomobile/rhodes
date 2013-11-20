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
        
        rho::apiGenerator::CMethodResult* m_onOpenCallback;
        rho::apiGenerator::CMethodResult* m_onMessageCallback;
        rho::apiGenerator::CMethodResult* m_onErrorCallback;

        
    public:
        CEventSourceImpl() :
            m_onErrorCallback(0), m_onMessageCallback(0), m_onOpenCallback(0)
        {
        }
        
        virtual ~CEventSourceImpl() {}
        
        virtual void setOnopen(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::setOnOpen";
            m_onOpenCallback = &oResult;
        }

        virtual void setOnmessage(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::setOnmessage";
            m_onMessageCallback = &oResult;
        }
        
        virtual void setOnerror(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::setOnerror";
            m_onErrorCallback = &oResult;
        }
        
        virtual void getUrl(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::getUrl";
            oResult.set("");            
        }
        
        virtual void getReadyState(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::getReadyState";
            oResult.set("");
        }
        
        virtual void getWithCredentials(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::getWithCredentials";
            oResult.set("");
        }
        
        virtual void create( const rho::String& url,  const rho::Hashtable<rho::String, rho::String>& settings, rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::create: + " + url;
            m_es = EventSource::create(url, this);
        }
        
        virtual void close(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::close";
        }
        
    private:
    
        virtual void onOpen() {
            LOG(INFO) + "CEventSourceImpl::onOpen";
        }
        
        virtual void onError(const String&) {
            LOG(INFO) + "CEventSourceImpl::onError";
        }
        
        virtual void onMessage(const String&) {
            LOG(INFO) + "CEventSourceImpl::onMessage";
        }

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