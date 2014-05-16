
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

using namespace std;

int __stdcall getpeername(SOCKET s, struct sockaddr *name, int * namelen)
{
    if (!GlobalSettings::IsLibraryInitialized())
    {
        WSASetLastError(WSANOTINITIALISED);
        return SOCKET_ERROR;
    }

    auto socketHandle = SocketManagement::GetSocket(s);

    if (socketHandle == nullptr)
    {
        WSASetLastError(WSAENOTSOCK);
        return SOCKET_ERROR;
    }

    if (*namelen < sizeof sockaddr_in)
    {
        WSASetLastError(WSAEFAULT);
        return SOCKET_ERROR;
    }

    if (!socketHandle->IsSocketCreated())
    {
        WSASetLastError(WSAENOTCONN);
        return SOCKET_ERROR;
    }

    if (socketHandle->GetSocketType() == InternalSocketType::Listener)
    {
        WSASetLastError(WSAENOTCONN);
        return SOCKET_ERROR;
    }

    auto socket = socketHandle->GetSocketAs<IRWSocket>();

    int remotePort = socket->RemotePort();
    wstring remoteIp = socket->RemoteHost();

    if (socket->GetConnectionStatus() != InternalSocketConnectionStatus::Connected || remoteIp.empty())
    {
        WSASetLastError(WSAENOTCONN);
        return SOCKET_ERROR;
    }

    auto addrIn = reinterpret_cast<sockaddr_in*>(name);

    FillSockAddrIn(addrIn, remoteIp, remotePort);
    addrIn->sin_family = AF_INET;

    WSASetLastError(0);
    return 0;
}