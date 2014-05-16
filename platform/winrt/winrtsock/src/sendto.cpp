
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"
#include "include\MiscUtils.h"

using namespace std;

int __stdcall sendto(SOCKET s, const char * buf, int len, int /*flags*/, const struct sockaddr *to, int tolen)
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

    if (socketHandle->GetSocketType() == InternalSocketType::Listener)
    {
        WSASetLastError(WSAENOTSOCK);
        return SOCKET_ERROR;
    }

    auto socket = socketHandle->GetSocketAs<IRWSocket>();

    if (socket->GetConnectionStatus() != InternalSocketConnectionStatus::Connected)
    {
        WSASetLastError(0);
        return len;
    }
    
    if (socketHandle->GetSocketType() == InternalSocketType::Stream)
    {
        return send(s, buf, len, 0);
    }
    else
    {
        if (tolen != sizeof sockaddr_in && to != nullptr)
        {
            WSASetLastError(WSAEFAULT);
            return SOCKET_ERROR;
        }

        auto datagram = dynamic_pointer_cast<InternalDatagramSocket>(socket);
        
        auto addrTo = (sockaddr_in*) to;

        in_addr inaddr = addrTo->sin_addr;
        auto str = inet_ntoa(inaddr);

        auto remoteAddress = SlimToWideString((unsigned char*)str);
        int port = ntohs(addrTo->sin_port);
        datagram->SendTo(buf, len, remoteAddress, port);
        return len;
    }
}