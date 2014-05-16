
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

using namespace std;

int __stdcall setsockopt(SOCKET s, int /*level*/, int optname, const char * optval, int /*optlen*/)
{
    if (optval == nullptr)
        return SOCKET_ERROR;

    auto socketHandle = SocketManagement::GetSocket(s);

    if (socketHandle == nullptr)
    {
        WSASetLastError(WSAENOTSOCK);
        return SOCKET_ERROR;
    }

    switch (optname)
    {
    case SO_REUSEADDR:
        WSASetLastError(0);
        return 0;
    case SO_SNDBUF:
        {
            DWORD buffsize = (DWORD)*optval;
            socketHandle->SetWriteBufferSize(buffsize);
            WSASetLastError(0);
            return 0;
        }
    case SO_RCVBUF:
        {
            DWORD buffsize = (DWORD)*optval;
            socketHandle->SetReadBufferSize(buffsize);
            return 0;
        }
    default:
        WSASetLastError(SOCKET_ERROR);
        return SOCKET_ERROR;
    }
}