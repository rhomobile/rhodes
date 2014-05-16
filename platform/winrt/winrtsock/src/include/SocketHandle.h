#pragma once
#include "IRWSocket.h"
#include <memory>

class InternalSocketHandle
{
public:
    InternalSocketHandle(int handleNumber);
public:
    void SetSocketType(InternalSocketType socketType);
    auto GetSocketType()->InternalSocketType;
    void SetSocket(Windows::Networking::Sockets::StreamSocket^ socket);
    auto GetSocket()->std::shared_ptr<ISocket>;

    template<class T>
    std::shared_ptr<T> GetSocketAs()
    {
        return std::dynamic_pointer_cast<T>(GetSocket());
    }

    template<class T>
    T* GetSocketAsNoRef()
    {
        return dynamic_cast<T*>(m_socket.get());
    }

    void SetReadTimeout(int timeout);
    int GetReadTimeout();
    int GetWriteTimeout();
    void SetWriteTimeout(int timeout);
    void SetReadBufferSize(int bufferSize);
    int GetReadBufferSize();
    void SetWriteBufferSize(int bufferSize);
    int GetWriteBufferSize();
    bool IsNonblocking();
    void SetNonblocking(bool nonblocking);
    bool IsSocketCreated();
    bool IsBound();
    void SetBindSettings(std::wstring const& address, int port);
    void GetBindSettings(std::wstring& address, int& port);
    int GetHandle();
private:
    InternalSocketType m_socketType;
    int m_handle;
    std::shared_ptr<ISocket> m_socket;
    int m_readTimeout;
    bool m_isNonblocking;
    int m_writeTimeout;
    int m_readBufferSize;
    int m_writeBufferSize;
    std::wstring m_bindAddress;
    int m_bindPort;
};