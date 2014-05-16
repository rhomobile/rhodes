
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

using namespace std;

int _stdcall recv(SOCKET s, char * buf, int len, int /*flags*/)
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

    if (!socketHandle->IsSocketCreated())
    {
        WSASetLastError(WSAENOTCONN);
        return SOCKET_ERROR;
    }

    auto socket = socketHandle->GetSocketAs<IRWSocket>();
    bool isNonBlocking = socket->IsNonblocking();

    if (!isNonBlocking)
    {
        try
        {
            WSASetLastError(0);
            return socket->ReadLoadedBytes((unsigned char*) buf, len);
        }
        catch (SocketReceiveException& e)
        {
        	switch (e.GetReason())
        	{
            case SocketRecieveExceptionReason::Timeout:
                WSASetLastError(WSAETIMEDOUT);
            default:
                WSASetLastError(WSAETIMEDOUT);
                break;
        	}

            return SOCKET_ERROR;
        }
    }
    else
    {
        WSASetLastError(WSAEWOULDBLOCK);
        return SOCKET_ERROR;
    }
}