
#include "..\include\winrtsock.h"
#include "Include\MiscUtils.h"
#include "Include\InternalSocketManagement.h"

using namespace std;

int __stdcall connect(SOCKET s, const struct sockaddr FAR *name, int namelen)
{
    if (!GlobalSettings::IsLibraryInitialized())
    {
        WSASetLastError(WSANOTINITIALISED);
        return SOCKET_ERROR;
    }

    if (namelen != sizeof sockaddr_in)
    {
        WSASetLastError(WSAEINVAL);
        return SOCKET_ERROR;
    }

    auto socketHandle = SocketManagement::GetSocket(s);

    if (socketHandle == nullptr)
    {
        WSASetLastError(WSAENOTSOCK);
        return SOCKET_ERROR;
    }

    if (socketHandle->GetSocketType() == InternalSocketType::Listener)
    {
        WSASetLastError(WSAEAFNOSUPPORT);
        return SOCKET_ERROR;
    }

    auto socket = socketHandle->GetSocketAs<IRWSocket>();
   
    if (socket->GetConnectionStatus() != InternalSocketConnectionStatus::NotConnected)
    {
        WSASetLastError(WSAEINPROGRESS);
        return SOCKET_ERROR;
    }

    auto socketAddr = reinterpret_cast<sockaddr_in*>(const_cast <sockaddr*>(name));

    auto address = GetWideAddressStringFromSocketAddress(socketAddr);
    auto port = ntohs(socketAddr->sin_port);

    socket->Connect(address, port);

    bool isNonblocking = socket->IsNonblocking();

    if (!isNonblocking)
    {
        auto status = socket->GetConnectionStatus();
        int lastError = 0;
        switch (status)
        {
        case InternalSocketConnectionStatus::NotConnected:
            lastError = WSAECONNREFUSED;
            return SOCKET_ERROR;
            break;
        case InternalSocketConnectionStatus::Connecting:
            /* shouldnt happen*/
            lastError = 0;
            break;
        case InternalSocketConnectionStatus::Connected:
            lastError = 0;
            break;
        case InternalSocketConnectionStatus::TimedOut:
            lastError = WSAETIMEDOUT;
            return SOCKET_ERROR;
            break;
        case InternalSocketConnectionStatus::Refused:
            lastError = WSAECONNREFUSED;
            return SOCKET_ERROR;
            break;
        case InternalSocketConnectionStatus::Unreachable:
            lastError = WSAENETUNREACH;
            return SOCKET_ERROR;
            break;
        default:
            break;
        }

        WSASetLastError(lastError);

        return lastError == 0 ? 0 : SOCKET_ERROR;
    }
    else
    {
        WSASetLastError(WSAEWOULDBLOCK);
        return WSAEWOULDBLOCK;
    }

    return SOCKET_ERROR;
}