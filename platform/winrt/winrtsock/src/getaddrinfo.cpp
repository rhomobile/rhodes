
#include "..\include\winrtsock.h"
#include "..\include\winrtsockEx.h"
#include "Include\InternalSocketManagement.h"

using namespace std;

int QueryByAddress(const PCSTR pServiceName, uint32 addr, const ADDRINFOA * pHints, PADDRINFOA * ppResult);
int QueryByName(const PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA * pHints, PADDRINFOA * ppResult);
addrinfo *CreateNewAddrInfoItem(const ADDRINFOA * pHints, in_addr ip, unsigned short port);

int  __stdcall getaddrinfo(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA * pHints, PADDRINFOA * ppResult)
{
    if (!GlobalSettings::IsLibraryInitialized())
    {
        WSASetLastError(WSANOTINITIALISED);
        return SOCKET_ERROR;
    }

    addrinfo hints;

    if (pHints == nullptr)
    {
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
    }
    else
    {
        memcpy(&hints, pHints, sizeof(hints));
    }

    if (!hints.ai_socktype) 
    {
        hints.ai_socktype = SOCK_STREAM;
    }

    int error = 0;

    if (hints.ai_family != AF_INET && hints.ai_family != AF_UNSPEC)
    {
        WSASetLastError(WSAEAFNOSUPPORT);
        return WSAEAFNOSUPPORT;
    }

    if (!pNodeName && !pServiceName) 
    {
        WSASetLastError(WSAHOST_NOT_FOUND);
        return WSAHOST_NOT_FOUND;
    }

    if (pNodeName)
    {
        if (pNodeName[0] == '\0') 
        {
            return QueryByAddress(pServiceName, INADDR_ANY, &hints, ppResult);
        }
        else if (hints.ai_flags & AI_NUMERICHOST) 
        {
            in_addr address;
            if (inet_pton(AF_INET, pNodeName, &address) <= 0)
            {
                WSASetLastError(WSAEAFNOSUPPORT);
                return WSAEAFNOSUPPORT;
            }

            error = QueryByAddress(pServiceName, ntohl(address.s_addr), &hints, ppResult);
            WSASetLastError(error);
            return error;
        }
        else 
        {
            error = QueryByName(pNodeName, pServiceName, &hints, ppResult);
            WSASetLastError(error);
            return error;
        }
    }
    else if (hints.ai_flags & AI_PASSIVE) 
    {
        return WSAHOST_NOT_FOUND;
    }
    return WSAHOST_NOT_FOUND;
}

int QueryByName(const PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA * pHints, PADDRINFOA * ppResult)
{
    addrinfo *addressInfoList = nullptr, *previousItem = nullptr;
    char **pptr = nullptr;
    
    hostent *hostEntry = nullptr;
    unsigned short port = 0;

    int err = 0;

    if (pServiceName)
    {
        port = (unsigned short) atoi(pServiceName);
    }

    hostEntry = gethostbyname(pNodeName);
    err = WSAGetLastError();
    if (err) 
    {
        return err;
    }

    for (pptr = hostEntry->h_addr_list; *pptr; pptr++)
    {
        in_addr ip = *(in_addr *)*pptr;
        addrinfo *addressInfo = CreateNewAddrInfoItem(pHints, ip, port);

        if (!addressInfoList) 
        {
            addressInfoList = addressInfo;
            previousItem = addressInfo;
            addressInfo->ai_canonname = _strdup(hostEntry->h_name);
        }
        else
        {
            previousItem->ai_next = addressInfo;
            previousItem = addressInfo;
        }
    }
    *ppResult = addressInfoList;
    return 0;
}

int QueryByAddress(const PCSTR pServiceName, uint32 addr, const ADDRINFOA * pHints, PADDRINFOA * ppResult)
{
    addrinfo *addressInfo = nullptr;
    in_addr ipAddr;
    unsigned short port = 0;

    if (pServiceName)
        port = static_cast<unsigned short>(atoi(pServiceName));
    
    ipAddr.s_addr = htonl(addr);

    addressInfo = CreateNewAddrInfoItem(pHints, ipAddr, port);

    if (!(pHints->ai_flags & AI_NUMERICSERV) && pHints->ai_flags & AI_CANONNAME)
    {
            if (addr != INADDR_LOOPBACK && addr != INADDR_ANY)
            {
                char host[200];
                ZeroMemory(host, sizeof host);
                //this really should be gethostbyaddr
                int error = gethostname(host, sizeof host /* AF_INET*/);

                if (!error)
                    return error;

                addressInfo->ai_canonname = _strdup(host);
            }

            if (addressInfo->ai_canonname == nullptr) 
            {
                freeaddrinfo(addressInfo);
                return WSAHOST_NOT_FOUND;
            }
    }

    *ppResult = addressInfo;
    return 0;
}

addrinfo *CreateNewAddrInfoItem(const ADDRINFOA * pHints, in_addr ip, unsigned short port)
{
    sockaddr_in *addrIn = nullptr;

    addrinfo *addressInfo = new addrinfo;
    ZeroMemory(addressInfo, sizeof addrinfo);

    addrIn = new sockaddr_in;
    ZeroMemory(addrIn, sizeof sockaddr_in);

    addrIn->sin_family = AF_INET;
    addrIn->sin_port = htons(port);
    addrIn->sin_addr = ip;

    addressInfo->ai_flags = 0;
    addressInfo->ai_family = AF_INET;
    addressInfo->ai_socktype = pHints->ai_socktype;
    addressInfo->ai_protocol = pHints->ai_protocol;
    addressInfo->ai_addrlen = sizeof(*addrIn);
    addressInfo->ai_addr = (sockaddr *) addrIn;
    addressInfo->ai_canonname = nullptr;
    addressInfo->ai_next = nullptr;

    return addressInfo;
}
