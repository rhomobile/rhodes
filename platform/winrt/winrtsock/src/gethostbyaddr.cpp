
#include "..\include\winrtsock.h"
#include <string>
#include <ppltasks.h>
#include <memory>
#include "include\MiscUtils.h"
#include "include\InternalSocketManagement.h"

using namespace std;
using namespace concurrency;
using namespace Platform;
using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Foundation::Collections;

hostent * __stdcall gethostbyaddr(const char * addr, int /*len*/, int type)
{
    if (!GlobalSettings::IsLibraryInitialized())
    {
        WSASetLastError(WSANOTINITIALISED);
        return nullptr;
    }
   
    if (type != AF_INET)
    {
        WSASetLastError(WSAEINVAL);
        return nullptr;
    }

    auto hostName = CharToPlatformString((unsigned char * const)addr);
    auto host = ref new HostName(hostName);

    int lastError = 0;

    hostent* hst = GlobalSettings::GetHostEntry();

    auto dummySocket = ref new DatagramSocket();

    create_task(dummySocket->GetEndpointPairsAsync(host, "0", HostNameSortOptions::OptimizeForLongConnections))
        .then([&lastError, hst, host, addr, type](task<IVectorView <EndpointPair^>^> resulttask)
    {
        bool found = false;
        int nameCounter = 0;

        try
        {
            auto data = resulttask.get();
            auto size = data->Size;

            if (size != 0)
            {
                String^ stringIp;

                for (unsigned int i = size - 1; i < size; i--)
                {
                    EndpointPair^ pair = data->GetAt(i);
                    if (pair->RemoteHostName->Type == HostNameType::Ipv4 && nameCounter < MAX_HOST_ENTRY_LIST_ITEMS - 1)
                    {
                        stringIp = pair->RemoteHostName->CanonicalName;
                        
                        auto networkType = pair->RemoteHostName->IPInformation->NetworkAdapter->NetworkItem->GetNetworkTypes();

                        int nameLen = strlen(addr) + 1;
                        if (hst->h_name)
                            delete [] hst->h_name;

                        hst->h_name = new char[nameLen];
                        strcpy_s(hst->h_name, nameLen, addr);

                        char ipChar[MAX_IP_ADDRESS_CHAR_LEN];
                        const wchar_t * ipWide = stringIp->Data();
                        int stringIpCharLen = stringIp->Length();

                        wcstombs(ipChar, ipWide, stringIpCharLen);
                        ipChar[stringIpCharLen] = 0;

                        auto addr = inet_addr(ipChar);
                        ipChar[stringIpCharLen] = 0;

                        memcpy(hst->h_addr_list[nameCounter], &addr, sizeof(struct in_addr));

                        found = true;
                        nameCounter++;
                    }
                }
            }
        }
        catch (Exception^)
        {
            lastError = WSAHOST_NOT_FOUND;
        }

        if (!found)
        {
            hst->h_addr_list[0] = nullptr;
            lastError = WSAHOST_NOT_FOUND;
            return;
        }

        hst->h_addr_list[nameCounter + 1] = nullptr;

    }).get();

    if (lastError != 0)
    {
        WSASetLastError(lastError);
        return nullptr;
    }

    hst->h_aliases = nullptr;
    hst->h_addrtype = static_cast<short>(type);
    hst->h_length = 4;

    return hst;
}