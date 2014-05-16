#pragma once
#include "InternalSocket.h"
#include "..\..\Include\winrtsock.h"
#include <memory>
#include "include\SocketHandle.h"

const int MAX_IP_ADDRESS_CHAR_LEN = 16;
const int MAX_HOST_NAME_CHAR_LEN = 4096;
const int MAX_HOST_ENTRY_LIST_ITEMS = 20;

class SocketManagement
{
public:
    static std::shared_ptr<InternalSocketHandle> CreateSocketHandleFromStreamSocket(Windows::Networking::Sockets::StreamSocket^ socketTyp);
    static std::shared_ptr<InternalSocketHandle> CreateSocket(InternalSocketType socketType);
    static void CloseSocket(SOCKET s);
    static std::shared_ptr<InternalSocketHandle> GetSocket(SOCKET s);
};

class GlobalSettings
{
public:
    static bool InitializeLibrary();
    static void UninitializeLibrary();
    static bool IsLibraryInitialized();
    static int GetLastError();
    static void SetLastError(int errorno);
    static char* Get_inet_ntoa_String();
    static HOSTENT* GetHostEntry();
};