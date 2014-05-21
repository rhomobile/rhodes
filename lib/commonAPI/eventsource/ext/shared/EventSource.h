
#pragma once
#include <vector>
#include "common/rhoStd.h"
#include "common/RhoThread.h"
#include "common/RhoTime.h"
#include "net/INetRequest.h"
#include "logging/RhoLog.h"

namespace rho {

class EventSource : public common::CRhoTimer::ICallback, public net::INetRequestCallback  {
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

    typedef short State;
    static const State CONNECTING = 0;
    static const State OPEN = 1;
    static const State CLOSED = 2;

    State readyState() const;
    void close();
    String url() const;
    bool withCredentials() const;

    DEFINE_LOGCLASS;
    
private:
    
    IEventSourceReceiver* m_pReceiver;    
    static const unsigned long long defaultReconnectDelay;

    EventSource(const String&, IEventSourceReceiver* receiver, const Hashtable<String,String>& init );

    virtual void didReceiveResponse(NetResponse&, const Hashtable<String,String>* headers);
    virtual void didReceiveData(const char*, int);
    virtual void didFinishLoading();
    virtual void didFail(NetResponse&);

    virtual void stop();

    void connect();

    void networkRequestEnded();
    void scheduleInitialConnect();
    void scheduleReconnect();
    virtual bool onTimer();
    void abortConnectionAttempt();
    void parseEventStream();
    void parseEventStreamLine(unsigned pos, int fieldLength, int lineLength);

    String m_url;
    bool m_withCredentials;
    State m_state;

    Vector<unsigned char> m_receiveBuf;
    bool m_discardTrailingNewline;

      //Actually thread executor would own net request so we don't have to manage its lifetime. Just need to make sure we don't access killed object.
    net::CAsyncNetRequest* m_pNetRequest;

    String m_eventName;
    Vector<unsigned char> m_data;
    String m_currentlyParsedEventId;
    String m_lastEventId;
    unsigned long long m_reconnectDelay;
    String m_eventStreamOrigin;
};

}