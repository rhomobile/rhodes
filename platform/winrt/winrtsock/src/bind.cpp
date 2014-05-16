
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"
#include <string>
#include <random>

using namespace std;

int __stdcall bind(SOCKET s, const struct sockaddr FAR *addr, int namelen)
{
    if (!GlobalSettings::IsLibraryInitialized())
    {
        WSASetLastError(WSANOTINITIALISED);
        return SOCKET_ERROR;
    }

    if (namelen != sizeof sockaddr_in)
    {
        WSASetLastError(WSAEINVAL);
        return SOCKET_ERROR;
    }

    auto socketHandle = SocketManagement::GetSocket(s);

    if (socketHandle == nullptr)
    {
        WSASetLastError(WSAENOTSOCK);
        return SOCKET_ERROR;
    }

    auto sockAddr = reinterpret_cast<const sockaddr_in*>(addr);
    
    wstring bindAddress = GetWideAddressStringFromSocketAddress(sockAddr);
    int bindPort        = ntohs(sockAddr->sin_port);

    int lastError = 0;

    try
    {
        if (bindPort == 0)
        {
            LARGE_INTEGER li;
            QueryPerformanceCounter(&li);
            
            std::default_random_engine rng(li.QuadPart);

            std::uniform_int_distribution<int> gen(20000, 64000);

            bindPort = gen(rng);
        }
        socketHandle->SetBindSettings(bindAddress, bindPort);
    }
    catch (SocketBindException& ex)
    {
        auto reason = ex.GetReason();

        switch (reason)
        {
        case SocketBindExceptionReason::Unknown:
            lastError = WSAEINVAL;
            break;
        case SocketBindExceptionReason::AccessDenied:
            lastError = WSAEACCES;
            break;
        case SocketBindExceptionReason::AddressInUse:
            lastError = WSAEADDRINUSE;
            break;
        case SocketBindExceptionReason::AddressNotAvailable:
            lastError = WSAEADDRNOTAVAIL;
            break;
        default:
            break;
        }
    }
    catch (exception&)
    {
        lastError = WSAEINVAL;
    }

    WSASetLastError(lastError);

    return lastError == 0 ? 0 : SOCKET_ERROR;
}