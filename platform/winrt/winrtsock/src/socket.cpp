
#include "..\include\winrtsock.h"
#include "include\InternalSocketManagement.h"

using namespace std;

SOCKET __stdcall socket(int af, int type, int /*protocol*/)
{
	if (af == AF_INET && type == SOCK_STREAM)
	{
        auto socketHandle = SocketManagement::CreateSocket(InternalSocketType::Stream);

        if (socketHandle == nullptr)
            return (UINT_PTR)SOCKET_ERROR;

        return socketHandle->GetHandle();
	}

    if (af == AF_INET && type == SOCK_DGRAM)
    {
        auto socketHandle = SocketManagement::CreateSocket(InternalSocketType::Datagram);

        if (socketHandle == nullptr)
            return (UINT_PTR) SOCKET_ERROR;

        return socketHandle->GetHandle();
    }

	return INVALID_SOCKET;
}