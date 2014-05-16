
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

using namespace std;

SOCKET __stdcall accept(SOCKET s, struct sockaddr *addr, int *addrlen)
{
    if (!GlobalSettings::IsLibraryInitialized())
    {
        WSASetLastError(WSANOTINITIALISED);
        return (UINT_PTR)SOCKET_ERROR;
    }

    if (*addrlen != sizeof sockaddr_in)
    {
        WSASetLastError(WSAEINVAL);
        return (UINT_PTR)SOCKET_ERROR;
    }

    auto socketHandle = SocketManagement::GetSocket(s);

    if (socketHandle == nullptr)
    {
        WSASetLastError(WSAENOTSOCK);
        return (UINT_PTR) SOCKET_ERROR;
    }

    if (socketHandle->GetSocketType() != InternalSocketType::Listener)
    {
        WSASetLastError(WSAEINVAL);
        return (UINT_PTR) SOCKET_ERROR;
    }

    auto socket = socketHandle->GetSocketAs<InternalSocketListener>();

    if (!socket->IsConnectionReady())
    {
        WSASetLastError(WSAEWOULDBLOCK);
        return (UINT_PTR)SOCKET_ERROR;
    }
    else
    {
        auto socketHandle = socket->GetConnectedSocket();

        auto socket = socketHandle->GetSocketAs<InternalStreamSocket>();

        auto sockAddr = (sockaddr_in*)addr;

        FillSockAddrIn(sockAddr, socket->RemoteHost(), socket->RemotePort());
        sockAddr->sin_family = AF_INET;

        WSASetLastError(0);
        return socketHandle->GetHandle();
    }

    return (UINT_PTR)SOCKET_ERROR;
}