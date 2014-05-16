
#include <memory>
#include <string>
#include <ppltasks.h>

#include "Include\InternalSocketListener.h"
#include "Include\MiscUtils.h"
#include "Include\InternalSocketManagement.h"
#include "include\ppltaskhelpers.h"

using namespace concurrency;
using namespace std;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;

bool InternalSocketListener::IsNonblocking()
{
    return m_isNonblocking;
}

void InternalSocketListener::SetNonblocking(bool nonblocking)
{
    m_isNonblocking = nonblocking;
}

InternalSocketType InternalSocketListener::GetSocketType()
{
    return InternalSocketType::Listener;
}

void InternalSocketListener::StartListening()
{
    SocketErrorStatus result = SocketErrorStatus::Unknown;
    bool success = false;

    try
    {
        create_task(m_socketListener->BindServiceNameAsync(m_bindPort.ToString())).then([&success, &result](task<void> resultTask)
        {
            try
            {
                resultTask.get();
                success = true;

            }
            catch (Exception^ ex)
            {
                result = Windows::Networking::Sockets::SocketError::GetStatus(ex->HResult);
            }
        }).get();
    }
    catch (Exception^)
    {
    	
    }
    

    if (!success)
    {
        auto reason = SocketListenExceptionReason::Unknown;
        switch (result)
        {
        case SocketErrorStatus::AddressAlreadyInUse:
             reason = SocketListenExceptionReason::AddressInUse;
            break;

        }
        throw SocketListenException(reason);
    }
}

InternalSocketListener::InternalSocketListener() : m_isNonblocking(false), m_bindPort(0), m_lastConnectionArgs(nullptr), m_isListening(false)
{
    m_msg_wait_event = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);

    m_socketListener = ref new StreamSocketListener();
    m_socketListener->ConnectionReceived += ref new TypedEventHandler<StreamSocketListener ^, StreamSocketListenerConnectionReceivedEventArgs ^>([this](StreamSocketListener ^sender, StreamSocketListenerConnectionReceivedEventArgs ^args)
    {
        OnConnectionReceived(sender, args);
    });
}

void InternalSocketListener::OnConnectionReceived(StreamSocketListener ^sender, StreamSocketListenerConnectionReceivedEventArgs ^args)
{
    m_lastConnectionArgs = args;
    WaitForSingleObjectEx(m_msg_wait_event, INFINITE, false);
    ResetEvent(m_msg_wait_event);
}

void InternalSocketListener::Close()
{
}

void InternalSocketListener::SetBindAddress(std::wstring const& bindAddress)
{
    m_bindAddress = bindAddress;
}

void InternalSocketListener::SetBindPort(int port)
{
    m_bindPort = port;
}

bool InternalSocketListener::IsConnectionReady()
{
    if (m_lastConnectionArgs != nullptr)
        return true;

    return false;
}

InternalSocketListener::~InternalSocketListener()
{
    CloseHandle(m_msg_wait_event);
}

std::shared_ptr<InternalSocketHandle> InternalSocketListener::GetConnectedSocket()
{
    if (!IsConnectionReady())
        return nullptr;

    auto streamSocket = m_lastConnectionArgs->Socket;

    auto socketHandle = SocketManagement::CreateSocketHandleFromStreamSocket(streamSocket);

    m_lastConnectionArgs = nullptr;

    SetEvent(m_msg_wait_event);
    return socketHandle;
}

bool InternalSocketListener::IsListening()
{
    return m_isListening;
}
