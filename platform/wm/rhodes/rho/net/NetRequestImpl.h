#pragma once

#include <wininet.h>
#include "net/INetRequest.h"

namespace rho {
namespace net {

class CNetDataImpl : public INetData
{
    bool   m_bValid;
    String m_data;
    bool   m_bResponseRecieved;
public:
    CNetDataImpl() : m_bValid(false),m_bResponseRecieved(false){}

    bool isValid()const{ return m_bValid; }
    void setValid(bool b){ m_bValid = b; }

    virtual const char* getCharData()
    {
        return m_bValid ? m_data.c_str() : null;
    }

    virtual unsigned int getDataSize()
    {
        return m_bValid ? m_data.size() : 0;
    }

    String& getRawData(){ return m_data; }

    boolean isResponseRecieved()const{ return m_bResponseRecieved;}
    void setResponseRecieved(boolean b){ m_bResponseRecieved = b;}

};

class CNetRequestImpl
{
    DEFINE_LOGCLASS;

    LPCTSTR  pszErrFunction;
    HINTERNET  hInet, hConnection, hRequest;
    URL_COMPONENTS uri;
    CAtlStringW strReqUrlW;
    String      m_strUrl;
    static CNetRequestImpl* m_pInstance;
public :
    CNetRequestImpl(const char* method, const String& strUrl);
    ~CNetRequestImpl();

    void close();
    bool isError(){ return pszErrFunction!= null; }
    CNetDataImpl* sendString(const String& strBody);
    CNetDataImpl* sendStream(common::InputStream* body);

    void ErrorMessage(LPCTSTR pszFunction);
    void readInetFile( void* hRequest, CNetDataImpl* pNetData );
    void alloc_url_components(URL_COMPONENTS *uri, const wchar_t *url);
    void free_url_components(URL_COMPONENTS *uri);
    void readResponse(CNetDataImpl* pNetData);
    bool SetupInternetConnection(LPCTSTR url);

    static void cancel()
    {
        if ( m_pInstance )
        {
            int i = 0;
            if ( m_pInstance->hRequest ) 
                i = 3;

            m_pInstance->close();
        }
    }

};

}
}

