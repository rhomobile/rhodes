#include "generated/cpp/NetworkAccessBase.h"
#include "net/INetRequest.h"
#include "common/RhoAppAdapter.h"
#include "json/JSONIterator.h"
#include "common/RhodesApp.h"
#include "System.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NetworkAcceess"

namespace rho {

using namespace apiGenerator;
using namespace common;
using namespace json;

class CNetworkAccessImpl: public CNetworkAccessSingletonBase
{
public:

    class CHttpCommand : public CGeneratorQueue::CGeneratorQueueCommand
    {
    public:

        NetRequest m_NetRequest;

        CHttpCommand(rho::common::CInstanceClassFunctorBase<CMethodResult>* pFunctor) : CGeneratorQueue::CGeneratorQueueCommand(pFunctor){}
        virtual void cancel()
        {
            m_NetRequest.cancel(); 
        }
    };

    CNetworkAccessImpl(): CNetworkAccessSingletonBase(){}

    virtual rho::common::CThreadQueue::IQueueCommand* createQueueCommand(rho::common::CInstanceClassFunctorBase<CMethodResult>* pFunctor){ return new CHttpCommand(pFunctor); }

    virtual void cancel( rho::apiGenerator::CMethodResult& oResult);
    virtual void downloadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void get( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void post( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void uploadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
private:

    NetRequest& getCurRequest(NetRequest& oNetRequest);

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

void CNetworkAccessImpl::cancel( rho::apiGenerator::CMethodResult& oResult)
{
    if (!getCommandQueue())
        return;

    synchronized(getCommandQueue()->getCommandLock());
    CHttpCommand* pCmd = (CHttpCommand*)getCommandQueue()->getCurCommand();

    if ( pCmd != null && ( !oResult.hasCallback() || pCmd->isEqualCallback(oResult) ) )
        pCmd->cancel();

    if ( !oResult.hasCallback() )
        getCommandQueue()->getCommands().clear();
    else
    {
        for (int i = getCommandQueue()->getCommands().size()-1; i >= 0; i--)
        {
            CHttpCommand* pCmd1 = (CHttpCommand*)getCommandQueue()->getCommands().get(i);

            if ( pCmd1 != null && pCmd1->isEqualCallback(oResult) )
                getCommandQueue()->getCommands().remove(i);
        }

    }
}

void CNetworkAccessImpl::get( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<String,String> mapHeaders;                                          
    readHeaders( propertyMap, mapHeaders );

    NetRequest oNetRequest;

    if ( propertyMap.containsKey("verifyPeerCertificate") )
        getCurRequest(oNetRequest).setSslVerifyPeer( propertyMap.get("verifyPeerCertificate") == "true" );

    NetResponse resp = getNetRequest(&getCurRequest(oNetRequest)).doRequest( getStringProp(propertyMap, "httpVerb", "GET").c_str(),
            propertyMap.get("url"), propertyMap.get("body"), null, &mapHeaders);

    if ( !getCurRequest(oNetRequest).isCancelled())
        createResult( resp, mapHeaders, oResult );
}

void CNetworkAccessImpl::downloadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<String,String> mapHeaders;                                          
    readHeaders( propertyMap, mapHeaders );

    NetRequest oNetRequest;

    if ( propertyMap.containsKey("verifyPeerCertificate") )
        getCurRequest(oNetRequest).setSslVerifyPeer( propertyMap.get("verifyPeerCertificate") == "true" );

    NetResponse resp = getNetRequest(&getCurRequest(oNetRequest)).pullFile( propertyMap.get("url"), propertyMap.get("filename"), null, &mapHeaders);

    if ( !getCurRequest(oNetRequest).isCancelled())
        createResult( resp, mapHeaders, oResult );
}

void CNetworkAccessImpl::post( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<String,String> mapHeaders;                                          
    readHeaders( propertyMap, mapHeaders );

    NetRequest oNetRequest;

    if ( propertyMap.containsKey("verifyPeerCertificate") )
        getCurRequest(oNetRequest).setSslVerifyPeer( propertyMap.get("verifyPeerCertificate") == "true" );

    NetResponse resp = getNetRequest(&getCurRequest(oNetRequest)).doRequest( getStringProp(propertyMap, "httpVerb", "POST").c_str(),
            propertyMap.get("url"), propertyMap.get("body"), null, &mapHeaders);

    if ( !getCurRequest(oNetRequest).isCancelled())
        createResult( resp, mapHeaders, oResult );
}

void CNetworkAccessImpl::uploadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<String,String> mapHeaders;                                          
    readHeaders( propertyMap, mapHeaders );

    NetRequest oNetRequest;

    if ( propertyMap.containsKey("verifyPeerCertificate") )
        getCurRequest(oNetRequest).setSslVerifyPeer( propertyMap.get("verifyPeerCertificate") == "true" );

    VectorPtr<net::CMultipartItem*> arMultipartItems;
    if ( propertyMap.containsKey("multipart") )
    {
        for( CJSONArrayIterator oArray( propertyMap.get("multipart").c_str() ); !oArray.isEnd(); oArray.next() )
        {
            CJSONEntry oItem(oArray.getCurItem().getString());

            net::CMultipartItem* pItem = new net::CMultipartItem();
            String strFilePath = oItem.getString("filename", "");
            if ( strFilePath.length() == 0 )
            {
                pItem->m_strBody = oItem.getString("body", "");
                pItem->m_strContentType = oItem.getString("contentType", "");
            }
            else
            {
                pItem->m_strFilePath = strFilePath;
                pItem->m_strContentType = oItem.getString("contentType", "application/octet-stream");
            }

            pItem->m_strName = oItem.getString("name", "");
            pItem->m_strFileName = oItem.getString("filenameBase", "");
            arMultipartItems.addElement(pItem);
        }

    }else
    {
        net::CMultipartItem* pItem = new net::CMultipartItem();
        pItem->m_strFilePath = propertyMap.get("filename");
        pItem->m_strContentType = getStringProp(propertyMap, "fileContentType", "application/octet-stream");
        pItem->m_strName = propertyMap.get("name");
        pItem->m_strFileName = propertyMap.get("filenameBase");
        arMultipartItems.addElement(pItem);

        String strBody = propertyMap.get("body");
        if ( strBody.length() > 0 )
        {
            net::CMultipartItem* pItem2 = new net::CMultipartItem();
            pItem2->m_strBody = strBody;
            pItem2->m_strContentType = mapHeaders.get("content-type");
            arMultipartItems.addElement(pItem2);
        }
    }

    NetResponse resp = getNetRequest(&getCurRequest(oNetRequest)).pushMultipartData( propertyMap.get("url"), arMultipartItems, null, &mapHeaders );

    if ( !getCurRequest(oNetRequest).isCancelled())
        createResult( resp, mapHeaders, oResult );
}

void CNetworkAccessImpl::readHeaders( const rho::Hashtable<rho::String, rho::String>& propertyMap, Hashtable<String,String>& mapHeaders )
{
    if ( propertyMap.get("headers").length() > 0 )
    {
        for( CJSONStructIterator oHashIter( propertyMap.get("headers").c_str() ); !oHashIter.isEnd(); oHashIter.next() )
        {
            mapHeaders[oHashIter.getCurKey()] = oHashIter.getCurString();
        }
    }

    if ( propertyMap.get("authType") == AUTH_BASIC )
    {
        int nLen = rho_base64_encode(propertyMap.get("authPassword").c_str(), -1, 0);
        char* szBuf = new char[nLen+1];
        rho_base64_encode(propertyMap.get("authPassword").c_str(), -1, szBuf );

        char* szBuf2 = new char[nLen+1];
        rho_base64_decode(szBuf, -1, szBuf2 );

        mapHeaders["Authorization"] = "Basic " + propertyMap.get("authUser") + ":" + szBuf;
        delete szBuf;
    }

    if ( mapHeaders.get("User-Agent").length() == 0 )
    {
        mapHeaders["User-Agent"] = "Mozilla-5.0 (" + rho::System::getPlatform() + "; " + rho::System::getDeviceName() + "; " + rho::System::getOsVersion() + ")";
    }

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

    INetworkAccessSingleton* createModuleSingleton()
    { 
        return new CNetworkAccessImpl(); 
    }
};

}

extern "C" void Init_NetworkAccess()                                                                                      
{
    rho::CNetworkAccessFactory::setInstance( new rho::CNetworkAccessFactory() );
    rho::Init_NetworkAccess_API();

    RHODESAPP().getExtManager().requireRubyFile("NetworkAccess");
}
