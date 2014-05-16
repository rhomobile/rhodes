
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

using namespace std;
using namespace concurrency;
using namespace Windows::Networking;
using namespace Platform;

int __stdcall send(SOCKET s, const char * buf, int len, int /*flags*/)
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
     
    int bytesSent = 0;

    auto socket = socketHandle->GetSocketAs<IRWSocket>();

    if (!socketHandle->IsNonblocking())
    {
        try
        {
            bytesSent = socket->Send(buf, len);
            WSASetLastError(0);
            return bytesSent;
        }
        catch (SocketSendException& e)
        {
            auto reason = e.GetReason();

            switch (reason)
            {
            case SocketSendExceptionReason::Unknown:
                WSASetLastError(WSAETIMEDOUT);
            	break;
            default:
                WSASetLastError(WSAETIMEDOUT);
            }

            return SOCKET_ERROR;
        }
    }
    else
    {
        WSASetLastError(WSAEWOULDBLOCK);
        socket->Send(buf, len);
        return len;
    }

    return SOCKET_ERROR;
}