
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

using namespace std;

int __stdcall recvfrom(SOCKET s, char * buf, int len, int /*flags*/, struct sockaddr * from, int * fromlen)
{
    if (!GlobalSettings::IsLibraryInitialized())
    {
        WSASetLastError(WSANOTINITIALISED);
        return SOCKET_ERROR;
    }

    if (*fromlen != sizeof sockaddr_in && from != nullptr)
    {
        WSASetLastError(WSAEFAULT);
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

    auto fromAddr = (sockaddr_in*) from;

    bool nonblocking = socketHandle->IsNonblocking();

    if (socketHandle->GetSocketType() == InternalSocketType::Stream)
    {
        auto streamSocket = socketHandle->GetSocketAs<InternalStreamSocket>();

        if (streamSocket->GetConnectionStatus() != InternalSocketConnectionStatus::Connected &&
            streamSocket->GetConnectionStatus() != InternalSocketConnectionStatus::Connected)
        {
            WSASetLastError(0);
            return 0;
        }

        auto bytesRead = streamSocket->ReadLoadedBytes((unsigned char*) buf, len);

        if (bytesRead == 0 && nonblocking)
        {
            WSASetLastError(WSAEWOULDBLOCK);
            return SOCKET_ERROR;
        }

        wstring remotename = streamSocket->RemoteHost();
        int remoteport = streamSocket->RemotePort();

        if (!remotename.empty())
        {
            FillSockAddrIn(fromAddr, remotename, remoteport);
            fromAddr->sin_family = AF_INET;
        }

        return bytesRead;
    }
    else
    {
        auto datagram = socketHandle->GetSocketAs<InternalDatagramSocket>();

        wstring remotename;
        int remoteport = 0;
        auto bytesRead = datagram->Read((const unsigned char*)buf, len, remotename, remoteport);

        if (!remotename.empty())
        {
            FillSockAddrIn(fromAddr, remotename, remoteport);
            fromAddr->sin_family = AF_INET;
        }

        if (bytesRead == 0)
        {
            WSASetLastError(WSAEWOULDBLOCK);
            return SOCKET_ERROR;
        }

        return bytesRead;
    }
}