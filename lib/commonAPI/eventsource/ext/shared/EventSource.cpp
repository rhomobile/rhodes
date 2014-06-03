#include "EventSource.h"
#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

namespace rho {


const unsigned long long EventSource::defaultReconnectDelay = 3000;

IMPLEMENT_LOGCLASS(EventSource,"EventSource");

inline EventSource::EventSource(const String& url, IEventSourceReceiver* receiver, const Hashtable<String,String>& eventSourceInit)
    :
      m_url(url)
    , m_withCredentials(false)
    , m_state(CONNECTING)
//    , m_decoder(TextResourceDecoder::create("text/plain", "UTF-8"))
    , m_discardTrailingNewline(false)
    , m_pNetRequest(0)
    , m_reconnectDelay(defaultReconnectDelay)
    , m_pReceiver(receiver)
{
    //eventSourceInit.get("withCredentials", m_withCredentials);
}

EventSource* EventSource::create(const String& url, IEventSourceReceiver* receiver, const Hashtable<String,String>& eventSourceInit )
{

    LOG(TRACE) + "Creating EventSource with URL: " + url;

    if (url.empty()) {
        return 0;
    }

    String fullUrl(
      (url.find("://")==String::npos)?
        (RHODESAPP().getCurrentUrl(-1) + url):
        (url)
    );

    LOG(TRACE) + "Full URL for EventSource: " + fullUrl;

    EventSource* source = new EventSource(fullUrl, receiver, eventSourceInit);

    source->scheduleInitialConnect();

    return source;
}

EventSource::~EventSource()
{
  close();
}

void EventSource::connect()
{

    LOG(TRACE) + "Connecting EventSource: " + m_url;

    ASSERT(m_state == CONNECTING);
    ASSERT(m_pNetRequest == 0);

    Hashtable<String,String> headers;

    headers.put("Accept","text/event-stream");
    headers.put("Cache-Control", "no-cache");
    if (!m_lastEventId.empty())
        headers.put("Last-Event-ID", m_lastEventId);

    m_pNetRequest = new net::CAsyncNetRequest();
    
    m_pNetRequest->setMethod("GET");
    m_pNetRequest->setUrl(m_url);
    m_pNetRequest->setHeaders(headers);
    m_pNetRequest->setCallback(this);

    new common::CRhoCallInThread<net::CAsyncNetRequest>( m_pNetRequest );

}

void EventSource::networkRequestEnded()
{
    LOG(TRACE) + "networkRequestEnded";

    if (m_pNetRequest == 0)
        return;

    m_pNetRequest = 0;

    if (m_state != CLOSED)
        scheduleReconnect();
//    else
//        unsetPendingActivity(this);
}

void EventSource::scheduleInitialConnect()
{
    LOG(TRACE) + "scheduleInitialConnect";

    ASSERT(m_state == CONNECTING);
    ASSERT(m_pNetRequest == 0);

    ::RHODESAPP().getTimer().addNativeTimer(0,this);
//    connect();
}

void EventSource::scheduleReconnect()
{
    LOG(TRACE) + "scheduleReconnect";

    m_state = CONNECTING;
    ::RHODESAPP().getTimer().addNativeTimer(m_reconnectDelay,this);
    m_pReceiver->onError("Lost connection. Scheduling reconnect.");
}

bool EventSource::onTimer()
{
  LOG(TRACE) + "onTimer";

  connect();
  return true;
}

String EventSource::url() const
{
    return m_url;
}

bool EventSource::withCredentials() const
{
    return m_withCredentials;
}

EventSource::State EventSource::readyState() const
{
    return m_state;
}

void EventSource::close()
{
    LOG(TRACE) + "close";

    if (m_state == CLOSED) {
        ASSERT(m_pNetRequest == 0);
        return;
    }

    ::RHODESAPP().getTimer().stopNativeTimer(this);

    if (m_pNetRequest != 0)
    {
      m_pNetRequest->setCallback(0);
      m_pNetRequest->cancel();
      m_pNetRequest = 0;
    }
//    else {
//        m_state = CLOSED;
//        unsetPendingActivity(this);
//    }

    m_state = CLOSED;
}



void EventSource::didReceiveResponse(NetResponse& response, const Hashtable<String,String>* headers)
{
    LOG(TRACE) + "didReceiveResponse";

    ASSERT(m_state==CONNECTING);

    int statusCode = response.getRespCode();
    bool mimeTypeIsValid = headers->get("content-type") == "text/event-stream";
    bool responseIsValid = statusCode == 200 && mimeTypeIsValid;
    if (responseIsValid) {
        const String& charset = headers->get("content-encoding");
        // If we have a charset, the only allowed value is UTF-8 (case-insensitive).
        responseIsValid = charset.empty() || (strcasecmp(charset.c_str(), "UTF-8")==0);
        if (!responseIsValid) {
          LOG(ERROR) + "EventSource's response has a charset (\"" + charset + "\") that is not UTF-8. Aborting the connection.";
        }
    } else {
        // To keep the signal-to-noise ratio low, we only log 200-response with an invalid MIME type.
        if (statusCode == 200 && !mimeTypeIsValid) {
          LOG(ERROR) + "EventSource's response has a MIME type (\"" + headers->get("content-type") + "\") that is not \"text/event-stream\". Aborting the connection.";
        }
    }

    if (responseIsValid) {
        m_state = OPEN;
        m_pReceiver->onOpen();
    } else {
        m_pReceiver->onError("");
        m_pNetRequest->cancel();
    }
}

void EventSource::didReceiveData(const char* data, int length)
{
    LOG(TRACE) + "didReceiveData";

    if ( m_state != OPEN )
    {
      m_pReceiver->onError("Received data but connection is not opened correctly.");
      m_pNetRequest->cancel();
      return;
    }

    ASSERT(m_pNetRequest != 0);

    m_receiveBuf.insert(m_receiveBuf.end(),data,data+length);
    parseEventStream();
}

void EventSource::didFinishLoading()
{
    LOG(TRACE) + "didFinishLoading";

    ASSERT(m_state == OPEN);
    ASSERT(m_pNetRequest != 0);

    if (m_receiveBuf.size() > 0 || m_data.size() > 0) {
        parseEventStream();

        // Discard everything that has not been dispatched by now.
        m_receiveBuf.clear();
        m_data.clear();
        m_eventName = "";
        m_currentlyParsedEventId = String();
    }
    networkRequestEnded();
}

void EventSource::didFail(NetResponse& error)
{
    LOG(TRACE) + "didFail";

    ASSERT(m_state != CLOSED);
    ASSERT(m_pNetRequest != 0);

//    if (error.isCancellation())
//        m_state = CLOSED;
    networkRequestEnded();
}
/*
void EventSource::didFailAccessControlCheck(const ResourceError& error)
{
    String message = makeString("EventSource cannot load ", error.failingURL(), ". ", error.localizedDescription());
    scriptExecutionContext()->addConsoleMessage(JSMessageSource, ErrorMessageLevel, message);

    abortConnectionAttempt();
}

void EventSource::didFailRedirectCheck()
{
    abortConnectionAttempt();
}

void EventSource::abortConnectionAttempt()
{
    ASSERT(m_state == CONNECTING);

    if (m_requestInFlight)
        m_loader->cancel();
    else {
        m_state = CLOSED;
        unsetPendingActivity(this);
    }

    ASSERT(m_state == CLOSED);
    dispatchEvent(Event::create(eventNames().errorEvent, false, false));
}
*/

void EventSource::parseEventStream()
{
    unsigned int bufPos = 0;
    unsigned int bufSize = m_receiveBuf.size();
    while (bufPos < bufSize) {
        if (m_discardTrailingNewline) {
            if (m_receiveBuf[bufPos] == '\n')
                bufPos++;
            m_discardTrailingNewline = false;
        }

        int lineLength = -1;
        int fieldLength = -1;
        for (unsigned int i = bufPos; lineLength < 0 && i < bufSize; i++) {
            switch (m_receiveBuf[i]) {
            case ':':
                if (fieldLength < 0)
                    fieldLength = i - bufPos;
                break;
            case '\r':
                m_discardTrailingNewline = true;
            case '\n':
                lineLength = i - bufPos;
                break;
            }
        }

        if (lineLength < 0)
            break;

        parseEventStreamLine(bufPos, fieldLength, lineLength);
        bufPos += lineLength + 1;

        // EventSource.close() might've been called by one of the message event handlers.
        // Per spec, no further messages should be fired after that.
        if (m_state == CLOSED)
            break;
    }

    if (bufPos == bufSize)
        m_receiveBuf.clear();
    else if (bufPos)
        m_receiveBuf.erase(m_receiveBuf.begin(),m_receiveBuf.begin()+bufPos);
        //m_receiveBuf.remove(0, bufPos);
}

void EventSource::parseEventStreamLine(unsigned bufPos, int fieldLength, int lineLength)
{
    if (!lineLength) {
        if (!m_data.isEmpty()) {
//            m_data.removeLast();
            m_data.erase(m_data.end()-1);
            if (!m_currentlyParsedEventId./*isNull*/empty()) {
                m_lastEventId.swap(m_currentlyParsedEventId);
                m_currentlyParsedEventId = String();
            }
            m_pReceiver->onMessage(m_eventName,String(m_data.begin(),m_data.end()),m_lastEventId);
            m_data.clear();
            //dispatchEvent(createMessageEvent());
        }
        if (!m_eventName.empty())
            m_eventName = "";
    } else if (fieldLength) {
        bool noValue = fieldLength < 0;

        String field((const char*)&m_receiveBuf[bufPos], (size_t)(noValue ? lineLength : fieldLength));
        int step;
        if (noValue)
            step = lineLength;
        else if (m_receiveBuf[bufPos + fieldLength + 1] != ' ')
            step = fieldLength + 1;
        else
            step = fieldLength + 2;
        bufPos += step;
        int valueLength = lineLength - step;

        if (field == "data") {
            if (valueLength)
//                m_data.append(&m_receiveBuf[bufPos], valueLength);
                m_data.insert(m_data.end(),&m_receiveBuf[bufPos],&m_receiveBuf[bufPos+valueLength]);
//            m_data.append('\n');
            m_data.push_back('\n');
        } else if (field == "event")
            m_eventName = valueLength ? String((const char*)&m_receiveBuf[bufPos], (size_t)valueLength) : "";
            if ( m_eventName.empty() ) {
                m_eventName = "message";
            }
        else if (field == "id")
            m_currentlyParsedEventId = valueLength ? String((const char*)&m_receiveBuf[bufPos], (size_t)valueLength) : "";
        else if (field == "retry") {
            if (!valueLength)
                m_reconnectDelay = defaultReconnectDelay;
            else {
                String value((const char*)&m_receiveBuf[bufPos], (size_t)valueLength);
                bool ok;
                unsigned long long retry = atoll(value.c_str());//value.toUInt64(&ok);
                if (ok)
                    m_reconnectDelay = retry;
            }
        }
    }
}

void EventSource::stop()
{
    LOG(TRACE) + "stop";

    close();
}

} // namespace rho
