
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

using namespace std;

int __stdcall getsockname(SOCKET s, struct sockaddr *name, int * namelen)
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

    if (*namelen != sizeof sockaddr_in)
    {
        WSASetLastError(WSAEFAULT);
        return SOCKET_ERROR;
    }

    int localPort = 0;

    if (socketHandle->IsSocketCreated())
    {
        auto socket = socketHandle->GetSocketAs<IRWSocket>();
        localPort = socket->LocalPort();
    }
    else
    {
        wstring address;
        socketHandle->GetBindSettings(address, localPort);
    }

    auto addrIn = reinterpret_cast<sockaddr_in*>(name);

    addrIn->sin_family = AF_INET;

    addrIn->sin_port = htons((u_short) localPort);

    char hostname[1024];

    //Temporary until we read the local address
    if (gethostname(hostname, sizeof hostname) < 0)
    {
        WSASetLastError(WSAEFAULT);
        return SOCKET_ERROR;
    }

    auto host = gethostbyname((const char*) &hostname);
    auto addr = (in_addr*)host->h_addr_list[0];
    addrIn->sin_addr.S_un = addr->S_un;
    *namelen = sizeof sockaddr_in;

    WSASetLastError(0);
    return 0;
}