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
Vector<CAsyncHttp*> CAsyncHttp::m_arInstances;

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

    {
	    synchronized(m_mxInstances);
        m_arInstances.addElement(this);
    }

    if (m_bNoThreaded)
        run();
    else
        start(epLow);
}

CAsyncHttp::~CAsyncHttp()
{
	synchronized(m_mxInstances)
    {		
        m_arInstances.removeElement(this);
    }
}

void CAsyncHttp::cancel()
{
    if (m_pNetRequest!=null)
        m_pNetRequest->cancel();

    stop(1000);
    delete this;
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

    m_pNetRequest = m_ptrFactory->createNetRequest();
    
    m_pNetResponse = m_pNetRequest->doRequest((m_bPost?"POST":"GET"), m_strUrl, m_strBody, null, &m_mapHeaders);
    INetResponse& resp = *m_pNetResponse;

    if ( !m_pNetRequest->isCancelled() && m_strCallback.length() > 0)
        callNotify(resp);

	LOG(INFO) + "RhoHttp thread end.";

    if ( !m_pNetRequest->isCancelled() )
        delete this;
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

void CAsyncHttp::processResponse(rho::net::INetResponse& resp )
{
    if (resp.isOK())
    {
        //TODO: parse JSON or xml
    }

    m_valBody = rho_ruby_create_string(resp.getCharData());
}

void CAsyncHttp::callNotify(rho::net::INetResponse& resp )
{
    processResponse(resp);

    String strBody = "rho_callback=1";
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

    strBody += "&" + RHODESAPP().addCallbackObject(m_valBody, "body");

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
        common::CAutoPtr<net::INetRequest> pNetRequest = m_ptrFactory->createNetRequest();

        String strFullUrl = pNetRequest->resolveUrl(m_strCallback);
        NetResponse(resp1,pNetRequest->pushData( strFullUrl, strBody, null ));
        if ( !resp1.isOK() )
            LOG(ERROR) + "AsyncHttp notification failed. Code: " + resp1.getRespCode() + "; Error body: " + resp1.getCharData();
    }

    RHODESAPP().delCallbackObject(m_valBody);
}

} // namespace net
} // namespace rho

extern "C" {

using namespace rho::net;

void rho_asynchttp_get(const char* url, unsigned long headers, const char* callback, const char* callback_params)
{
    new CAsyncHttp(rho::common::createClassFactory(), url, headers, null, callback, callback_params );
}

void rho_asynchttp_post(const char* url, unsigned long headers, const char* body, const char* callback, const char* callback_params)
{
    new CAsyncHttp(rho::common::createClassFactory(), url, headers, body!=null?body:"", callback, callback_params );
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
