
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

int __stdcall ioctlsocket(SOCKET s, long cmd, u_long *argp)
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

    switch (cmd)
    {
    case FIONBIO:
        socketHandle->SetNonblocking(*(bool*)argp);
        WSASetLastError(0);
        return 0;
    default:
        WSASetLastError(SOCKET_ERROR);
        return SOCKET_ERROR;
    }
    return SOCKET_ERROR;
}