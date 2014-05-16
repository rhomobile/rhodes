#pragma once
#include <ppltasks.h>
#include <string>

enum class InternalSocketType
{
    Stream,
    Datagram,
    Listener
};

enum class InternalSocketConnectionStatus
{
    NotConnected,
    Connecting,
    Connected,
    TimedOut,
    Refused,
    Unreachable,
    ForcablyClosed
};

struct ISocket
{
    virtual bool IsNonblocking() = 0;
    virtual void SetNonblocking(bool nonblocking) = 0;
    virtual InternalSocketType GetSocketType() = 0;
    virtual void Close() = 0;
    virtual ~ISocket(){};
};

struct IRWSocket : public ISocket
{
    virtual void SetReadTimeout(int timeout) = 0;
    virtual void SetWriteTimeout(int timeout) = 0;

    virtual void SetWriteBufferSize(int bufferSize) = 0;
    virtual void SetReadBufferSize(int bufferSize) = 0;
    virtual bool IsWriteReady() = 0;
    virtual InternalSocketConnectionStatus GetConnectionStatus() = 0;
    virtual void Connect(const std::wstring remoteAddress, int port) = 0;
    virtual int Send(const char* buffer, int length) = 0;
    virtual int ReadLoadedBytes(unsigned char* buffer, int length) = 0;
    virtual int GetLoadedBytes() = 0;
    virtual int RemotePort() = 0;
    virtual std::wstring RemoteHost() = 0;
    virtual int LocalPort() = 0;
    virtual std::wstring LocalAddress() = 0;
    virtual ~IRWSocket(){};
};