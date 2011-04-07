#include "net/AsyncHttp.h"
#include "common/StringConverter.h"
#include "net/URI.h"
#include "common/RhodesApp.h"
#include "common/RhoAppAdapter.h"

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

    m_pInstance = new CAsyncHttp( );
    return m_pInstance;
}

/*static*/void CAsyncHttp::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

CAsyncHttp::CAsyncHttp() : CThreadQueue()
{
    CThreadQueue::setLogCategory(getLogCategory());

    setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);
}

CAsyncHttp::~CAsyncHttp(void)
{
    stop(-1);
    LOG(INFO) + "Thread shutdown";
}

unsigned long CAsyncHttp::addHttpCommand(IQueueCommand* pCmd)
{
    if ( ((CHttpCommand*)pCmd)->m_strCallback.length()==0)
    {
        processCommandBase(pCmd);
        unsigned long ret = ((CHttpCommand*)pCmd)->getRetValue();
        delete pCmd;
        return ret;
    }

    CThreadQueue::addQueueCommand(pCmd);
    start(epLow);

    return ((CHttpCommand*)pCmd)->getRetValue();
}

void CAsyncHttp::cancelRequest(const char* szCallback)
{
    if (!szCallback || !*szCallback )
    {
        LOG(INFO) + "Cancel callback should not be empty. Use * for cancel all";
        return;
    }

    synchronized(getCommandLock());
    CHttpCommand* pCmd = (CHttpCommand*)getCurCommand();

    if ( pCmd != null && ( *szCallback == '*' || pCmd->m_strCallback.compare(szCallback) == 0) )
        pCmd->cancel();

    if ( *szCallback == '*' )
        getCommands().clear();
    else
    {
        for (int i = getCommands().size()-1; i >= 0; i--)
        {
            CHttpCommand* pCmd1 = (CHttpCommand*)getCommands().get(i);

            if ( pCmd1 != null && pCmd1->m_strCallback.compare(szCallback) == 0 )
                getCommands().remove(i);
        }

    }
}

void CAsyncHttp::processCommand(IQueueCommand* pCmd)
{
    ((CHttpCommand*)pCmd)->execute();
}

extern "C" void header_iter(const char* szName, const char* szValue, void* pHash)
{
    ((Hashtable<String,String>*)pHash)->put(szName, szValue);
}

CAsyncHttp::CHttpCommand::CHttpCommand(String strCmd, rho_param *p) : m_params(p)
{
    m_eCmd = translateCommand(strCmd);
    m_strCallback = m_params.getString("callback");
    m_strCallbackParams = m_params.getString("callback_param");

    m_params.getHash("headers", m_mapHeaders);

    if(m_params.has("ssl_verify_peer"))
        m_NetRequest.setSslVerifyPeer(m_params.getBool("ssl_verify_peer"));
}

void CAsyncHttp::CHttpCommand::execute()
{
    NetResponse resp;
    switch( m_eCmd )
    {
    case hcGet:
        resp = getNet().doRequest( m_params.getString("http_command", "GET").c_str(),
            m_params.getString("url"), m_params.getString("body"), null, &m_mapHeaders);
        break;
    case hcPost:
        resp = getNet().doRequest(m_params.getString("http_command", "POST").c_str(),
            m_params.getString("url"), m_params.getString("body"), null, &m_mapHeaders);
        break;

    case hcDownload:
        resp = getNet().pullFile(m_params.getString("url"), m_params.getString("filename"), null, &m_mapHeaders);
        break;

    case hcUpload:
        {
            VectorPtr<net::CMultipartItem*> arMultipartItems;

            CRhoParamArray arParams( m_params, "multipart");
            if ( arParams.size() > 0 )
            {
                for( int i = 0; i < arParams.size(); i++)
                {
                    CRhoParams oItem = arParams.getItem(i);

                    net::CMultipartItem* pItem = new net::CMultipartItem();
                    String strFilePath = oItem.getString("filename");
                    if ( strFilePath.length() == 0 )
                    {
                        pItem->m_strBody = oItem.getString("body");
                        pItem->m_strContentType = oItem.getString("content_type", "");
                    }
                    else
                    {
                        pItem->m_strFilePath = strFilePath;
                        pItem->m_strContentType = oItem.getString("content_type", "application/octet-stream");
                    }

                    pItem->m_strName = oItem.getString("name");
                    pItem->m_strFileName = oItem.getString("filename_base");
                    arMultipartItems.addElement(pItem);
                }
            }else
            {
                net::CMultipartItem* pItem = new net::CMultipartItem();
                pItem->m_strFilePath = m_params.getString("filename");
                pItem->m_strContentType = m_params.getString("file_content_type", "application/octet-stream");
                pItem->m_strName = m_params.getString("name");
                pItem->m_strFileName = m_params.getString("filename_base");
                arMultipartItems.addElement(pItem);

                String strBody = m_params.getString("body");
                if ( strBody.length() > 0 )
                {
                    net::CMultipartItem* pItem2 = new net::CMultipartItem();
                    pItem2->m_strBody = strBody;
                    pItem2->m_strContentType = m_mapHeaders.get("content-type");
                    arMultipartItems.addElement(pItem2);
                }
            }

            resp = getNet().pushMultipartData( m_params.getString("url"), arMultipartItems, null, &m_mapHeaders );
            break;
        }
    }

    if ( !m_NetRequest.isCancelled())
        callNotify(resp,0);

    m_params.free_params();
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

void CAsyncHttp::CHttpCommand::callNotify(NetResponse& resp, int nError )
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
            m_strResBody += convertToStringA(RhoAppAdapter.getErrorFromResponse(resp));
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
            new CAsyncHttpResponse(resp, m_mapHeaders.get("content-type")), "body");
    }

    if ( m_strCallbackParams.length() > 0 )
        m_strResBody += "&" + m_strCallbackParams;

    if ( m_strCallback.length() > 0 )
    {
        String strFullUrl = m_NetRequest.resolveUrl(m_strCallback);
        getNet().pushData( strFullUrl, m_strResBody, null );
    }
}

CAsyncHttp::CAsyncHttpResponse::~CAsyncHttpResponse(){}

extern "C" VALUE rjson_tokener_parse(const char *str, char** pszError );
unsigned long CAsyncHttp::CAsyncHttpResponse::getObjectValue()
{
    if (m_NetResponse.isOK())
    {
        if ( m_strContentType.find("application/json") != String::npos )
        {
            char* szError = 0;
            unsigned long valBody = rjson_tokener_parse(m_NetResponse.getCharData(), &szError);
            if ( valBody != 0 )
                return valBody;

            LOG(ERROR) + "Incorrect json body.Error:" + (szError ? szError : "");
            if ( szError )
                free(szError);
        }
    }

    return rho_ruby_create_string(m_NetResponse.getCharData());
}

} // namespace net
} // namespace rho

extern "C" {

using namespace rho::net;

void rho_asynchttp_cancel(const char* cancel_callback)
{
    if ( CAsyncHttp::getInstance() != null )
        CAsyncHttp::getInstance()->cancelRequest(cancel_callback);
}

unsigned long rho_asynchttp_request(const char* command, rho_param *p)
{
    CAsyncHttp::Create();

    return CAsyncHttp::getInstance()->addHttpCommand(new CAsyncHttp::CHttpCommand( command, p ));
}

}
