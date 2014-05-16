
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

using namespace std;

int __stdcall listen(SOCKET s, int /*backlog*/)
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

    socketHandle->SetSocketType(InternalSocketType::Listener);

    auto socket = socketHandle->GetSocketAs<InternalSocketListener>();

    try
    {
        socket->StartListening();
        WSASetLastError(0);
        return 0;
    }
    catch (SocketListenException& ex)
    {
        int error = 0;
        switch (ex.GetReason())
        {
        case SocketListenExceptionReason::AddressInUse:
            error = WSAEADDRINUSE;
        default:
            break;
        }
        WSASetLastError(error);
        return SOCKET_ERROR;
    }

    return SOCKET_ERROR;
}