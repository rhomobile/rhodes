
#pragma once
#include <vector>
#include "common/rhoStd.h"

namespace rho {


class EventSource {
public:

    class IEventSourceReceiver {
    public:
        virtual ~IEventSourceReceiver() {}
        
        virtual void onOpen() = 0;
        virtual void onError( const String& error ) = 0;
        virtual void onMessage( const String& message ) = 0;
    };

    static EventSource* create( const String& url, IEventSourceReceiver* receiver );
    virtual ~EventSource();
    
private:
    
    static const unsigned long long defaultReconnectDelay;

    //String url() const;
    //bool withCredentials() const;

    typedef short State;
    static const State CONNECTING = 0;
    static const State OPEN = 1;
    static const State CLOSED = 2;

    //State readyState() const;
/*
    DEFINE_ATTRIBUTE_EVENT_LISTENER(open);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(message);
    DEFINE_ATTRIBUTE_EVENT_LISTENER(error);
*/
    //void close();

private:
    EventSource(const String&, IEventSourceReceiver* receiver );
/*
    virtual void didReceiveResponse(unsigned long, const ResourceResponse&) OVERRIDE;
    virtual void didReceiveData(const char*, int) OVERRIDE;
    virtual void didFinishLoading(unsigned long, double) OVERRIDE;
    virtual void didFail(const ResourceError&) OVERRIDE;
    virtual void didFailAccessControlCheck(const ResourceError&) OVERRIDE;
    virtual void didFailRedirectCheck() OVERRIDE;

    virtual void stop() OVERRIDE;
*/
    void connect();
/*
    void networkRequestEnded();
    void scheduleInitialConnect();
    void scheduleReconnect();
    void connectTimerFired(Timer<EventSource>*);
    void abortConnectionAttempt();
    void parseEventStream();
    void parseEventStreamLine(unsigned pos, int fieldLength, int lineLength);
    PassRefPtr<MessageEvent> createMessageEvent();

    URL m_url;
    bool m_withCredentials;
    State m_state;

    Vector<UChar> m_receiveBuf;
    bool m_discardTrailingNewline;
    bool m_requestInFlight;

    String m_eventName;
    Vector<UChar> m_data;
    String m_currentlyParsedEventId;
    String m_lastEventId;
    unsigned long long m_reconnectDelay;
    String m_eventStreamOrigin;
*/
};

}