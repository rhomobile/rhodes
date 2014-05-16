
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

int __stdcall getsockopt(SOCKET s, int level, int optname, char * optval, int * /*optlen*/)
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

    if (level == SOL_SOCKET)
    {
        switch (optname)
        {
        case SO_ERROR:
            {
                if (!socketHandle->IsSocketCreated())
                {
                    *(int*) optval = 0;
                    WSASetLastError(0);
                    return 0;
                }

                auto socket = socketHandle->GetSocketAs<IRWSocket>();

                if (socket == nullptr)
                {
                    *(int*) optval = 0;
                    WSASetLastError(0);
                    return 0;
                }

                InternalSocketConnectionStatus currentStatus = socket->GetConnectionStatus();

                if (currentStatus == InternalSocketConnectionStatus::Connecting ||
                    currentStatus == InternalSocketConnectionStatus::Connected)
                {
                    *(int*) optval = 0;
                    WSASetLastError(0);
                    return 0;
                }
                else
                {
                    *(int*) optval = WSAESHUTDOWN;
                    WSASetLastError(0);
                    return 0;
                }
            }
            
            break;
        case SO_SNDBUF:
            {
                int buffsize = socketHandle->GetWriteBufferSize();
                *(int*)optval = buffsize;
                WSASetLastError(0);
                return 0;
            }
            break;
        case SO_RCVBUF:
            {
                int buffsize = socketHandle->GetReadBufferSize();
                *(int*) optval = buffsize;
                WSASetLastError(0);
                return 0;
            }
            break;
        }
    }

    return SOCKET_ERROR;
}