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
CAsyncHttp* CAsyncHttp::m_pInstance = 0;

/*static*/ CAsyncHttp* CAsyncHttp::Create()
{
    if ( m_pInstance ) 
        return m_pInstance;

    m_pInstance = new CAsyncHttp( rho::common::createClassFactory());
    return m_pInstance;
}

/*static*/void CAsyncHttp::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

CAsyncHttp::CAsyncHttp(common::IRhoClassFactory* factory) : CThreadQueue(factory)
{
    CThreadQueue::setLogCategory(getLogCategory());

    setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);

    m_pCurCmd = null;
}

CAsyncHttp::~CAsyncHttp(void)
{
    cancelRequest("*", true);
    LOG(INFO) + "Thread shutdown";
}

void CAsyncHttp::addQueueCommand(CQueueCommand* pCmd)
{
    if ( ((CHttpCommand*)pCmd)->m_strCallback.length()==0)
        processCommand(pCmd);
    else
    {
        CThreadQueue::addQueueCommand(pCmd);
        start(epLow);
    }
}

void CAsyncHttp::cancelRequest(const char* szCallback, boolean bWait)
{
    if (!szCallback || !*szCallback )
    {
        LOG(INFO) + "Cancel callback should not be empty. Use * for cancel all";
        return;
    }

    if ( m_pCurCmd != null )
        m_pCurCmd->cancel();

    if ( bWait )
        stop(-1);

    //TODO: find command by callback and cancel it if current, remove if it is still in queue
}

void CAsyncHttp::processCommand(CQueueCommand* pCmd)
{
    m_pCurCmd = (CHttpCommand*)pCmd;
    m_pCurCmd->execute();
    m_pCurCmd = null;
}

extern "C" void header_iter(const char* szName, const char* szValue, void* pHash)
{
    ((Hashtable<String,String>*)pHash)->put(szName, szValue);
}

CAsyncHttp::CHttpCommand::CHttpCommand(
    EHttpCommands eCmd,
    const char* url, unsigned long headers, const char* body,
    const char* file_path,
    const char* callback, const char* callback_params, boolean ssl_verify_peer)
{
    m_strUrl = url != null ? url : "";
    m_strBody = body != null ? body : "";
    m_strFilePath = file_path != null ? file_path : "";
    m_strCallback = callback != null ? callback : "";
    m_strCallbackParams = callback_params != null ? callback_params : "";
    m_eCmd = eCmd;
    m_sslVerifyPeer = ssl_verify_peer;

    rho_ruby_enum_strhash(headers, &header_iter, &m_mapHeaders);

    m_pNetRequest = CAsyncHttp::getInstance()->getFactory()->createNetRequest();
    m_pNetRequest->sslVerifyPeer(m_sslVerifyPeer);

}

void CAsyncHttp::CHttpCommand::cancel()
{
    if (m_pNetRequest!=null && !m_pNetRequest->isCancelled() )
        m_pNetRequest->cancel();
}

void CAsyncHttp::CHttpCommand::execute()
{
    INetResponse* resp = null;
    switch( m_eCmd )
    {
    case hcGet:
        resp = m_pNetRequest->doRequest("GET", m_strUrl, m_strBody, null, &m_mapHeaders);
        break;
    case hcPost:
        resp = m_pNetRequest->doRequest("POST", m_strUrl, m_strBody, null, &m_mapHeaders);
        break;

    case hcDownload:
        resp = m_pNetRequest->pullFile(m_strUrl, m_strFilePath, null, &m_mapHeaders);
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

            resp = m_pNetRequest->pushMultipartData( m_strUrl, arMultipartItems, null, &m_mapHeaders );
            break;
        }
    }

    if ( !m_pNetRequest->isCancelled())
        callNotify(resp,0);
}

unsigned long CAsyncHttp::CHttpCommand::getRetValue()
{
    if ( m_strCallback.length() == 0 )
        return rho_ruby_create_string(m_strResBody.c_str()); 

    return rho_ruby_get_NIL();
}

String CAsyncHttp::CHttpCommand::makeHeadersString()
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

void CAsyncHttp::CHttpCommand::callNotify(rho::net::INetResponse* pResp, int nError )
{
    rho::net::INetResponse& resp = *pResp;

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

        m_strResBody += "&" + RHODESAPP().addCallbackObject(
            new CAsyncHttpResponse(pResp, m_mapHeaders.get("content-type")), "body");
    }

    if ( m_strCallbackParams.length() > 0 )
        m_strResBody += "&" + m_strCallbackParams;

    if ( m_strCallback.length() > 0 )
    {
        String strFullUrl = m_pNetRequest->resolveUrl(m_strCallback);
        m_pNetRequest->pushData( strFullUrl, m_strResBody, null );
    }
}

CAsyncHttp::CAsyncHttpResponse::~CAsyncHttpResponse(){}

extern "C" VALUE rjson_tokener_parse(const char *str, char** pszError );
unsigned long CAsyncHttp::CAsyncHttpResponse::getObjectValue()
{ 
    rho::net::INetResponse& resp = *m_pNetResponse;
    if (resp.isOK())
    {
        if ( m_strContentType.find("application/json") != String::npos )
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

} // namespace net
} // namespace rho

extern "C" {

using namespace rho::net;

unsigned long rho_asynchttp_get(const char* url, unsigned long headers, const char* callback, const char* callback_params, int ssl_verify_peer)
{
    CAsyncHttp::Create();

    CAsyncHttp::CHttpCommand* pHttp = new CAsyncHttp::CHttpCommand(CAsyncHttp::hcGet, url, headers, null, null, callback, callback_params, ssl_verify_peer!=0 );
    CAsyncHttp::getInstance()->addQueueCommand(pHttp);
    return pHttp->getRetValue();
}

unsigned long rho_asynchttp_post(const char* url, unsigned long headers, const char* body, const char* callback, const char* callback_params, int ssl_verify_peer)
{
    CAsyncHttp::Create();

    CAsyncHttp::CHttpCommand* pHttp = new CAsyncHttp::CHttpCommand(CAsyncHttp::hcPost, url, headers, body!=null?body:"", null, callback, callback_params, ssl_verify_peer!=0 );
    CAsyncHttp::getInstance()->addQueueCommand(pHttp);
    return pHttp->getRetValue();
}

unsigned long rho_asynchttp_downloadfile(const char* url, unsigned long headers, const char* file_path, const char* callback, const char* callback_params, int ssl_verify_peer)
{
    CAsyncHttp::Create();

    CAsyncHttp::CHttpCommand* pHttp = new CAsyncHttp::CHttpCommand(CAsyncHttp::hcDownload, url, headers, "", file_path, callback, callback_params, ssl_verify_peer!=0 );
    CAsyncHttp::getInstance()->addQueueCommand(pHttp);
    return pHttp->getRetValue();
}

unsigned long rho_asynchttp_uploadfile(const char* url, unsigned long headers, const char* body, const char* file_path, const char* callback, const char* callback_params, int ssl_verify_peer)
{
    CAsyncHttp::Create();

    CAsyncHttp::CHttpCommand* pHttp = new CAsyncHttp::CHttpCommand(CAsyncHttp::hcUpload, url, headers, body, file_path, callback, callback_params, ssl_verify_peer!=0 );
    CAsyncHttp::getInstance()->addQueueCommand(pHttp);
    return pHttp->getRetValue();
}

void rho_asynchttp_cancel(const char* cancel_callback)
{
    if ( CAsyncHttp::getInstance() )
        CAsyncHttp::getInstance()->cancelRequest(cancel_callback, false);
}

}
