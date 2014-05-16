
#pragma once
#include <vector>
#include "common/rhoStd.h"
#include "common/RhoThread.h"
#include "common/RhoTime.h"
#include "net/INetRequest.h"

namespace rho {

class CAsyncNetRequest
{
public:
  class ICallback
  {
  public:
    virtual ~ICallback() {}

    virtual void didReceiveResponse(NetResponse&, const Hashtable<String,String>* headers) = 0;
    virtual void didReceiveData(const char*, int) = 0;
    virtual void didFinishLoading(unsigned long, double) = 0;
    virtual void didFail(NetResponse&) = 0;
    
  };

private:
  const String m_method;
  const String m_url;
  const String m_body;
  net::IRhoSession* m_pSession;
  Hashtable<String,String> m_headers;
  CAsyncNetRequest::ICallback* m_pCallback;

  net::CNetRequestWrapper  m_request;

public:

  CAsyncNetRequest( const String& method, const String& url, const String& body, net::IRhoSession* session, Hashtable<String,String>* headers, CAsyncNetRequest::ICallback* callback )
    : m_method(method)
    , m_url(url)
    , m_body(body)
    , m_pSession(session)
    , m_headers(headers==0?(Hashtable<String,String>()):(*headers))
    , m_pCallback(callback)
    , m_request(::getNetRequest())
  {
  }

  void run(common::CRhoThread &)
  {
    NetResponse resp = m_request.doRequest(m_method.c_str(),m_url,m_body,m_pSession,&m_headers);
    if ( m_pCallback != 0 )
    {
      if ( resp.isOK() )
      {
        m_pCallback->didReceiveResponse(resp,&m_headers);
        m_pCallback->didReceiveData(resp.getCharData(),resp.getDataSize());
        m_pCallback->didFinishLoading(0,.0);
      }
      else
      {
        m_pCallback->didFail(resp);
      }
    }
  }

  void cancel()
  {
    m_request.cancel();
  }
};


class EventSource : public common::CRhoTimer::ICallback, public CAsyncNetRequest::ICallback  {
public:

    class IEventSourceReceiver {
    public:
        virtual ~IEventSourceReceiver() {}
        
        virtual void onOpen() = 0;
        virtual void onError( const String& error ) = 0;
        virtual void onMessage( const String& event, const String& message, const String& eventId ) = 0;
    };

    static EventSource* create( const String& url, IEventSourceReceiver* receiver, const Hashtable<String,String>& init );
    virtual ~EventSource();
    
private:
    
    IEventSourceReceiver* m_pReceiver;
    
    static const unsigned long long defaultReconnectDelay;

    String url() const;
    bool withCredentials() const;

    typedef short State;
    static const State CONNECTING = 0;
    static const State OPEN = 1;
    static const State CLOSED = 2;

    State readyState() const;
/*
    DEFINE_ATTRIBUTE_EVENT_LISTENER(open);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(message);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(error);
*/
    void close();

private:
    EventSource(const String&, IEventSourceReceiver* receiver, const Hashtable<String,String>& init );

    virtual void didReceiveResponse(NetResponse&, const Hashtable<String,String>* headers);
    virtual void didReceiveData(const char*, int);
    virtual void didFinishLoading(unsigned long, double);
    virtual void didFail(NetResponse&);
/*
    virtual void didFailAccessControlCheck(const ResourceError&) OVERRIDE;
    virtual void didFailRedirectCheck() OVERRIDE;
*/
    virtual void stop();

    void connect();

    void networkRequestEnded();
    void scheduleInitialConnect();
    void scheduleReconnect();
//    void connectTimerFired(Timer<EventSource>*);
    virtual bool onTimer();
    void abortConnectionAttempt();
    void parseEventStream();
    void parseEventStreamLine(unsigned pos, int fieldLength, int lineLength);
//    PassRefPtr<MessageEvent> createMessageEvent();

    String m_url;
    bool m_withCredentials;
    State m_state;

    Vector<unsigned char> m_receiveBuf;
    bool m_discardTrailingNewline;
    bool m_requestInFlight;

    String m_eventName;
    Vector<unsigned char> m_data;
    String m_currentlyParsedEventId;
    String m_lastEventId;
    unsigned long long m_reconnectDelay;
    String m_eventStreamOrigin;
};

}