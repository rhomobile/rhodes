
#include <memory>
#include <ppltasks.h>
#include <string>

#include "Include\InternalStreamSocket.h"
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


InternalStreamSocket::InternalStreamSocket() : 
    m_connectionStatus(InternalSocketConnectionStatus::NotConnected),
    m_readTimeout(8000),
    m_writeTimeout(8000),
    m_isNonblocking(false),
    m_isAsyncReading(false),
    m_isBusySending(false),
    m_readBufferSize(1024 * 512),
    m_writeBufferSize(1024 * 512),
    m_isWriteReady(false)
{
    m_socket = ref new StreamSocket();
}

InternalStreamSocket::InternalStreamSocket(Windows::Networking::Sockets::StreamSocket^ socket) : 
    m_readTimeout(8000),
    m_writeTimeout(8000),
    m_isNonblocking(false),
    m_isAsyncReading(false),
    m_isBusySending(false),
    m_readBufferSize(1024 * 512),
    m_writeBufferSize(1024 * 512),
    m_isWriteReady(false)
{
    m_socket = socket;
    InitializeSocket();
}

void InternalStreamSocket::InitializeSocket()
{
    if (!m_socket)
        return;

    m_remoteHost = m_socket->Information->RemoteAddress->CanonicalName->Data();
    m_socketReader = ref new DataReader(m_socket->InputStream);
    m_socketWriter = ref new DataWriter(m_socket->OutputStream);

    m_socketReader->InputStreamOptions = InputStreamOptions::Partial;

    m_connectionStatus = InternalSocketConnectionStatus::Connected;

    m_sendBuffer = ref new Buffer(m_writeBufferSize);
}

InternalStreamSocket::~InternalStreamSocket()
{
    Close();
}

InternalSocketType InternalStreamSocket::GetSocketType()
{
    return InternalSocketType::Stream;
}

void InternalStreamSocket::Connect(const std::wstring remoteAddress, int port)
{
    auto hostname = ref new HostName(ref new String(remoteAddress.c_str()));
    auto remotePort = port.ToString();
    m_connectionStatus = InternalSocketConnectionStatus::Connecting;

   
    try
    {
        auto connectTask = concurrency::create_task(m_socket->ConnectAsync(hostname, remotePort, SocketProtectionLevel::PlainSocket))
            .then([=](concurrency::task<void> previousTask)
        {
            try
            {
                previousTask.get();
                InitializeSocket();
            }
            catch (Platform::Exception^ ex)
            {
                SocketErrorStatus status = SocketError::GetStatus(ex->HResult);

                switch (status)
                {
                case SocketErrorStatus::UnreachableHost:
                    m_connectionStatus = InternalSocketConnectionStatus::Unreachable;
                    break;
                case SocketErrorStatus::ConnectionTimedOut:
                    m_connectionStatus = InternalSocketConnectionStatus::TimedOut;
                    break;
                case SocketErrorStatus::ConnectionRefused:
                    m_connectionStatus = InternalSocketConnectionStatus::Refused;
                    break;
                default:
                    m_connectionStatus = InternalSocketConnectionStatus::NotConnected;
                    break;
                }
                return m_connectionStatus;
            }

            if (m_isNonblocking)
                StartAsyncReceive();

            return m_connectionStatus;
        });

        if (!m_isNonblocking)
            connectTask.get();
    }
    catch (Exception^)
    {
        m_connectionStatus = InternalSocketConnectionStatus::NotConnected;
    }
}

int InternalStreamSocket::Send(const char* buffer, int length)
{
    int bytesSent = 0;

    if (length <= 0)
        return -1;
    
    auto rtBuffer = m_sendBuffer;

    length = min(static_cast<int>(rtBuffer->Capacity), length);

    CopyToIBuffer((unsigned char*)buffer, length, rtBuffer);
    rtBuffer->Length = length;

    m_socketWriter->WriteBuffer(rtBuffer);
    
    if (!m_isNonblocking)
    {
        bool success = false;
        cancellation_token_source cts;
        auto ctsToken = cts.get_token();
        auto storeTask = create_task(m_socketWriter->StoreAsync(), ctsToken);
        auto lastError = SocketSendExceptionReason::Unknown;

        bytesSent = cancel_after_timeout(storeTask, cts, (unsigned int) m_writeTimeout)
            .then([ctsToken, &lastError, &success](task<unsigned int> resultTask)
        {
            int sent = 0;
            try
            {
                if (ctsToken.is_canceled())
                {
                    lastError = SocketSendExceptionReason::Timeout;
                    return 0;
                }
                sent = resultTask.get();
                success = true;
                return sent;
            }
            catch (std::exception&)
            {
                lastError = SocketSendExceptionReason::Timeout;
                return 0;
            }
            catch (Exception^)
            {
                lastError = SocketSendExceptionReason::Timeout;
                return 0;
            }
        }).get();


        if (!success)
        {
            throw SocketSendException(lastError);
        }
        else
        {
            return bytesSent;
        }
    }
    else
    {
        if (m_isBusySending)
        {
            //return length;
        }

        m_isBusySending = true;
        auto storeTask = create_task(m_socketWriter->StoreAsync());

        storeTask.then([this](task<unsigned int> resultTask)
        {
            int sent = 0;
            try
            {
                sent = resultTask.get();
                m_isBusySending = false;
                return sent;
            }
            catch (std::exception&)
            {
                m_connectionStatus = InternalSocketConnectionStatus::ForcablyClosed;
                m_isBusySending = false;
                return 0;
            }
            catch (Exception^)
            {
                m_connectionStatus = InternalSocketConnectionStatus::ForcablyClosed;
                m_isBusySending = false;
                return 0;
            }
        });
       
        return length;
    }
}

int InternalStreamSocket::Receive(unsigned char* buffer, int length)
{
    cancellation_token_source cts;
    auto ctsToken = cts.get_token();

    auto lastError = SocketRecieveExceptionReason::Unknown;
    bool success = false;

    auto bytesRead = cancel_after_timeout(create_task(m_socketReader->LoadAsync(length), ctsToken), cts, m_readTimeout)
        .then([this, length, buffer, ctsToken, &lastError, &success](task<unsigned int> resultTask)
    {
        int read = 0;
        try
        {
            if (ctsToken.is_canceled())
            {
                lastError = SocketRecieveExceptionReason::Timeout;
                return 0;
            }
            read = resultTask.get();
            success = true;
        }
        catch (std::exception&)
        {
            lastError = SocketRecieveExceptionReason::Timeout;
            return 0;
        }
        catch (Exception^)
        {
            lastError = SocketRecieveExceptionReason::Timeout;
            return 0;
        }

        int bytesToRead = min((int) m_socketReader->UnconsumedBufferLength, length);

        IBuffer^ rtBuffer = m_socketReader->ReadBuffer(bytesToRead);
        
        unsigned int readLen = rtBuffer->Length;
        CopyFromIBuffer(buffer, readLen, rtBuffer);

        return (int) readLen;
    }).get();

    if (!success)
    {
        throw SocketReceiveException(lastError);
    }

    return bytesRead;
}

bool InternalStreamSocket::IsNonblocking()
{
    return m_isNonblocking;
}

InternalSocketConnectionStatus InternalStreamSocket::GetConnectionStatus()
{
    return m_connectionStatus;
}

void InternalStreamSocket::SetReadTimeout(int timeout)
{
    m_readTimeout = timeout;
}

void InternalStreamSocket::SetWriteTimeout(int timeout)
{
    m_writeTimeout = timeout;
}

void InternalStreamSocket::SetNonblocking(bool nonblocking)
{
    m_isNonblocking = nonblocking;

    if (m_isNonblocking)
        StartAsyncReceive();
}

void InternalStreamSocket::StartAsyncReceive()
{
    if (m_isAsyncReading ||
       !m_socketReader   ||
        m_connectionStatus != InternalSocketConnectionStatus::Connected)
        return;

    m_socketReader->InputStreamOptions = InputStreamOptions::Partial;

    int length = 1024 * 100;

    m_isAsyncReading = true;

    try
    {
        auto bytesRead = create_task(m_socketReader->LoadAsync(length))
            .then([this](task<unsigned int> resultTask)
        {
            try
            {
                auto result = resultTask.get();
                result++;
            }
            catch (std::exception&)
            {
                m_connectionStatus = InternalSocketConnectionStatus::ForcablyClosed;
            }
            catch (Platform::Exception^)
            {
                m_connectionStatus = InternalSocketConnectionStatus::ForcablyClosed;
            }
            m_isAsyncReading = false;
        });
    }
    catch (Exception^ e)
    {
        m_connectionStatus = InternalSocketConnectionStatus::Connected;
        m_isAsyncReading = false;
    }
}

int InternalStreamSocket::ReadLoadedBytes(unsigned char* buffer, int length)
{
    if (!m_isNonblocking)
    {
        return Receive(buffer, length);
    }

    int loadedBytes = GetLoadedBytes();
    if (loadedBytes == 0)
    {
        StartAsyncReceive();

        return 0;
    }

    if (!m_bufferReader.HasBuffer())
    {
        try
        {
            IBuffer^ rtBuffer = m_socketReader->ReadBuffer(loadedBytes);
            m_bufferReader.Attatch(rtBuffer);
            
        }
        catch (Exception^ e)
        {
            m_connectionStatus = InternalSocketConnectionStatus::ForcablyClosed;
            return 0;
        }

    }

    int bytesRead = m_bufferReader.Read(buffer, length);
    try
    {
        if (m_bufferReader.UnconsumedBytes() == 0)
        {
            m_bufferReader.Detatch();
            StartAsyncReceive();
        }
        else if (m_socketReader->UnconsumedBufferLength < 20 * 1024)
        {
            StartAsyncReceive();
        }
    }
    catch (Exception^ e)
    {
        m_connectionStatus = InternalSocketConnectionStatus::ForcablyClosed;
        return 0;
    }
    
    assert(bytesRead != 0);

    return bytesRead;
}

int InternalStreamSocket::GetLoadedBytes()
{
    StartAsyncReceive();
    if (!m_socketReader)
        return 0;

    try
    {
        return m_socketReader->UnconsumedBufferLength + m_bufferReader.UnconsumedBytes();
    }
    catch (Exception^ e)
    {
        m_connectionStatus = InternalSocketConnectionStatus::ForcablyClosed;
        return 0;
    }
}

std::wstring InternalStreamSocket::RemoteHost()
{
    return m_remoteHost;
}

int InternalStreamSocket::RemotePort()
{
    auto remotePortString = m_socket->Information->RemotePort;

    int port = _wtoi(remotePortString->Data());

    return port;
}

int InternalStreamSocket::LocalPort()
{
    auto localportString = m_socket->Information->LocalPort;

    int port = _wtoi(localportString->Data());

    return port;
}

std::wstring InternalStreamSocket::LocalAddress()
{
    auto remotePortString = m_socket->Information->LocalAddress->CanonicalName;

    wstring str(remotePortString->Data());

    return str;
}

void InternalStreamSocket::Close()
{
    if (m_socket == nullptr)
        return;

    m_connectionStatus = InternalSocketConnectionStatus::NotConnected;
}

void InternalStreamSocket::SetWriteBufferSize(int bufferSize)
{
    m_writeBufferSize = bufferSize;
}

void InternalStreamSocket::SetReadBufferSize(int bufferSize)
{
    m_readBufferSize = bufferSize;
}

bool InternalStreamSocket::IsWriteReady()
{
    return m_isWriteReady;
}
