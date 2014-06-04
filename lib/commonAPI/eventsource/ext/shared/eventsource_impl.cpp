//
//  EventSourceImpl.cpp
#include "generated/cpp/EventSourceBase.h"
#include "EventSource.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"

#include <algorithm>

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
        
        rho::apiGenerator::CMethodResult m_onOpenCallback;
        rho::apiGenerator::CMethodResult m_onMessageCallback;
        rho::apiGenerator::CMethodResult m_onErrorCallback;

        Hashtable<String,rho::apiGenerator::CMethodResult> m_eventTypeCallbacks;

      public:
        CEventSourceImpl() :
            m_onErrorCallback(0), m_onMessageCallback(0), m_onOpenCallback(0)
        {
        }
        
        virtual ~CEventSourceImpl() 
        {
          LOG(TRACE) + "CEventSourceImpl dtor";
        }
        
        virtual void setOnopen(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::setOnOpen";
            m_onOpenCallback = oResult;
        }

        virtual void setOnmessage(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::setOnmessage";
            m_onMessageCallback = oResult;
        }
        
        virtual void setOnerror(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::setOnerror";
            m_onErrorCallback = oResult;
        }
        
        virtual void getUrl(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::getUrl";
            oResult.set(m_es->url());            
        }
        
        virtual void getReadyState(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::getReadyState";
            oResult.set(m_es->readyState());
        }
        
        virtual void getWithCredentials(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::getWithCredentials";
            oResult.set(m_es->withCredentials());
        }
        
        virtual void create( const rho::String& url,  const rho::Hashtable<rho::String, rho::String>& settings, rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::create: " + url;
            
            if ( url.empty() ) {
                oResult.setArgError("Failed to construct 'EventSource': Cannot open an EventSource to an empty URL.");
                return;
            }
            
            m_es = EventSource::create(url, this, settings );
        }

        virtual void addEventListener( const rho::String& event, rho::apiGenerator::CMethodResult& oResult) {
        
          LOG(INFO) + "CEventSourceImpl::addEventListener: + " + event;

          if (!event.empty())
          {
            String evtLower;
            std::transform(event.begin(), event.end(), std::back_inserter(evtLower), &::tolower);
            m_eventTypeCallbacks.put(evtLower,oResult);
          }

        }

        
        virtual void close(rho::apiGenerator::CMethodResult& oResult) {
            LOG(INFO) + "CEventSourceImpl::close";
            m_es->close();
        }
        
    private:
    
        virtual void onOpen() {
            LOG(INFO) + "CEventSourceImpl::onOpen";
            m_onOpenCallback.set((void*)0);

            if (m_eventTypeCallbacks.containsKey("open"))
            {
              m_eventTypeCallbacks.get("open").set((void*)0);
            }
        }
        
        virtual void onError(const String& error) {
            LOG(INFO) + "CEventSourceImpl::onError - " + error;
            m_onErrorCallback.set(error);

            if (m_eventTypeCallbacks.containsKey("error"))
            {
              m_eventTypeCallbacks.get("error").set(error);
            }
        }
        
        virtual void onMessage(const String& event, const String& message, const String& eventId) {
            LOG(INFO) + "CEventSourceImpl::onMessage - " + message;
            Hashtable<String,String> params;
            params.put("event",event);
            params.put("data",message);
            params.put("lastEventId",eventId);
            params.put("origin",m_es->url());
            m_onMessageCallback.set(params);

            String evtLower;
            std::transform(event.begin(), event.end(), std::back_inserter(evtLower), &::tolower);

            if (m_eventTypeCallbacks.containsKey(evtLower))
            {
              m_eventTypeCallbacks.get(evtLower).set(params);
            }

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