
#include <memory>
#include "include\SocketHandle.h"
#include "include\InternalSocket.h"

using namespace std;

void InternalSocketHandle::SetSocketType(InternalSocketType socketType)
{
    m_socketType = socketType;
}

InternalSocketHandle::InternalSocketHandle(int handleNumber) : m_isNonblocking(false),
    m_socketType(InternalSocketType::Stream),
    m_readTimeout(3000),
    m_writeTimeout(3000),
    m_readBufferSize(100 * 1024),
    m_writeBufferSize(100 * 1024),
    m_handle(handleNumber)
{

}

auto InternalSocketHandle::GetSocket() -> std::shared_ptr<ISocket>
{
    if (m_socket == nullptr)
    {
        if (m_socketType == InternalSocketType::Stream)
        {
            auto socket = std::make_shared<InternalStreamSocket>();
            socket->SetReadTimeout(m_readTimeout);
            socket->SetNonblocking(m_isNonblocking);
            socket->SetWriteTimeout(m_writeTimeout);

            m_socket = socket;
        }
        else if(m_socketType == InternalSocketType::Datagram)
        {
            auto socket = std::make_shared<InternalDatagramSocket>();
            /* Create datagram socket*/
            socket->SetReadTimeout(m_readTimeout);
            socket->SetNonblocking(m_isNonblocking);
            socket->SetWriteTimeout(m_writeTimeout);

            m_socket = socket;
        }
        else if (m_socketType == InternalSocketType::Listener)
        {
            auto socket = std::make_shared<InternalSocketListener>();
            //socket->SetReadTimeout(m_readTimeout);
            socket->SetNonblocking(m_isNonblocking);
            //socket->SetWriteTimeout(m_writeTimeout);
            socket->SetBindAddress(m_bindAddress);
            socket->SetBindPort(m_bindPort);

            m_socket = std::dynamic_pointer_cast<ISocket>(socket);
        }
    }
    return m_socket;
}

auto InternalSocketHandle::GetSocketType() -> InternalSocketType
{
    return m_socketType;
}

void InternalSocketHandle::SetReadTimeout(int timeout)
{
    if (m_socketType != InternalSocketType::Listener && m_socket.get())
    {
        auto socket = static_cast<IRWSocket*>(m_socket.get());
        socket->SetReadTimeout(timeout);
    }
    m_readTimeout = timeout;
}

int InternalSocketHandle::GetReadTimeout()
{
    
    return m_readTimeout;
}

int InternalSocketHandle::GetWriteTimeout()
{
    return m_writeTimeout;
}

void InternalSocketHandle::SetWriteTimeout(int timeout)
{
    if (m_socketType != InternalSocketType::Listener && m_socket.get())
    {
        auto socket = static_cast<IRWSocket*>(m_socket.get());
        socket->SetWriteTimeout(timeout);
    }
    m_writeTimeout = timeout;
}

void InternalSocketHandle::SetReadBufferSize(int bufferSize)
{
    if (m_socketType != InternalSocketType::Listener && m_socket.get())
    {
        auto socket = static_cast<IRWSocket*>(m_socket.get());
        socket->SetReadBufferSize(bufferSize);
    }
    m_readBufferSize = bufferSize;
}

int InternalSocketHandle::GetReadBufferSize()
{
    return m_readBufferSize;
}

void InternalSocketHandle::SetWriteBufferSize(int bufferSize)
{
    if (m_socketType != InternalSocketType::Listener && m_socket.get())
    {
        auto socket = static_cast<IRWSocket*>(m_socket.get());
        socket->SetWriteBufferSize(bufferSize);
    }

    m_writeBufferSize = bufferSize;
}

int InternalSocketHandle::GetWriteBufferSize()
{
    return m_writeBufferSize;
}

bool InternalSocketHandle::IsNonblocking()
{
    return m_isNonblocking;
}

void InternalSocketHandle::SetNonblocking(bool nonblocking)
{
    m_isNonblocking = nonblocking;

    if (m_socket.get())
    {
        auto socket = static_cast<IRWSocket*>(m_socket.get());
        socket->SetNonblocking(m_isNonblocking);
    }
}

int InternalSocketHandle::GetHandle()
{
    return m_handle;
}

void InternalSocketHandle::SetBindSettings(std::wstring const& address, int port)
{
    m_bindAddress = address;
    m_bindPort = port;

    if (m_socketType == InternalSocketType::Datagram)
    {
        auto socket = GetSocketAs<InternalDatagramSocket>();
        socket->Bind(address, port);
    }
}


void InternalSocketHandle::GetBindSettings(std::wstring& address, int& port)
{
    address = m_bindAddress;
    port = m_bindPort;
}


void InternalSocketHandle::SetSocket(Windows::Networking::Sockets::StreamSocket^ streamSocket)
{
    if (m_socket != nullptr)
        throw std::exception("InternalSocketHandle::SetSocket socket not null");

    auto socket = std::make_shared<InternalStreamSocket>(streamSocket);
    socket->SetReadTimeout(m_readTimeout);
    socket->SetNonblocking(m_isNonblocking);
    socket->SetWriteTimeout(m_writeTimeout);

    m_socket = socket;
}

bool InternalSocketHandle::IsBound()
{
    if (m_socket == nullptr)
        return false;

    if (m_socketType == InternalSocketType::Datagram || m_socketType == InternalSocketType::Stream)
    {
        auto socket = GetSocketAs<IRWSocket>();
        return socket->GetConnectionStatus() == InternalSocketConnectionStatus::Connected;
    }

    if (m_socketType == InternalSocketType::Listener)
    {
        auto socket = GetSocketAs<InternalSocketListener>();
        return socket->IsListening();
    }

    return false;
}

bool InternalSocketHandle::IsSocketCreated()
{
    return m_socket != nullptr;
}
