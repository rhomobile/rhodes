#pragma once

#include <wininet.h>
#include "net/INetRequest.h"

namespace rho {
namespace common {
    class CRhoFile;
}

namespace net {

class CNetResponseImpl : public INetResponse
{
    bool   m_bValid;
    String m_data;
    int    m_nRespCode;
public:
    CNetResponseImpl() : m_bValid(false),m_nRespCode(-1){}

    bool isValid()const{ return m_bValid; }
    void setValid(bool b){ m_bValid = b; }

    virtual const char* getCharData()
    {
        return m_bValid ? m_data.c_str() : "";
    }

    virtual unsigned int getDataSize()
    {
        return m_bValid ? m_data.size() : 0;
    }

    String& getRawData(){ return m_data; }

    boolean isResponseRecieved(){ return m_nRespCode!=-1;}
    void setResponseCode(int nRespCode){ m_nRespCode = nRespCode;}

    int getRespCode(){ return m_nRespCode;}

	boolean isOK()
	{
		return m_nRespCode == 200;
	}

    boolean isUnathorized()
    {
        return m_nRespCode == 401;
    }

};

class CNetRequest;
class CNetRequestImpl
{
    DEFINE_LOGCLASS;

    LPCTSTR  pszErrFunction;
    HINTERNET  hInet, hConnection, hRequest;
    URL_COMPONENTS uri;
    CAtlStringW strReqUrlW;
    String      m_strUrl;
    CNetRequest* m_pParent;
public :
    CNetRequestImpl(CNetRequest* pParent, const char* method, const String& strUrl);
    ~CNetRequestImpl();

    void close();
    bool isError(){ return pszErrFunction!= null; }
    CNetResponseImpl* sendString(const String& strBody);
    CNetResponseImpl* sendStream(common::InputStream* body);
    CNetResponseImpl* downloadFile(common::CRhoFile& oFile);

    void ErrorMessage(LPCTSTR pszFunction);
    void readInetFile( void* hRequest, CNetResponseImpl* pNetData, common::CRhoFile* pFile = NULL );
    void alloc_url_components(URL_COMPONENTS *uri, const wchar_t *url);
    void free_url_components(URL_COMPONENTS *uri);
    void readResponse(CNetResponseImpl* pNetData);
    bool SetupInternetConnection(LPCTSTR url);
};

}
}

