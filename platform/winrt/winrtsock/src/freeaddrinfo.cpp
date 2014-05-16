
#include "..\include\winrtsock.h"
#include "..\include\winrtsockEx.h"
#include "Include\InternalSocketManagement.h"

using namespace std;

void  __stdcall freeaddrinfo(PADDRINFOA pAddrInfo)
{
    if (!pAddrInfo)
        return;

    struct addrinfo *next = nullptr;

    for (; pAddrInfo; pAddrInfo = next) 
    {
        next = pAddrInfo->ai_next;
        if (pAddrInfo->ai_canonname) 
            delete [] pAddrInfo->ai_canonname;
        
        if (pAddrInfo->ai_addr)
            delete pAddrInfo->ai_addr;
        
        delete pAddrInfo;
    }
}
    