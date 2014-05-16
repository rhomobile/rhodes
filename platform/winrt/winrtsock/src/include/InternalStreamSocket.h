#pragma once
#include <ppltasks.h>
#include <vector>
#include <string>

#include "..\..\include\winrtsock.h"
#include "IRWSocket.h"
#include "BufferReader.h"

class InternalStreamSocket : public IRWSocket
{
public:
    InternalStreamSocket();
    InternalStreamSocket(Windows::Networking::Sockets::StreamSocket^ socket);
    virtual ~InternalStreamSocket();
    virtual bool IsNonblocking() override;
    virtual void SetNonblocking(bool nonblocking) override;
    virtual InternalSocketType GetSocketType() override;
    virtual InternalSocketConnectionStatus GetConnectionStatus();
    virtual void Connect(const std::wstring remoteAddress, int port);
    virtual int Send(const char* buffer, int length) override;
   
    virtual void SetReadTimeout(int timeout);
    virtual void SetWriteTimeout(int timeout);
    virtual void SetWriteBufferSize(int bufferSize);
    virtual void SetReadBufferSize(int bufferSize);
    virtual bool IsWriteReady();

    virtual int RemotePort() override;
    virtual int LocalPort() override;
    virtual std::wstring RemoteHost() override;
    virtual std::wstring LocalAddress() override;
    virtual int ReadLoadedBytes(unsigned char* buffer, int length) override;
    virtual int GetLoadedBytes() override;
    virtual void Close() override;

private:
    int Receive(unsigned char* buffer, int length);
    void StartAsyncReceive();
    void InitializeSocket();
private:
    int m_readTimeout;
    int m_writeTimeout;
    bool m_isNonblocking;
    volatile bool m_isAsyncReading;
    std::wstring m_remoteHost;
    bool m_isBusySending;
    InternalSocketConnectionStatus m_connectionStatus;
    Windows::Networking::Sockets::StreamSocket^ m_socket;
    Windows::Storage::Streams::DataReader^ m_socketReader;
    Windows::Storage::Streams::DataWriter^ m_socketWriter;
    Windows::Storage::Streams::IBuffer^ m_sendBuffer;
    BufferReader m_bufferReader;
    int m_readBufferSize;
    int m_writeBufferSize;
    bool m_isWriteReady;
};

