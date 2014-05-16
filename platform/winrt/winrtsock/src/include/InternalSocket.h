#pragma once

#include "..\..\include\winrtsock.h"
#include "InternalStreamSocket.h"
#include "InternalDatagramSocket.h"
#include "InternalSocketListener.h"


enum class SocketBindExceptionReason
{
    Unknown,
    AccessDenied,
    AddressInUse,
    AddressNotAvailable
};

class SocketBindException : public std::exception
{
public:
    SocketBindException(SocketBindExceptionReason reason) :  std::exception("bind error"), 
                                                            m_reason(SocketBindExceptionReason::Unknown)
    {
        m_reason = reason;
    }

    SocketBindExceptionReason GetReason()
    {
        return m_reason;
    }
private:
    SocketBindExceptionReason m_reason;
};

enum class SocketSendExceptionReason
{
    Unknown,
    Timeout
};

class SocketSendException : public std::exception
{
public:
    SocketSendException(SocketSendExceptionReason reason) : std::exception("send error"),
        m_reason(SocketSendExceptionReason::Unknown)
    {
        m_reason = reason;
    }

    SocketSendExceptionReason GetReason()
    {
        return m_reason;
    }
private:
    SocketSendExceptionReason m_reason;
};

enum class SocketRecieveExceptionReason
{
    Unknown,
    Timeout
};

class SocketReceiveException : public std::exception
{
public:
    SocketReceiveException(SocketRecieveExceptionReason reason) : std::exception("receive error"),
        m_reason(SocketRecieveExceptionReason::Unknown)
    {
        m_reason = reason;
    }

    SocketRecieveExceptionReason GetReason()
    {
        return m_reason;
    }
private:
    SocketRecieveExceptionReason m_reason;
};

enum class SocketListenExceptionReason
{
    Unknown,
    AddressInUse
};

class SocketListenException : public std::exception
{
public:
    SocketListenException(SocketListenExceptionReason reason) : std::exception("listen error"),
        m_reason(SocketListenExceptionReason::Unknown)
    {
        m_reason = reason;
    }

    SocketListenExceptionReason GetReason()
    {
        return m_reason;
    }
private:
    SocketListenExceptionReason m_reason;
};