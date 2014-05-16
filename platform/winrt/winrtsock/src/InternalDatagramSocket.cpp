
#include <ppltasks.h>
#include <string>

#include "Include\InternalDatagramSocket.h"
#include "Include\MiscUtils.h"
#include "Include\InternalSocketManagement.h"
#include "include\ppltaskhelpers.h"

using namespace std;
using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation::Collections;


bool InternalDatagramSocket::IsNonblocking()
{
    return m_isNonblocking;
}

void InternalDatagramSocket::SetNonblocking(bool nonblocking)
{
    m_isNonblocking = nonblocking;
}

void InternalDatagramSocket::SetReadTimeout(int timeout)
{
    m_readTimeout = timeout;
}

void InternalDatagramSocket::SetWriteTimeout(int timeout)
{
    m_writeTimeout = timeout;
}

InternalSocketType InternalDatagramSocket::GetSocketType()
{
    return InternalSocketType::Datagram;
}

InternalSocketConnectionStatus InternalDatagramSocket::GetConnectionStatus()
{
    return m_connectionStatus;
}


void InternalDatagramSocket::Bind(std::wstring localaddress, int localport /*= 0*/)
{
    auto host = ref new HostName(ref new Platform::String(localaddress.c_str()));

    bool success = false;

    SocketErrorStatus errorStatus = SocketErrorStatus::Unknown;

    try
    {
        create_task(m_socket->BindServiceNameAsync(localport.ToString()))
            .then([this, &success, &errorStatus](task<void> resulttask)
        {
            try
            {
                resulttask.get();
                success = true;
            }
            catch (Exception^ ex)
            {
                errorStatus = SocketError::GetStatus(ex->HResult);
                return;
            }
            catch (exception&)
            {
                return;
            }

            m_connectionStatus = InternalSocketConnectionStatus::Connected;
        }).get();
    }
    catch (Exception^ ex)
    {
        errorStatus = SocketError::GetStatus(ex->HResult);
    }
   

    if (!success)
    {
        auto bindFailReason = SocketBindExceptionReason::Unknown;
        switch (errorStatus)
        {
        case SocketErrorStatus::AddressAlreadyInUse:
            bindFailReason = SocketBindExceptionReason::AddressInUse;
            break;

        case SocketErrorStatus::CannotAssignRequestedAddress:
            bindFailReason = SocketBindExceptionReason::AccessDenied;
            break;

        }

        throw SocketBindException(bindFailReason);
    }
}

void InternalDatagramSocket::Connect(const std::wstring /*remoteAddress*/, int /*port*/)
{
    throw std::logic_error("The method or operation is not implemented.");
}

int InternalDatagramSocket::SendTo(const char* buffer, int length, const std::wstring& remotename, int remotePort)
{
    if (m_connectionStatus != InternalSocketConnectionStatus::Connected)
        return 0;

    auto hostname = ref new HostName(ref new String(remotename.c_str()));

    auto rtBuffer = m_sendBuffer;

    length = min(static_cast<int>(rtBuffer->Capacity), length);

    CopyToIBuffer((unsigned char*) buffer, length, rtBuffer);
    rtBuffer->Length = length;

    auto& cacheItem = GetOutputCacheEntry(remotename, remotePort);

    if (cacheItem.dataWriter)
    {
        unsigned int unstoredBufferLength = cacheItem.dataWriter->UnstoredBufferLength;

        /*if (unstoredBufferLength > m_writeBufferSize)
        return length;*/

        cacheItem.dataWriter->WriteBuffer(rtBuffer);

        try
        {
            cacheItem.dataWriter->StoreAsync();
        }
        catch (Exception^ e)
        {
            m_connectionStatus = InternalSocketConnectionStatus::ForcablyClosed;
            return 0;
        }

        return length;
    }

    create_task(m_socket->GetOutputStreamAsync(hostname, remotePort.ToString()))
        .then([rtBuffer, remotename, remotePort, this](task<IOutputStream^> resulttask)
    {
        IOutputStream^ outstream = nullptr;
        try
        {
            outstream = resulttask.get();

            auto& cacheItem = GetOutputCacheEntry(remotename, remotePort);

            if (!cacheItem.dataWriter)
                cacheItem.dataWriter = ref new DataWriter(outstream);
           
            cacheItem.dataWriter->WriteBuffer(rtBuffer);

            cacheItem.dataWriter->StoreAsync();
        }
        catch (std::exception&)
        {
            m_connectionStatus = InternalSocketConnectionStatus::ForcablyClosed;
            return;
        }
        catch (Exception^)
        {
            m_connectionStatus = InternalSocketConnectionStatus::ForcablyClosed;
            return;
        }
    }).get();

    return length;
}

int InternalDatagramSocket::Send(const char*, int)
{
    throw std::logic_error("The method or operation is not implemented.");
}

int InternalDatagramSocket::Receive(unsigned char* buffer, int len)
{
    return ReadLoadedBytes(buffer, len);
}

int InternalDatagramSocket::ReadLoadedBytes(unsigned char* buffer, int len)
{
    wstring host;
    int port;
    return Read(buffer, len, host, port);
}

int InternalDatagramSocket::GetLoadedBytes()
{
    return m_bytesToRead;
}

int InternalDatagramSocket::RemotePort()
{
    auto remotePortString = m_socket->Information->RemotePort;

    int port = _wtoi(remotePortString->Data());

    return port;
}

int InternalDatagramSocket::LocalPort()
{
    auto localportString = m_socket->Information->LocalPort;

    int port = _wtoi(localportString->Data());

    return port;
}

std::wstring InternalDatagramSocket::RemoteHost()
{
    throw std::logic_error("The method or operation is not implemented.");
}

std::wstring InternalDatagramSocket::LocalAddress()
{
    auto remotePortString = m_socket->Information->LocalAddress->CanonicalName;

    wstring str(remotePortString->Data());

    return str;
}

void InternalDatagramSocket::Close()
{ 
    m_shutdown = true;

    SetEvent(m_msg_wait_event);
}


InternalDatagramSocket::~InternalDatagramSocket()
{
    delete m_socket;
    m_shutdown = true;
    CloseHandle(m_msg_wait_event);
}

ref class RecvHelper sealed
{
internal:
    RecvHelper(InternalDatagramSocket* callback, DatagramSocket^ socket)
    {
        m_messageEventToken = socket->MessageReceived += ref new TypedEventHandler<DatagramSocket ^, DatagramSocketMessageReceivedEventArgs ^>(this, &RecvHelper::OnMessageReceived);
        m_callback = callback;
    }
   
    void OnMessageReceived(DatagramSocket ^sender, DatagramSocketMessageReceivedEventArgs ^args)
    {
        try
        {
            m_callback->OnMessageReceived(sender, args);
        }
        catch (Exception^ e)
        {
        	
        }
    }

private:

     ~RecvHelper()
    {
        if (m_socket)
        m_socket->MessageReceived -= m_messageEventToken;
    }

    Windows::Foundation::EventRegistrationToken m_messageEventToken;
    InternalDatagramSocket* m_callback;
    DatagramSocket^ m_socket;
};


InternalDatagramSocket::InternalDatagramSocket() : m_bytesToRead(0), m_readBufferSize(1024 * 512), m_writeBufferSize(1024 * 512), m_isWriteReady(false), m_shutdown(false)
{
    m_socket = ref new DatagramSocket();
    m_sendBuffer = ref new Buffer(m_writeBufferSize);
    m_recvHelper = ref new RecvHelper(this, m_socket);
    m_msg_wait_event = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
}

int InternalDatagramSocket::Read(const unsigned char* buffer, int length, std::wstring& remotename, int& remotePort)
{
    if (m_lastArgs == nullptr || m_connectionStatus != InternalSocketConnectionStatus::Connected)
        return 0;


    if (!m_bufferReader.HasBuffer())
    {
        auto dr = m_lastArgs->GetDataReader();

        auto rtBuffer = dr->ReadBuffer(dr->UnconsumedBufferLength);

        m_bufferReader.Attatch(rtBuffer);
    }


    int readBytes = m_bufferReader.Read(buffer, length);

    const wchar_t *pRemoteName = m_lastArgs->RemoteAddress->CanonicalName->Data();

    remotename = pRemoteName;
    remotePort = _wtoi(m_lastArgs->RemotePort->Data());

    m_lastArgs = nullptr;
    m_bytesToRead = m_bufferReader.UnconsumedBytes();

    if (m_bytesToRead == 0)
    {
        m_bufferReader.Detatch();
        SetEvent(m_msg_wait_event);
    }

    return readBytes;
}

void InternalDatagramSocket::OnMessageReceived(DatagramSocket^ socket, DatagramSocketMessageReceivedEventArgs^ args)
{
    m_lastArgs = args;

    try
    {
        m_bytesToRead = args->GetDataReader()->UnconsumedBufferLength;
    }
    catch (Exception^ e)
    {
    	
    }

    WaitForSingleObjectEx(m_msg_wait_event,INFINITE, false);
}

bool InternalDatagramSocket::HasOutputStreamCache(std::wstring const& remoteHost, int remotePort)
{
    for each(auto& cacheItem in m_outputStreamCache)
    {
        if (cacheItem.remotePort == remotePort &&
            cacheItem.remoteHost == remoteHost)
        {
            return true;
        }
    }

    return false;
}

auto InternalDatagramSocket::GetOutputCacheEntry(std::wstring const& remoteHost, int remotePort) -> OutputStreamCacheEntry&
{
    for (size_t i = 0; i < m_outputStreamCache.size(); i++)
    {
        OutputStreamCacheEntry& cacheItem = m_outputStreamCache[i];
        if (cacheItem.remotePort == remotePort &&
            cacheItem.remoteHost == remoteHost)
        {
            return cacheItem;
        }
    }

    //add locking
    m_outputStreamCache.emplace_back(remoteHost, remotePort);

    return m_outputStreamCache.back();
}

void InternalDatagramSocket::SetWriteBufferSize(int bufferSize)
{
    m_writeBufferSize = bufferSize;
}

void InternalDatagramSocket::SetReadBufferSize(int bufferSize)
{
    m_readBufferSize = bufferSize;
}

bool InternalDatagramSocket::IsWriteReady()
{
    return m_isWriteReady;
}
