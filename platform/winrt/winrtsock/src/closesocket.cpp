
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

int __stdcall closesocket(SOCKET s)
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

    SocketManagement::CloseSocket(s);
    return 0;
}