#include "net/AsyncHttp.h"
#include "rubyext/RhoRuby.h"
#include "common/StringConverter.h"
#include "net/URI.h"
#include "common/RhodesApp.h"

namespace rho
{
using namespace rho::common;
namespace net
{

IMPLEMENT_LOGCLASS(CAsyncHttp, "AsyncHttp");
common::CMutex CAsyncHttp::m_mxInstances;
VectorPtr<CAsyncHttp*> CAsyncHttp::m_arInstances;

extern "C" void header_iter(const char* szName, const char* szValue, void* pHash)
{
    ((Hashtable<String,String>*)pHash)->put(szName, szValue);
}

CAsyncHttp::CAsyncHttp(common::IRhoClassFactory* factory, EHttpCommands eCmd,
    const char* url, unsigned long headers, const char* body,
    const char* file_path,
    const char* callback, const char* callback_params, boolean ssl_verify_peer) :  CRhoThread(factory)
{
    m_bFinished = false;
    m_ptrFactory = factory;
    m_strUrl = url != null ? url : "";
    m_strBody = body != null ? body : "";
    m_strFilePath = file_path != null ? file_path : "";
    m_strCallback = callback != null ? callback : "";
    m_strCallbackParams = callback_params != null ? callback_params : "";
    m_eCmd = eCmd;
    m_sslVerifyPeer = ssl_verify_peer;

    rho_ruby_enum_strhash(headers, &header_iter, &m_mapHeaders);

    addNewObject(this);

    if (m_strCallback.length()==0)
        run();
    else
        start(epLow);
}

void CAsyncHttp::cancel(boolean bWait)
{
    {		
    	synchronized(m_mxRequest)

        if (m_pNetRequest!=null && !m_pNetRequest->isCancelled() )
            m_pNetRequest->cancel();
    }

    if ( bWait )
        stop(-1);
}

/*static*/ void CAsyncHttp::addNewObject(CAsyncHttp* pObj)
{
	synchronized(m_mxInstances)
    {
        while(1)
        {
            int nToDelete = -1;
            for (int i = 0; i < (int)m_arInstances.size(); i++ )
            {
                if ( m_arInstances.elementAt(i)->m_bFinished )
                {
                    nToDelete = i;
                    break;
                }
            }

            if (nToDelete==-1)
                break;

            m_arInstances.removeElementAt(nToDelete);
        }

        m_arInstances.addElement(pObj);
    }
}

/*static*/ void CAsyncHttp::cancelRequest(const char* szCallback, boolean bWait)
{
    if (!szCallback || !*szCallback )
    {
        LOG(INFO) + "Cancel callback should not be empty. Use * for cancel all";
        return;
    }

	synchronized(m_mxInstances)
    {		
        if ( *szCallback == '*')
        {
            for (int i = 0; i < (int)m_arInstances.size(); i++ )
                m_arInstances.elementAt(i)->cancel(bWait);
        }else
        {
            for (int i = 0; i < (int)m_arInstances.size(); i++ )
            {
                if ( m_arInstances.elementAt(i)->m_strCallback.compare(szCallback) == 0 )
                    m_arInstances.elementAt(i)->cancel(bWait);    
            }
        }
    }
}

void CAsyncHttp::run()
{
    LOG(INFO) + "RhoHttp thread start.";

    {		
        synchronized(m_mxRequest)
        m_pNetRequest = m_ptrFactory->createNetRequest();
        m_pNetRequest->sslVerifyPeer(m_sslVerifyPeer);
    }

    switch( m_eCmd )
    {
    case hcGet:
        m_pNetResponse = m_pNetRequest->doRequest("GET", m_strUrl, m_strBody, null, &m_mapHeaders);
        break;
    case hcPost:
        m_pNetResponse = m_pNetRequest->doRequest("POST", m_strUrl, m_strBody, null, &m_mapHeaders);
        break;

    case hcDownload:
        m_pNetResponse = m_pNetRequest->pullFile(m_strUrl, m_strFilePath, null, &m_mapHeaders);
        break;

    case hcUpload:
        {
            VectorPtr<net::CMultipartItem*> arMultipartItems;

            net::CMultipartItem* pItem = new net::CMultipartItem();
            pItem->m_strFilePath = m_strFilePath;
            pItem->m_strContentType = "application/octet-stream";
            arMultipartItems.addElement(pItem);

            if ( m_strBody.length() > 0 )
            {
                net::CMultipartItem* pItem = new net::CMultipartItem();
                pItem->m_strBody = m_strBody;
                pItem->m_strContentType = m_mapHeaders.get("content-type");
                arMultipartItems.addElement(pItem);
            }

            m_pNetResponse = m_pNetRequest->pushMultipartData( m_strUrl, arMultipartItems, null, &m_mapHeaders );
            break;
        }
    }

    if ( !m_pNetRequest->isCancelled())
    {
        //processResponse(*m_pNetResponse);
        callNotify(*m_pNetResponse,0);
    }

	LOG(INFO) + "RhoHttp thread end.";

    m_bFinished = true;
}

String CAsyncHttp::makeHeadersString()
{
    String strRes = "";

    for ( Hashtable<String,String>::iterator it = m_mapHeaders.begin();  it != m_mapHeaders.end(); ++it )
    {
        if ( strRes.length() > 0)
            strRes += "&";

        strRes += "headers[";
        URI::urlEncode(it->first,strRes);
        strRes += "]=";
        URI::urlEncode(it->second,strRes);
    }

    return strRes;
}

extern "C" VALUE rjson_tokener_parse(const char *str, char** pszError );
unsigned long CAsyncHttp::getObjectValue()
{ 
    rho::net::INetResponse& resp = *m_pNetResponse;
    if (resp.isOK())
    {
        String strContType = m_mapHeaders.get("content-type");
        if ( strContType.find("application/json") != String::npos )
        {
            char* szError = 0;
            unsigned long valBody = rjson_tokener_parse(resp.getCharData(), &szError);
            if ( valBody != 0 )
                return valBody;

            LOG(ERROR) + "Incorrect json body.Error:" + (szError ? szError : "");
            if ( szError )
                free(szError);
        }
    }

    return rho_ruby_create_string(resp.getCharData()); 
}

unsigned long CAsyncHttp::getRetValue()
{
    if ( m_strCallback.length() == 0 )
        return rho_ruby_create_string(m_strResBody.c_str()); 

    return rho_ruby_get_NIL();
}

void CAsyncHttp::callNotify(rho::net::INetResponse& resp, int nError )
{
    m_strResBody = "rho_callback=1";
    m_strResBody += "&status=";
    if ( nError > 0 )
    {
    	m_strResBody += "error&error_code=" + nError;
    }else
    {
        if ( resp.isOK() )
    	    m_strResBody += "ok";
        else
        {
    	    m_strResBody += "error&error_code=";
            m_strResBody += convertToStringA(RhoRuby.getErrorFromResponse(resp));
            if ( resp.isResponseRecieved())
	            m_strResBody += "&http_error=" + convertToStringA(resp.getRespCode());
        }

        String cookies = resp.getCookies();
        if (cookies.length()>0)
            m_strResBody += "&cookies=" + URI::urlEncode(cookies);

        String strHeaders = makeHeadersString();
        if (strHeaders.length() > 0 )
            m_strResBody += "&" + strHeaders;

        m_strResBody += "&" + RHODESAPP().addCallbackObject(this, "body");
    }

    if ( m_strCallbackParams.length() > 0 )
        m_strResBody += "&" + m_strCallbackParams;

    if ( m_strCallback.length() > 0 )
    {
        common::CAutoPtr<INetRequest> pNetRequest = m_ptrFactory->createNetRequest();
        String strFullUrl = pNetRequest->resolveUrl(m_strCallback);
        NetResponse(resp1,pNetRequest->pushData( strFullUrl, m_strResBody, null ));
        if ( !resp1.isOK() )
            LOG(ERROR) + "AsyncHttp notification failed. Code: " + resp1.getRespCode() + "; Error body: " + resp1.getCharData();
    }
}

} // namespace net
} // namespace rho

extern "C" {

using namespace rho::net;

unsigned long rho_asynchttp_get(const char* url, unsigned long headers, const char* callback, const char* callback_params, int ssl_verify_peer)
{
    CAsyncHttp* pHttp = new CAsyncHttp(rho::common::createClassFactory(), CAsyncHttp::hcGet, url, headers, null, null, callback, callback_params, ssl_verify_peer!=0 );
    return pHttp->getRetValue();
}

unsigned long rho_asynchttp_post(const char* url, unsigned long headers, const char* body, const char* callback, const char* callback_params, int ssl_verify_peer)
{
    CAsyncHttp* pHttp = new CAsyncHttp(rho::common::createClassFactory(), CAsyncHttp::hcPost, url, headers, body!=null?body:"", null, callback, callback_params, ssl_verify_peer!=0 );
    return pHttp->getRetValue();
}

unsigned long rho_asynchttp_downloadfile(const char* url, unsigned long headers, const char* file_path, const char* callback, const char* callback_params, int ssl_verify_peer)
{
    CAsyncHttp* pHttp = new CAsyncHttp(rho::common::createClassFactory(), CAsyncHttp::hcDownload, url, headers, "", file_path, callback, callback_params, ssl_verify_peer!=0 );
    return pHttp->getRetValue();
}

unsigned long rho_asynchttp_uploadfile(const char* url, unsigned long headers, const char* body, const char* file_path, const char* callback, const char* callback_params, int ssl_verify_peer)
{
    CAsyncHttp* pHttp = new CAsyncHttp(rho::common::createClassFactory(), CAsyncHttp::hcUpload, url, headers, body, file_path, callback, callback_params, ssl_verify_peer!=0 );
    return pHttp->getRetValue();
}

void rho_asynchttp_cancel(const char* cancel_callback)
{
    CAsyncHttp::cancelRequest(cancel_callback, false);
}

void rho_asynchttp_destroy()
{
    CAsyncHttp::cancelRequest("*", true);
}

}
