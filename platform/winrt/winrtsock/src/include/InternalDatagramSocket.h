#pragma once
#include "..\..\include\winrtsock.h"
#include "IRWSocket.h"
#include <ppltasks.h>
#include <vector>
#include <string>
#include "BufferReader.h"
#include <map>

ref class RecvHelper;

class InternalDatagramSocket : public IRWSocket
{
private:
    struct OutputStreamCacheEntry
    {
        OutputStreamCacheEntry(std::wstring const& host, int port) : dataWriter(nullptr)
        {
            dataWriter = nullptr;

            remoteHost = host;
            remotePort = port;
        }

        std::wstring remoteHost;
        int remotePort;
        Windows::Storage::Streams::DataWriter^ dataWriter;
    };

public:
    InternalDatagramSocket();

    ~InternalDatagramSocket();
    virtual bool IsNonblocking();
    virtual void SetNonblocking(bool nonblocking);
    virtual void SetReadTimeout(int timeout);
    virtual void SetWriteTimeout(int timeout);
    virtual void SetWriteBufferSize(int bufferSize);
    virtual void SetReadBufferSize(int bufferSize) ;

    virtual InternalSocketType GetSocketType();
    virtual InternalSocketConnectionStatus GetConnectionStatus();
    virtual void Connect(const std::wstring remoteAddress, int port);
    virtual int Send(const char* buffer, int length);
    virtual int Receive(unsigned char* buffer, int length);
    virtual int ReadLoadedBytes(unsigned char* buffer, int length);
    virtual int GetLoadedBytes();
    virtual int RemotePort();
    virtual int LocalPort();
    virtual std::wstring RemoteHost();
    virtual std::wstring LocalAddress();
    virtual void Close();
    virtual bool IsWriteReady();
    void OnMessageReceived(Windows::Networking::Sockets::DatagramSocket^ socket, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs^ args);

    void Bind(std::wstring localaddress, int localport = 0);
    int SendTo(const char* buffer, int length, const std::wstring& remotename, int reportPort);
    int Read(unsigned const char* buffer, int length, std::wstring& remotename, int& remotePort);
private:
    bool HasOutputStreamCache(std::wstring const& remoteHost, int remotePort);
    OutputStreamCacheEntry& GetOutputCacheEntry(std::wstring const& remoteHost, int remotePort);
private:
    Windows::Foundation::EventRegistrationToken m_messageEventToken;
    int m_readTimeout;
    int m_writeTimeout;
    bool m_isNonblocking;
    int m_bytesToRead;
    HANDLE m_msg_wait_event;
    BufferReader m_bufferReader;
    volatile bool m_isAsyncReading;
    std::wstring m_remoteHost;
    Windows::Storage::Streams::IBuffer^ m_sendBuffer;
    Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs^ m_lastArgs;
    InternalSocketConnectionStatus m_connectionStatus;
    Windows::Networking::Sockets::DatagramSocket^ m_socket;
    std::vector<OutputStreamCacheEntry> m_outputStreamCache;
    int m_readBufferSize;
    int m_writeBufferSize;
    bool m_isWriteReady;
    bool m_shutdown;
    RecvHelper^ m_recvHelper;
};

