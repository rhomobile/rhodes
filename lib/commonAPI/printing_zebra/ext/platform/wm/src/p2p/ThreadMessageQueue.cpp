#include "ThreadMessageQueue.h"
#include "MethodWrappers.h"

#include <common/StringConverter.h>

extern "C" void rho_wmsys_run_app_with_show(const wchar_t* szPath, const wchar_t* szParams, bool bShow );
extern "C" const char* rho_native_get_appname();

namespace rho
{
namespace printing
{
namespace ipc
{
namespace thread
{

ThreadMessageQueue* ThreadMessageQueue::m_instance = NULL;

const wstring ThreadMessageQueue::TO_SERVER_MQ_NAME   = L"p2p-to-client";
const wstring ThreadMessageQueue::FROM_SERVER_MQ_NAME = L"p2p-from-client";

const wstring ThreadMessageQueue::SERVICE_MQ_NAME = L"printing-service-0B77E505-1745-4cfc-9A26-3B5B6AFB9377";

const rho::String  ThreadMessageQueue::END_OF_MESSAGE_TAG = "endofmessage";
const rho::StringW ThreadMessageQueue::SERVICE_PATH = L"\\Program Files\\Printing Service\\PrintingService.exe";

//////////////////////////////////////////////////////////////////////////

void MessageQueueCommand::execute()
{
    try
    {
        int result = -1;

        if (m_toServer && m_message)
        {
            result = m_toServer->Send(m_message, 0);
        }

        if (m_fromServer && result == ipc::OK)
        {
            Message *reviceMsg = new Message();

            while(true)
            {
                result = m_fromServer->Receive(reviceMsg);

                if (ipc::OK == result)
                {
                    rho::String answer = reviceMsg->MessageBytes();

                    answer.resize(reviceMsg->Length());

                    if (answer == ThreadMessageQueue::END_OF_MESSAGE_TAG)
                        return;

                    if (reviceMsg->IsAlert())
                    {
                        m_oResult.setError(answer);
                    }
                    else
                    {
                        resultExecute(answer, m_oResult);
                    }
                }
            }

            delete reviceMsg;
        }    
    }
    catch (...)
    {
        LOG(ERROR) + "unhanded exception in send/recieve messages in P2P message queue";
    }
}

void MessageQueueCommand::resultExecute(const rho::String& resultFromServer, rho::apiGenerator::CMethodResult& apiResult)
{
    apiResult.setJSON(resultFromServer);
}

//////////////////////////////////////////////////////////////////////////

void ThreadMessageQueue::init()
{    
    rho::StringW uuid = rho::common::convertToStringW(rho_native_get_appname());

    m_toServer   = new P2PMessageQueue(false, TO_SERVER_MQ_NAME + uuid);
    m_fromServer = new P2PMessageQueue(true, FROM_SERVER_MQ_NAME + uuid);

    m_serviceCommand = new P2PMessageQueue(false, SERVICE_MQ_NAME);
}

void ThreadMessageQueue::processCommand(IQueueCommand* pCmd)
{
    if ( pCmd )
    {
        runService();

        MessageQueueCommand* command = static_cast<MessageQueueCommand*>(pCmd);
        command->setQueues(m_toServer, m_fromServer);
        command->execute();
    }
}

void ThreadMessageQueue::start(EPriority ePriority)
{
    init();

    rho::common::CThreadQueue::start(ePriority);
}

void ThreadMessageQueue::runService()
{
    if (!m_isServiceRunning)
    {
        LOG(INFO) + "start printing service";

#ifndef _DEBUG
        const wchar_t* szPath = ThreadMessageQueue::SERVICE_PATH.c_str();
        StringW strAppNameW = szPath;
        BOOL bWorked;
        STARTUPINFO suInfo;
        PROCESS_INFORMATION procInfo;
      
        memset (&suInfo, 0, sizeof(suInfo));
        suInfo.cb = sizeof(suInfo);

        bWorked = ::CreateProcess(szPath,
                 NULL,      // can also be NULL
                 NULL,
                 NULL,
                 FALSE,
                 CREATE_NEW_CONSOLE,
                 NULL,
                 NULL,
                 &suInfo,
                 &procInfo);

        if (procInfo.dwThreadId == NULL || !bWorked)
        {
            LOG(ERROR) + "Printing service cant run.";
            LOG(ERROR) + "Cannot execute: " + strAppNameW + ";Error: " + GetLastError();
        }

        m_servicePid = procInfo.dwProcessId;
#endif

        registerApplication();
        
        m_isServiceRunning = true;
    }
}

bool ThreadMessageQueue::registerApplication()
{
    LOG(INFO) + "send register application command to service";

    using namespace rho::printing::ipc::wrappers;

    int result = -1;
    rho::apiGenerator::CMethodResult oRes;
    
    std::auto_ptr<RegisterClient> regTask = std::auto_ptr<RegisterClient>(new RegisterClient(rho_native_get_appname(), oRes)); 

    result = m_serviceCommand->Send(regTask->getMessage());

    return (bool)(result == ipc::OK);
}

bool ThreadMessageQueue::unregisterApplication()
{
    LOG(INFO) + "send unregister application command to service";

    using namespace rho::printing::ipc::wrappers;

    int result = -1;
    rho::apiGenerator::CMethodResult oRes;

    std::auto_ptr<UnregisterClient> regTask = std::auto_ptr<UnregisterClient>(new UnregisterClient(rho_native_get_appname(), oRes)); 

    result = m_serviceCommand->Send(regTask->getMessage());

    return (bool)(result == ipc::OK);
}

ThreadMessageQueue* ThreadMessageQueue::getInstance()
{
    if (!m_instance)
    {
        m_instance = new ThreadMessageQueue();

        m_instance->start(rho::common::IRhoRunnable::epNormal);
    }

    return m_instance;
}

void ThreadMessageQueue::deInit()
{
    unregisterApplication();

    LOG(INFO) + "start terminate printing service pid=" + m_servicePid;

    HANDLE ps = OpenProcess( SYNCHRONIZE|PROCESS_TERMINATE, FALSE, m_servicePid);

    if (!TerminateProcess(ps, 0))
    {
        LOG(INFO) + "printing service wasn't terminate. error: " + GetLastError();
    }

    m_toServer->Close();
    m_fromServer->Close();

    delete m_fromServer;
    delete m_toServer;
}

}
}
}
}