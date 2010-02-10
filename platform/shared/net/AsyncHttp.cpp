#include "net/AsyncHttp.h"
#include "ruby/ext/rho/rhoruby.h"
#include "common/StringConverter.h"
#include "net/URI.h"

namespace rho
{
using namespace rho::common;
namespace net
{

IMPLEMENT_LOGCLASS(CAsyncHttp, "AsyncHttp");
boolean CAsyncHttp::m_bNoThreaded = false;
CAsyncHttp* CAsyncHttp::m_pInstance = 0;

extern "C" void header_iter(const char* szName, const char* szValue, void* pHash)
{
    ((Hashtable<String,String>*)pHash)->put(szName, szValue);
}

CAsyncHttp::CAsyncHttp(common::IRhoClassFactory* factory,
    const char* url, unsigned long headers, const char* body, const char* callback, const char* callback_params) :  CRhoThread(factory)
{
    m_ptrFactory = factory;
    m_strUrl = url != null ? url : "";
    m_bPost = body != null;
    m_strBody = body != null ? body : "";
    m_strCallback = callback != null ? callback : "";
    m_strCallbackParams = callback_params != null ? callback_params : "";

    rho_ruby_enum_strhash(headers, header_iter, &m_mapHeaders);

    m_pInstance = this;

    if (m_bNoThreaded)
        run();
    else
        start(epLow);
}

CAsyncHttp::~CAsyncHttp()
{
    m_pInstance = null;
}

void CAsyncHttp::cancel()
{
    if (m_pNetRequest!=null)
        m_pNetRequest->cancel();

    stop(1000);
    delete this;
}

void CAsyncHttp::run()
{
	LOG(INFO) + "RhoHttp thread start.";

    m_pNetRequest = m_ptrFactory->createNetRequest();
    
    m_pNetResponse = m_pNetRequest->doRequest((m_bPost?"POST":"GET"), m_strUrl, m_strBody, null, &m_mapHeaders);
    INetResponse& resp = *m_pNetResponse;

    if ( !m_pNetRequest->isCancelled() )
    {
        if (resp.isOK())
            processResponse(resp);

        callNotify(resp);
    }

	LOG(INFO) + "RhoHttp thread end.";

    if ( !m_pNetRequest->isCancelled() )
        delete this;
}

void CAsyncHttp::processResponse(rho::net::INetResponse& resp )
{
}

unsigned int CAsyncHttp::getBodyValue()
{
    if (!m_pNetResponse)   
        return rho_ruby_get_NIL();

    return rho_ruby_create_string(m_pNetResponse->getCharData());
}

String CAsyncHttp::makeHeadersString()
{
    String strRes;

    for ( Hashtable<String,String>::iterator it = m_mapHeaders.begin();  it != m_mapHeaders.end(); ++it )
    {
        if ( strRes.length() )
            strRes += "&";

        strRes += "headers[";
        URI::urlEncode(it->first,strRes);
        strRes += "]=";
        URI::urlEncode(it->second,strRes);
    }

    return strRes;
}

void CAsyncHttp::callNotify(rho::net::INetResponse& resp )
{
    String strBody = "rho_callback=2";
    strBody += "&status=";
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

    strBody += "&" + makeHeadersString();

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
        return;
    }

    common::CAutoPtr<net::INetRequest> pNetRequest = m_ptrFactory->createNetRequest();

    String strFullUrl = pNetRequest->resolveUrl(m_strCallback);
    NetResponse(resp1,pNetRequest->pushData( strFullUrl, strBody, null ));
    if ( !resp1.isOK() )
        LOG(ERROR) + "AsyncHttp notification failed. Code: " + resp1.getRespCode() + "; Error body: " + resp1.getCharData();

}

} // namespace net
} // namespace rho

extern "C" {

using namespace rho::net;

void rho_asynchttp_get(const char* url, unsigned long headers, const char* callback, const char* callback_params)
{
    rho_asynchttp_cancel();
    new CAsyncHttp(rho::common::createClassFactory(), url, headers, null, callback, callback_params );
}

void rho_asynchttp_post(const char* url, unsigned long headers, const char* body, const char* callback, const char* callback_params)
{
    rho_asynchttp_cancel();
    new CAsyncHttp(rho::common::createClassFactory(), url, headers, body!=null?body:"", callback, callback_params );
}

unsigned int rho_asynchttp_getbody()
{
    if (CAsyncHttp::getInstance()!= null)
        return CAsyncHttp::getInstance()->getBodyValue();

    return rho_ruby_get_NIL();
}

void rho_asynchttp_cancel()
{
    if (CAsyncHttp::getInstance()!= null)
        CAsyncHttp::getInstance()->cancel();
}

void rho_asynchttp_set_threaded_mode(int b)
{
    CAsyncHttp::m_bNoThreaded = (b==0);
}

}
