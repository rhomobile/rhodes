#include "generated/cpp/NetworkAccessBase.h"
#include "net/INetRequest.h"
#include "common/RhoAppAdapter.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NetworkAcceess"

namespace rho {

using namespace apiGenerator;
using namespace common;

class CNetworkAccessImpl: public CNetworkAccessSingletonBase
{
public:

    class CHttpCommand : public CGeneratorQueue::CGeneratorQueueCommand
    {
    public:

        NetRequest m_NetRequest;

        CHttpCommand(rho::common::IRhoRunnable* pFunctor) : CGeneratorQueue::CGeneratorQueueCommand(pFunctor){}
        virtual void cancel(){}
    };

    CNetworkAccessImpl(): CNetworkAccessSingletonBase(){}

    virtual rho::common::CThreadQueue::IQueueCommand* createQueueCommand(rho::common::IRhoRunnable* pFunctor){ return new CHttpCommand(pFunctor); }

    virtual void cancel( rho::apiGenerator::CMethodResult& oResult){}
    virtual void downloadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult){}
    virtual void get( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void post( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult){}
    virtual void uploadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult){}
private:

    NetRequest& getCurRequest(NetRequest& oNetRequest);
    net::CNetRequestWrapper getNet( NetRequest oNetRequest ){ return getNetRequest(&getCurRequest(oNetRequest)); }

    void readHeaders( const rho::Hashtable<rho::String, rho::String>& propertyMap, Hashtable<String,String>& mapHeaders );
    void createResult( NetResponse& resp, Hashtable<String,String>& mapHeaders, rho::apiGenerator::CMethodResult& oResult );
};

NetRequest& CNetworkAccessImpl::getCurRequest(NetRequest& oNetRequest)
{
    if (!getCommandQueue())
        return oNetRequest;

    CHttpCommand* pCmd = (CHttpCommand*)(getCommandQueue()->getCurCommand());
    if ( pCmd )
        return pCmd->m_NetRequest;

    return oNetRequest;
}

static String getStringProp(const rho::Hashtable<rho::String, rho::String>& propertyMap, const char* szKey, const char* szDefValue = "")
{
    String strRes;
    if (propertyMap.containsKey(szKey) )
        strRes = propertyMap.get(szKey);

    if (strRes.length() == 0 && szDefValue && *szDefValue)
        strRes = szDefValue;

    return strRes;
}

void CNetworkAccessImpl::get( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<String,String> mapHeaders;                                          
    readHeaders( propertyMap, mapHeaders );

    NetRequest oNetRequest;
    NetResponse resp = getNet(oNetRequest).doRequest( getStringProp(propertyMap, "http_command", "GET").c_str(),
            propertyMap.get("url"), propertyMap.get("body"), null, &mapHeaders);

    if ( !getCurRequest(oNetRequest).isCancelled())
        createResult( resp, mapHeaders, oResult );
}

void CNetworkAccessImpl::readHeaders( const rho::Hashtable<rho::String, rho::String>& propertyMap, Hashtable<String,String>& mapHeaders )
{
    //TODO: read Auth
    //TODO:: readHeaders 
}

void CNetworkAccessImpl::createResult( NetResponse& resp, Hashtable<String,String>& mapHeaders, rho::apiGenerator::CMethodResult& oResult )
{
    Hashtable<String,String>& mapRes = oResult.getStringHash();

    if ( resp.isSuccess() )
    {
        mapRes["status"] = "ok";
        if ( resp.isResponseRecieved())
            mapRes["http_error"] = convertToStringA(resp.getRespCode());
    }
    else
    {
        mapRes["status"] = "error";
        mapRes["error_code"] = convertToStringA(RhoAppAdapter.getErrorFromResponse(resp));;
        if ( resp.isResponseRecieved())
            mapRes["http_error"] = convertToStringA(resp.getRespCode());
    }

    String cookies = resp.getCookies();
    if (cookies.length()>0)
        mapRes["cookies"] = cookies;

    oResult.getStringHashL2()["headers"] = mapHeaders;

    //TODO: support "application/json" content-type
    mapRes["body"].assign( resp.getCharData(), resp.getDataSize() );

    oResult.set(mapRes);
}

////////////////////////////////////////////////////////////////////////

class CNetworkAccessFactory: public CNetworkAccessFactoryBase
{
public:
    ~CNetworkAccessFactory(){}

    INetworkAccessSingleton* createModuleSingleton(){ return new CNetworkAccessImpl(); }
};

extern "C" void Init_NetworkAccess()
{
    CNetworkAccessFactory::setInstance( new CNetworkAccessFactory() );
    Init_NetworkAccess_API();
}

}