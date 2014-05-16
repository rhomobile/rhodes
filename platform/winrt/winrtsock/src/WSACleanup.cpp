
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

int __stdcall WSACleanup(void)
{
    GlobalSettings::UninitializeLibrary();
    WSASetLastError(0);
    return 0;
}
