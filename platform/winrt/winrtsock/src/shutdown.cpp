
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

using namespace std;

//Essentially a no-op now
int __stdcall shutdown(SOCKET s, int /*how*/)
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

    return 0;
}