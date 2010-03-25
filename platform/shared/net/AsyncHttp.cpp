#include "net/AsyncHttp.h"
#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "net/URI.h"
#include "common/RhodesApp.h"

namespace rho
{
using namespace rho::common;
namespace net
{

IMPLEMENT_LOGCLASS(CAsyncHttp, "AsyncHttp");
boolean CAsyncHttp::m_bNoThreaded = false;
common::CMutex CAsyncHttp::m_mxInstances;
VectorPtr<CAsyncHttp*> CAsyncHttp::m_arInstances;

extern "C" void header_iter(const char* szName, const char* szValue, void* pHash)
{
    ((Hashtable<String,String>*)pHash)->put(szName, szValue);
}

CAsyncHttp::CAsyncHttp(common::IRhoClassFactory* factory, EHttpCommands eCmd,
    const char* url, unsigned long headers, const char* body, const char* callback, const char* callback_params) :  CRhoThread(factory)
{
    m_bFinished = false;
    m_ptrFactory = factory;
    m_strUrl = url != null ? url : "";
    m_strBody = body != null ? body : "";
    m_strCallback = callback != null ? callback : "";
    m_strCallbackParams = callback_params != null ? callback_params : "";
    m_eCmd = eCmd;

    rho_ruby_enum_strhash(headers, &header_iter, &m_mapHeaders);

    addNewObject(this);

    if (m_bNoThreaded)
        run();
    else
        start(epLow);
}

void CAsyncHttp::cancel()
{
    {		
    	synchronized(m_mxRequest)

        if (m_pNetRequest!=null && !m_pNetRequest->isCancelled() )
            m_pNetRequest->cancel();
    }

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

/*static*/ void CAsyncHttp::cancelRequest(const char* szCallback)
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
                m_arInstances.elementAt(i)->cancel();
        }else
        {
            for (int i = 0; i < (int)m_arInstances.size(); i++ )
            {
                if ( m_arInstances.elementAt(i)->m_strCallback.compare(szCallback) == 0 )
                    m_arInstances.elementAt(i)->cancel();    
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
        m_pNetResponse = m_pNetRequest->pullFile(m_strUrl, m_strBody, null, &m_mapHeaders);
        break;

    case hcUpload:
        m_pNetResponse = m_pNetRequest->pushFile(m_strUrl, m_strBody, null, &m_mapHeaders);
        break;
    }

    if ( !m_pNetRequest->isCancelled() && m_strCallback.length() > 0)
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

void CAsyncHttp::callNotify(rho::net::INetResponse& resp, int nError )
{
    String strBody = "rho_callback=1";
    strBody += "&status=";
    if ( nError > 0 )
    {
    	strBody += "error&error_code=" + nError;
    }else
    {
        if ( resp.isOK() )
    	    strBody += "ok";
        else
        {
    	    strBody += "error&error_code=";
            if ( !resp.isResponseRecieved())
                strBody += convertToStringA(1);//RhoRuby.ERR_NETWORK);
            else if ( resp.isUnathorized() )
                strBody += convertToStringA(9);//RhoRuby.ERR_UNATHORIZED);
            else
	            strBody += convertToStringA(2);//RhoRuby.ERR_REMOTESERVER);

            if ( resp.isResponseRecieved())
	            strBody += "&http_error=" + convertToStringA(resp.getRespCode());
        }

        String cookies = resp.getCookies();
        if (cookies.length()>0)
            strBody += "&cookies=" + URI::urlEncode(cookies);

        strBody += "&" + makeHeadersString();
        strBody += "&" + RHODESAPP().addCallbackObject(this, "body");
    }

    if ( m_strCallbackParams.length() > 0 )
        strBody += "&" + m_strCallbackParams;

    if ( m_bNoThreaded )
    {
        const char* szName = strrchr(m_strCallback.c_str(), '/');
        if (!szName)
            szName = m_strCallback.c_str();
        else
            szName++;

        String strName = "C_";
        strName += szName;
        rho_ruby_set_const( strName.c_str(), strBody.c_str());
    }else
    {
        {
            synchronized(m_mxRequest)
            m_pNetRequest = m_ptrFactory->createNetRequest();
        }

        String strFullUrl = m_pNetRequest->resolveUrl(m_strCallback);
        NetResponse(resp1,m_pNetRequest->pushData( strFullUrl, strBody, null ));
        if ( !resp1.isOK() )
            LOG(ERROR) + "AsyncHttp notification failed. Code: " + resp1.getRespCode() + "; Error body: " + resp1.getCharData();
    }

    RHODESAPP().delCallbackObject(this);
}

} // namespace net
} // namespace rho

extern "C" {

using namespace rho::net;

void rho_asynchttp_get(const char* url, unsigned long headers, const char* callback, const char* callback_params)
{
    new CAsyncHttp(rho::common::createClassFactory(), CAsyncHttp::hcGet, url, headers, null, callback, callback_params );
}

void rho_asynchttp_post(const char* url, unsigned long headers, const char* body, const char* callback, const char* callback_params)
{
    new CAsyncHttp(rho::common::createClassFactory(), CAsyncHttp::hcPost, url, headers, body!=null?body:"", callback, callback_params );
}

void rho_asynchttp_downloadfile(const char* url, unsigned long headers, const char* file_path, const char* callback, const char* callback_params)
{
    new CAsyncHttp(rho::common::createClassFactory(), CAsyncHttp::hcDownload, url, headers, file_path, callback, callback_params );
}

void rho_asynchttp_uploadfile(const char* url, unsigned long headers, const char* file_path, const char* callback, const char* callback_params)
{
    new CAsyncHttp(rho::common::createClassFactory(), CAsyncHttp::hcUpload, url, headers, file_path, callback, callback_params );
}

void rho_asynchttp_cancel(const char* cancel_callback)
{
    CAsyncHttp::cancelRequest(cancel_callback);
}

void rho_asynchttp_set_threaded_mode(int b)
{
    CAsyncHttp::m_bNoThreaded = (b==0);
}

}
