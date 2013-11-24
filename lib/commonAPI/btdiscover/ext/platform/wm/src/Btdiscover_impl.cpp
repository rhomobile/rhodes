#include "../../../shared/generated/cpp/BtdiscoverBase.h"
#include <winsock2.h>
#include <bt_api.h>
#include "logging/RhoLog.h"

namespace rho {

using namespace apiGenerator;

class CBtdiscoverImpl: public CBtdiscoverBase
{
public:
    CBtdiscoverImpl(const rho::String& strID): CBtdiscoverBase()
    {
    }

};

class CBtdiscoverSingleton: public CBtdiscoverSingletonBase
{
    ~CBtdiscoverSingleton(){}

    virtual void enumerate(CMethodResult& oResult);
};

class CBtdiscoverFactory: public CBtdiscoverFactoryBase
{
    ~CBtdiscoverFactory(){}
    virtual IBtdiscoverSingleton* createModuleSingleton();
    virtual IBtdiscover* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Btdiscover_extension()
{
    CBtdiscoverFactory::setInstance( new CBtdiscoverFactory() );
    Init_Btdiscover_API();
}

IBtdiscover* CBtdiscoverFactory::createModuleByID(const rho::String& strID)
{
    return new CBtdiscoverImpl(strID);
}

IBtdiscoverSingleton* CBtdiscoverFactory::createModuleSingleton()
{
    return new CBtdiscoverSingleton();
}

void CBtdiscoverSingleton::enumerate(CMethodResult& oResult)
{
    rho::Hashtable<rho::String, rho::String> devices;



    INT             iResult = 0;
    LPWSAQUERYSET   pwsaResults;
    DWORD           dwSize = 0;
    WSAQUERYSET     wsaq;
    HANDLE          hLookup = 0;
    TCHAR           btAddr[40];
    
    
    memset (&wsaq, 0, sizeof(wsaq));
    wsaq.dwSize      = sizeof(wsaq);
    wsaq.dwNameSpace = NS_BTH;
    wsaq.lpcsaBuffer = NULL;
    
    // initialize searching procedure
    iResult = WSALookupServiceBegin(&wsaq, 
        LUP_CONTAINERS, 
        &hLookup);
    
    if (iResult != 0)
    {
        TCHAR tszErr[32];
        iResult = WSAGetLastError();        
        StringCchPrintf(tszErr, 32, _T("Socket Error: %d"), iResult);
        oResult.set(devices);
        return;
    }
    
    union {
        CHAR buf[5000];             // returned struct can be quite large 
        SOCKADDR_BTH    __unused;   // properly align buffer to BT_ADDR requirements
    };
    
    
    for (; ;)
    {
        // set the wait cursor while searching
        pwsaResults = (LPWSAQUERYSET) buf;
        
        dwSize  = sizeof(buf);
        
        memset(pwsaResults,0,sizeof(WSAQUERYSET));
        pwsaResults->dwSize      = sizeof(WSAQUERYSET);
        // namespace MUST be NS_BTH for bluetooth queries
        pwsaResults->dwNameSpace = NS_BTH;
        pwsaResults->lpBlob      = NULL;
        
        // iterate through all found devices, returning name and address
        // (this sample only uses the name, but address could be used for
        // further queries)
        iResult = WSALookupServiceNext (hLookup, 
            LUP_RETURN_NAME | LUP_RETURN_ADDR, 
            &dwSize, 
            pwsaResults);
        
        
        if (iResult != 0)
        {
            iResult = WSAGetLastError();
            if (iResult != WSA_E_NO_MORE)
            {
                TCHAR tszErr[32];
                iResult = WSAGetLastError();
                StringCchPrintf(tszErr, 32, _T("Socket Error: %d"), iResult);
                // XXX print message        
                
            }
            // we're finished

            break;
        }
        
        // add the name and mac to the return results
        if (pwsaResults->lpszServiceInstanceName && pwsaResults->lpcsaBuffer)
        {
            SOCKADDR_BTH *sa = (SOCKADDR_BTH*)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr;
            wsprintf (btAddr,L"%04X%08X", GET_NAP(sa->btAddr), GET_SAP(sa->btAddr));
            devices.put(rho::common::convertToStringA(pwsaResults->lpszServiceInstanceName), rho::common::convertToStringA((LPTSTR)btAddr) );
        }
    }
    
    WSALookupServiceEnd(hLookup);
    oResult.set(devices);

}
}