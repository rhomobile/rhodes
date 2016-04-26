#pragma  once

#include <string>
#include <common/ThreadQueue.h>
#include <common/RhoStd.h>

#include "../../../../shared/generated/cpp/PrinterZebraBase.h"
#include "P2PMessageQueue.h"

namespace rho
{
namespace printing
{
namespace ipc
{
namespace thread
{

class ThreadMessageQueue;

class MessageQueueCommand : public rho::common::CThreadQueue::IQueueCommand
{
    friend class ThreadMessageQueue;

protected:
    ipc::Message*                     m_message;
    rho::apiGenerator::CMethodResult  m_oResult;
    P2PMessageQueue*                  m_toServer;
    P2PMessageQueue*                  m_fromServer;

public:
    virtual ~MessageQueueCommand()
    {
        delete m_message;
    }

    MessageQueueCommand() : m_message(0), m_fromServer(0)
    {
    }

    MessageQueueCommand(rho::apiGenerator::CMethodResult& oResult)
        : m_message(0), m_fromServer(0)
    {
        m_oResult = oResult;
    }

    rho::apiGenerator::CMethodResult getResult() { return m_oResult; }

    virtual void execute();
    virtual void resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult);    
    virtual rho::boolean equals(const rho::common::CThreadQueue::IQueueCommand& cmd) { return false; }
    virtual rho::String toString() { return "MessageQueueCommand"; }

private:
    ipc::Message* getMessage() const { return m_message; }
    void setQueues(P2PMessageQueue* toServer, P2PMessageQueue* fromServer) { m_fromServer = fromServer; m_toServer = toServer; }
};

class ThreadMessageQueue : public rho::common::CThreadQueue
{
public:
    const static rho::String  END_OF_MESSAGE_TAG;
    const static rho::StringW SERVICE_PATH;

private:
    const static wstring TO_SERVER_MQ_NAME;
    const static wstring FROM_SERVER_MQ_NAME;
    const static wstring SERVICE_MQ_NAME ;
    
    static ThreadMessageQueue* m_instance;

    P2PMessageQueue* m_serviceCommand;
    P2PMessageQueue* m_toServer;
    P2PMessageQueue* m_fromServer;

    bool m_isServiceRunning;

    DWORD m_servicePid;
private:
    void init();    
    void runService();
    bool registerApplication();
    bool unregisterApplication();

public:
    ThreadMessageQueue() : m_toServer(0), m_fromServer(0), m_isServiceRunning(false) {}
    virtual ~ThreadMessageQueue() {}
    
    void deInit();

    virtual void start(EPriority ePriority);
    virtual void processCommand(IQueueCommand* pCmd);

    static ThreadMessageQueue* getInstance();
};

} // thread
} // ipc
} // printing
} // rho