
#include "..\include\winrtsock.h"
#include "Include\InternalSocketManagement.h"

using namespace Platform;
using namespace Windows::Networking;
using namespace Windows::Networking::Connectivity;
using namespace Windows::Foundation::Collections;

int __stdcall gethostname(char * name, int /*namelen*/)
{
    if (!GlobalSettings::IsLibraryInitialized())
    {
        WSASetLastError(WSANOTINITIALISED);
        return SOCKET_ERROR;
    }

    IVectorView<HostName^>^ hostnames;

    hostnames = NetworkInformation::GetHostNames();

    String^ domainname;
    bool found = false;

    for (unsigned int i = 0; i < hostnames->Size; i++)
    {
        auto entry = hostnames->GetAt(i);

        if (entry->Type == HostNameType::DomainName)
        {
            domainname = entry->CanonicalName;
            found = true;
            break;
        }
    }
   
    if (domainname == nullptr)
    {
        WSASetLastError(0);
        return SOCKET_ERROR;
    }

    const wchar_t * domainNameWide = domainname->Data();

    wcstombs(name, domainNameWide, domainname->Length());
    name[domainname->Length()] = 0;
   
    WSASetLastError(0);
    return 0;
}