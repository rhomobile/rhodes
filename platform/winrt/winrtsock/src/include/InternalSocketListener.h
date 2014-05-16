#pragma once
#include <ppltasks.h>
#include <string>
#include <memory>

#include "..\..\include\winrtsock.h"
#include "MiscUtils.h"
#include "IRWSocket.h"
#include "SocketHandle.h"

class InternalSocketListener : public ISocket
{
public:
    InternalSocketListener();
    virtual ~InternalSocketListener();;
    void StartListening();
    virtual bool IsNonblocking();
    virtual void SetNonblocking(bool nonblocking);
    virtual InternalSocketType GetSocketType();
    virtual void Close() override;
    void SetBindAddress(std::wstring const& bindAddress);
    void SetBindPort(int port);
    bool IsConnectionReady();
    bool IsListening();
    std::shared_ptr<InternalSocketHandle> GetConnectedSocket();
private:
    void OnConnectionReceived(Windows::Networking::Sockets::StreamSocketListener ^sender, Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs ^args);
private:
    bool m_isNonblocking;
    std::wstring m_bindAddress;
    int m_bindPort;
    Windows::Networking::Sockets::StreamSocketListener^ m_socketListener;
    HANDLE m_msg_wait_event;
    bool m_isListening;
    Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs^ m_lastConnectionArgs;
};

