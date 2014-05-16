
#include "..\include\winrtsock.h"
#include "include\InternalSocketManagement.h"

using namespace std;

void __stdcall WSASetLastError(int iError)
{
    GlobalSettings::SetLastError(iError);
}

int __stdcall WSAGetLastError(void)
{
    return GlobalSettings::GetLastError();
}

int __stdcall WSAStartup(WORD wVersionRequired, LPWSADATA lpWSAData)
{
    if (!GlobalSettings::InitializeLibrary())
    {
        ZeroMemory(lpWSAData, sizeof WSADATA);
        return WSASYSNOTREADY;
    }

    lpWSAData->wVersion = wVersionRequired;
    lpWSAData->wHighVersion = wVersionRequired;

    WSASetLastError(0);
    return 0;
}

