#include "common/RhoPort.h"
#include "INetworkDetect.h"
#include <list>

#if (defined OS_WINCE || defined OS_WP8 || defined(OS_UWP))
#if (defined OS_WINCE)// && !defined(OS_PLATFORM_MOTCE)
#include "../platform/wm/src/ConnectionManager.h"
#endif
#endif

#if (defined OS_ANDROID)
#include "../platform/android/jni/NetworkAvailability.h"
#endif

#include "generated/cpp/NetworkBase.h"
#include "net/INetRequest.h"
#include "common/RhoAppAdapter.h"
#include "json/JSONIterator.h"
#include "common/RhodesApp.h"
#include "System.h"
#include "common/RhoConf.h"
#include "unzip/zip.h"
#ifdef OS_WINDOWS_DESKTOP
#include "zlib.h"
#endif

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NetworkAcceess"

namespace rho {

using namespace apiGenerator;
using namespace common;
using namespace json;

class CNetworkImpl: public CNetworkSingletonBase
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

    CNetworkImpl(): CNetworkSingletonBase()
	{
    m_networkPoller = 0;
#if (defined OS_WINCE)// && !defined(OS_PLATFORM_MOTCE)
		if(winversion == 1)
		{
			CRhoExtData rhodesData = RHODESAPP().getExtManager().makeExtData();
			if (rhodesData.m_hBrowserWnd)
			{
	//			m_pConnectionManager = new CWAN(rhodesData.m_hBrowserWnd);
				m_pConnectionManager = new CWAN(rhodesData.m_hWnd);
				m_pConnectionManager->Initialise();
			}
			else
				m_pConnectionManager = NULL;
		}
#endif
	}

	~CNetworkImpl()
	{
#if (defined OS_WINCE)// && !defined(OS_PLATFORM_MOTCE)
		if(winversion == 1)
		{
			if (m_pConnectionManager)
				delete m_pConnectionManager;
		}
#endif
	}

    virtual rho::common::CThreadQueue::IQueueCommand* createQueueCommand(rho::common::CInstanceClassFunctorBase<CMethodResult>* pFunctor){ return new CHttpCommand(pFunctor); }
    
    virtual void getResponseTimeout(rho::apiGenerator::CMethodResult& oResult);
    virtual void setResponseTimeout( int responseTimeout, rho::apiGenerator::CMethodResult& oResult);

    virtual void cancel( rho::apiGenerator::CMethodResult& oResult);
    virtual void downloadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void get( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void post( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void uploadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);

    virtual void hasNetwork(rho::apiGenerator::CMethodResult& oResult);
    virtual void hasWifiNetwork(rho::apiGenerator::CMethodResult& oResult);
    virtual void hasCellNetwork(rho::apiGenerator::CMethodResult& oResult);
    virtual void startStatusNotify( int pollInterval, rho::apiGenerator::CMethodResult& oResult);
    virtual void stopStatusNotify(rho::apiGenerator::CMethodResult& oResult);
    virtual void detectConnection( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
	virtual void stopDetectingConnection(rho::apiGenerator::CMethodResult& oResult);
    virtual void connectWan( const rho::String& connectionDestination, rho::apiGenerator::CMethodResult& oResult);
    virtual void disconnectWan(rho::apiGenerator::CMethodResult& oResult);
    virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<CMethodResult>* pFunctor);
    common::CMutex addingToQueueMutex;
#if (defined OS_WINCE)// && !defined(OS_PLATFORM_MOTCE)
    virtual bool onWndMsg(MSG& oMsg)
	{
		if(winversion == 1)
		{
			if (oMsg.message == WM_USER_CONNECTION_MANGER_STATUS)
			{
				if (!m_pConnectionManager) {return FALSE;}
					m_pConnectionManager->ConnectionManagerStatusUpdate();
			}
		}
		return FALSE;
	}
#endif
private:

    NetRequest& getCurRequest(NetRequest& oNetRequest, const rho::apiGenerator::CMethodResult& oResult) const;
    rho::String getEncodedBody(const rho::String& body, const rho::String& encoding);

    void readHeaders( const rho::Hashtable<rho::String, rho::String>& propertyMap, Hashtable<String,String>& mapHeaders ) const;
    void createResult( const NetResponse& resp, const Hashtable<String,String>& mapHeaders, rho::apiGenerator::CMethodResult& oResult ) const;
	//  RE1 Network API
//	std::list<INetworkDetection*> m_networkPollers;
    INetworkDetection* m_networkPoller;
#if (defined OS_WINCE)// && !defined(OS_PLATFORM_MOTCE)
	CWAN *m_pConnectionManager;
#endif
    void setupSecureConnection( 
        const rho::Hashtable<rho::String, rho::String>& propertyMap, 
        NetRequest& oNetRequest, 
        const rho::apiGenerator::CMethodResult& oResult ) const;

    rho::net::CNetRequestWrapper prepareRequest ( 
        const rho::Hashtable<rho::String, rho::String>& propertyMap, 
        NetRequest& oNetRequest, 
        const rho::apiGenerator::CMethodResult& oResult,
        /*out*/ Hashtable<String,String>& mapHeaders ) const
    {
        readHeaders( propertyMap, mapHeaders );

        setupSecureConnection( propertyMap, oNetRequest, oResult );

        rho::net::CNetRequestWrapper reqWrapper = getNetRequest(&getCurRequest(oNetRequest, oResult));

        if ( propertyMap.get("authType") == AUTH_BASIC ) {
            LOG(TRACE)+"Setting HTTP auth type to BASIC";

            reqWrapper.setAuthMethod( rho::net::AUTH_BASIC );
            reqWrapper.setAuthUser( propertyMap.get("authUser") );
            reqWrapper.setAuthPassword( propertyMap.get("authPassword") );
        } else if ( propertyMap.get("authType") == AUTH_DIGEST ) {
            LOG(TRACE)+"Setting HTTP auth type to DIGEST";

            reqWrapper.setAuthMethod( rho::net::AUTH_DIGEST );
            reqWrapper.setAuthUser( propertyMap.get("authUser") );
            reqWrapper.setAuthPassword( propertyMap.get("authPassword") );
        } else {
            LOG(TRACE)+"Setting HTTP auth type to NONE";
            reqWrapper.setAuthMethod( rho::net::AUTH_NONE );
        }

        return reqWrapper;
    }

    void prepareResult( 
        const NetResponse& resp, 
        NetRequest& oNetRequest,
        const Hashtable<String,String>& mapHeaders,
        rho::apiGenerator::CMethodResult& oResult ) const
    {
        if ( !getCurRequest(oNetRequest, oResult).isCancelled())
            createResult( resp, mapHeaders, oResult );
    }
};

NetRequest& CNetworkImpl::getCurRequest(NetRequest& oNetRequest, const rho::apiGenerator::CMethodResult& oResult) const
{
    if (!getCommandQueue() || !oResult.hasCallback() )
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
    
void CNetworkImpl::getResponseTimeout(rho::apiGenerator::CMethodResult& oResult) {
    int timeout = RHOCONF().getInt("net_timeout");
    if (0==timeout) timeout=30;
    oResult.set(timeout);
} 
    
void CNetworkImpl::setResponseTimeout( int responseTimeout, rho::apiGenerator::CMethodResult& oResult) {
    RHOCONF().setInt("net_timeout",responseTimeout,false);
}


void CNetworkImpl::cancel( rho::apiGenerator::CMethodResult& oResult)
{
    if (!getCommandQueue())
        return;

    synchronized(getCommandQueue()->getCommandLock());
    CHttpCommand* pCmd = (CHttpCommand*)getCommandQueue()->getCurCommand();

    if ( pCmd != NULL && ( !oResult.hasCallback() || pCmd->isEqualCallback(oResult) ) )
        pCmd->cancel();

    if ( !oResult.hasCallback() )
        getCommandQueue()->getCommands().clear();
    else
    {
        for (int i = getCommandQueue()->getCommands().size()-1; i >= 0; i--)
        {
            CHttpCommand* pCmd1 = (CHttpCommand*)getCommandQueue()->getCommands().get(i);

            if ( pCmd1 != NULL && pCmd1->isEqualCallback(oResult) )
                getCommandQueue()->getCommands().remove(i);
        }

    }
}

void CNetworkImpl::get( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<String,String> mapHeaders;                                          
    NetRequest oNetRequest;
    rho::net::CNetRequestWrapper reqWrapper = prepareRequest( propertyMap, oNetRequest, oResult, mapHeaders );
    
    String body = propertyMap.get("body");
    if ( propertyMap.containsKey("contentEncoding") ) {
        body = getEncodedBody(body,propertyMap.get("contentEncoding"));
    }
    
    NetResponse resp = reqWrapper.doRequest( getStringProp(propertyMap, "httpVerb", "GET").c_str(),
            propertyMap.get("url"), body, NULL, &mapHeaders);

    prepareResult( resp, oNetRequest, mapHeaders, oResult );
}

void CNetworkImpl::downloadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<String,String> mapHeaders;                                          
    NetRequest oNetRequest;
    rho::net::CNetRequestWrapper reqWrapper = prepareRequest( propertyMap, oNetRequest, oResult, mapHeaders );
    
    bool overwriteFile = propertyMap.containsKey("overwriteFile") && (propertyMap.get("overwriteFile")=="true");
    bool createFolders = propertyMap.containsKey("createFolders") && (propertyMap.get("createFolders")=="true");
    bool wantMoreCallbacks = propertyMap.containsKey( "wantReceiveProgressCallbacks" ) && (propertyMap.get( "wantReceiveProgressCallbacks" )=="true");
    bool fileExists = false;        

    //inplace callback for download progress notifications    
    struct NetCallback : public net::INetRequestCallback
    {
        rho::apiGenerator::CMethodResult& m_result;

        NetCallback();
        NetCallback( rho::apiGenerator::CMethodResult& r ) : m_result(r) {}

        virtual void didReceiveResponse(NetResponse& resp, const Hashtable<String,String>* headers)
        {
            Hashtable<String,String>& mapRes = m_result.getStringHash();
            mapRes["status"] = "progress_headers";
            mapRes["http_error"] = convertToStringA(resp.getRespCode());
            if ( headers != 0 ) {
                m_result.getStringHashL2()["headers"] = *headers;
            }

            m_result.set(mapRes);
        }

        virtual void didReceiveData(const char* data, int len)
        {
            (m_result.getStringHashL2()["headers"]).clear();

            Hashtable<String,String>& mapRes = m_result.getStringHash();
            mapRes["status"] = "progress_data";
            mapRes["length"] = convertToStringA( len );
            //mapRes["data"] = String( data, len );

            m_result.set(mapRes);
        }

        virtual void didFinishLoading() {}
        virtual void didFail(NetResponse&) {}
    };

    NetCallback cb(oResult);

    if ( wantMoreCallbacks )
    {        
        reqWrapper.setCallback( &cb );
    }

    NetResponse resp = reqWrapper.pullFile( propertyMap.get("url"), propertyMap.get("filename"), NULL, &mapHeaders,overwriteFile,createFolders,&fileExists);

    if ( !getCurRequest(oNetRequest, oResult).isCancelled()) {
        if (fileExists) {
            Hashtable<String,String>& mapRes = oResult.getStringHash();
            mapRes["status"] = "error";
            mapRes["fileExists"] = "true";
            oResult.set(mapRes);

        } else {
            createResult( resp, mapHeaders, oResult );
        }
    }
}

void CNetworkImpl::addCommandToQueue(
        rho::common::CInstanceClassFunctorBase<CMethodResult>* pFunctor){
        synchronized(addingToQueueMutex);
        CModuleSingletonBase<INetworkSingleton>::addCommandToQueue(pFunctor);
}



void CNetworkImpl::post( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<String,String> mapHeaders;                                          
    NetRequest oNetRequest;
    rho::net::CNetRequestWrapper reqWrapper = prepareRequest( propertyMap, oNetRequest, oResult, mapHeaders );

    String body = propertyMap.get("body");
    if ( propertyMap.containsKey("contentEncoding") ) {
        body = getEncodedBody(body,propertyMap.get("contentEncoding"));
    }

    NetResponse resp = reqWrapper.doRequest( getStringProp(propertyMap, "httpVerb", "POST").c_str(),
        propertyMap.get("url"), body, NULL, &mapHeaders);

    prepareResult( resp, oNetRequest, mapHeaders, oResult );
}

void CNetworkImpl::uploadFile( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<String,String> mapHeaders;                                          
    NetRequest oNetRequest;
    rho::net::CNetRequestWrapper reqWrapper = prepareRequest( propertyMap, oNetRequest, oResult, mapHeaders );

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

    NetResponse resp = reqWrapper.pushMultipartData( propertyMap.get("url"), arMultipartItems, NULL, &mapHeaders );

    prepareResult( resp, oNetRequest, mapHeaders, oResult );    
}

void CNetworkImpl::readHeaders( const rho::Hashtable<rho::String, rho::String>& propertyMap, Hashtable<String,String>& mapHeaders ) const
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
        String creds = propertyMap.get("authUser") + ":" + propertyMap.get("authPassword");
        
        int nLen = rho_base64_encode(creds.c_str(), -1, 0);
        char* szBuf = new char[nLen+1];
        rho_base64_encode(creds.c_str(), -1, szBuf );
        mapHeaders["Authorization"] = String("Basic ") + szBuf;
        delete szBuf;
    }

    if ( mapHeaders.get("User-Agent").length() == 0 )
    {
        mapHeaders["User-Agent"] = "Mozilla-5.0 (" + rho::System::getPlatform() + "; " + rho::System::getDeviceName() + "; " + rho::System::getOsVersion() + ")";
    }
    
    if ( propertyMap.containsKey("contentEncoding") )
    {
        mapHeaders["Content-Encoding"] = propertyMap.get("contentEncoding");
    }
}
    
rho::String CNetworkImpl::getEncodedBody(const rho::String& body, const rho::String& encoding)
{
    String ret = body;
    
    if ( encoding == "gzip" ) {
        String encoded;
        
        if (ZR_OK==::GZipBuffer(body,encoded)) {
            ret = encoded;
        }
    }
    
    return ret;    
}


void CNetworkImpl::createResult( const NetResponse& resp, const Hashtable<String,String>& mapHeaders, rho::apiGenerator::CMethodResult& oResult ) const
{
/*
    String strJSON = "{";
    if ( resp.isSuccess() )
    {
        strJSON += "\"status\":" + CJSONEntry::quoteValue("ok");
        if ( resp.isResponseRecieved())
            strJSON += ",\"http_error\":" + CJSONEntry::quoteValue( convertToStringA(resp.getRespCode()) );
    }
    else
    {
        strJSON += "\"status\":" + CJSONEntry::quoteValue("error");
        strJSON += ",\"error_code\":" + CJSONEntry::quoteValue(convertToStringA(RhoAppAdapter.getErrorFromResponse(resp)));
        if ( resp.isResponseRecieved())
            strJSON += ",\"http_error\":" + CJSONEntry::quoteValue( convertToStringA(resp.getRespCode()) );
    }

    String cookies = resp.getCookies();
    if ( cookies.length() > 0 )
        strJSON += ",\"cookies\":" + CJSONEntry::quoteValue(cookies);

    strJSON += ",\"headers\":" + CJSONEntry::toJSON(mapHeaders);
    strJSON += ",\"body\":" + CJSONEntry::quoteValue(String(resp.getCharData(), resp.getDataSize()));

    strJSON += "}";

    oResult.setJSON(strJSON); */

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
        mapRes["error_code"] = convertToStringA(RhoAppAdapter.getErrorFromResponse(resp));
        if ( resp.isResponseRecieved())
            mapRes["http_error"] = convertToStringA(resp.getRespCode());
        mapRes["error_message"] = resp.getErrorMessage();
    }

    String cookies = resp.getCookies();
    if (cookies.length()>0)
        mapRes["cookies"] = cookies;

    oResult.getStringHashL2()["headers"] = mapHeaders;

    //TODO: support "application/json" content-type
#ifdef OS_WINDOWS_DESKTOP
    if (mapHeaders.containsKey("content-encoding") &&
        ((mapHeaders.get("content-encoding") == "gzip") ||
         (mapHeaders.get("content-encoding") == "x-gzip")))
    {
        size_t bufSize = 30*((size_t)resp.getDataSize());
        if (bufSize < 4096)
            bufSize = 4096;
        Bytef* decoded = (Bytef*)malloc(bufSize+1);
        *decoded = '\0';
        z_stream strm;
        strm.next_in = (Bytef*)resp.getCharData();
        strm.avail_in = resp.getDataSize();
        strm.next_out = decoded;
        strm.avail_out = bufSize;
        strm.opaque = Z_NULL;
        strm.zalloc = (alloc_func)0;
        strm.zfree = (free_func)0;
        int res = inflateInit2(&strm, 16 + MAX_WBITS);
        if (res == Z_OK) {
            do {
                res = inflate(&strm, Z_SYNC_FLUSH);
                decoded[strm.total_out] = '\0';
                if ((res == Z_OK) || (res == Z_STREAM_END)) {
                    if (strm.avail_in == 0)
                        mapRes["body"].assign( (char*)decoded, strm.total_out );
                    else {
                        bufSize *= 4;
                        decoded = (Bytef*)realloc(decoded, bufSize+1);
                        strm.next_out = decoded + strm.total_out;
                        strm.avail_out = bufSize - strm.total_out;
                    }
                }
            } while ((res == Z_OK) && (strm.avail_in > 0));
        }
        if ((res != Z_OK) && (res != Z_STREAM_END)) {
            mapRes["status"] = "error";
            mapRes["error_code"] = "gzip: ";
            mapRes.get("error_code").append(strm.msg);
            LOG(INFO) + "gzip error: " + strm.msg;
        }
        inflateEnd(&strm);
        free(decoded);
    } else
#endif
    mapRes["body"].assign( resp.getCharData(), resp.getDataSize() );

    oResult.set(mapRes);
}

static int g_rho_has_network = 1, g_rho_has_cellnetwork = 0;

extern "C" void rho_sysimpl_sethas_network(int nValue)
{
    g_rho_has_network = nValue >= 1 ? 1 : 0;
}

extern "C" int rho_sysimpl_has_network()
{
    return g_rho_has_network;
}

extern "C" void rho_sysimpl_sethas_cellnetwork(int nValue)
{
    g_rho_has_cellnetwork = nValue;
}

void CNetworkImpl::hasNetwork(rho::apiGenerator::CMethodResult& oResult)
{
    RAWTRACE("NetworkC hasNetwork+");
#if (defined OS_ANDROID)
	int iResult = CNetworkAvailability::hasNetwork();
	if(iResult == -2)
	{
		oResult.setError("Internal Error: Could not connect to Android");
	}
	else if(iResult == -1)
	{
		oResult.setError("Could not detect for a network");
	}
	else
	{
		oResult.set(iResult != 0);
	}
	RAWTRACE("NetworkC hasNetwork-");
	return;
#endif
#if (!defined OS_ANDROID)
    oResult.set(g_rho_has_network!= 0 || g_rho_has_cellnetwork!= 0);
#endif
}

void CNetworkImpl::hasWifiNetwork(rho::apiGenerator::CMethodResult& oResult)
{
    RAWTRACE("NetworkC hasWifiNetwork+");
#if (defined OS_ANDROID)
	int iResult = CNetworkAvailability::hasWifiNetwork();
	if(iResult == -2)
	{
		oResult.setError("Internal Error: Could not connect to Android");
	}
	else if(iResult == -1)
	{
		oResult.setError("Could not detect for a wifi network");
	}
	else
	{
		oResult.set(iResult != 0);
	}
	RAWTRACE("NetworkC hasWifiNetwork-");
	return;
#endif
#if (!defined OS_ANDROID)
    oResult.set(g_rho_has_network!= 0);
#endif
}

void CNetworkImpl::hasCellNetwork(rho::apiGenerator::CMethodResult& oResult)
{
    RAWTRACE("NetworkC hasCellNetwork+");
#if (defined OS_ANDROID)
	int iResult = CNetworkAvailability::hasCellNetwork();
	if(iResult == -2)
	{
		oResult.setError("Internal Error: Could not connect to Android");
	}
	else if(iResult == -1)
	{
		oResult.setError("Could not detect for a cell network");
	}
	else
	{
		oResult.set(iResult != 0);
	}
	RAWTRACE("NetworkC hasCellNetwork-");
	return;
#endif
#if (!defined OS_ANDROID)
    oResult.set(g_rho_has_cellnetwork!= 0);
#endif
}

void CNetworkImpl::startStatusNotify( int pollInterval, rho::apiGenerator::CMethodResult& oResult)
{
    RHODESAPP().setNetworkStatusNotify(oResult,pollInterval);
}

void CNetworkImpl::stopStatusNotify(rho::apiGenerator::CMethodResult& oResult)
{
    RHODESAPP().clearNetworkStatusNotify();
}


//  RE1 Network API Implementation
void CNetworkImpl::detectConnection( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    stopDetectingConnection(oResult);

    if (!oResult.hasCallback()) {
        return;
    }
    
	INetworkDetection* pNetworkDetection = NetworkDetectionFactory::createNetworkDetection();
    if ( 0 == pNetworkDetection ) {
        LOG(ERROR) + "Unable to create network detection object";
        return;
    }
    
	pNetworkDetection->Initialise();
//	m_networkPollers.push_back(pNetworkDetection);
  if ( m_networkPoller != 0 )
  {
    m_networkPoller->CleanupAndDeleteSelf();
    m_networkPoller = 0;
  }

    m_networkPoller = pNetworkDetection;
	if (!pNetworkDetection->IsChecking())
	{
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			if (iterator->first.compare("host") == 0)
				pNetworkDetection->SetHost(iterator->second);
			else if (iterator->first.compare("port") == 0)
				pNetworkDetection->SetPort(atoi(iterator->second.c_str()));
			else if (iterator->first.compare("pollInterval") == 0)
				pNetworkDetection->SetNetworkPollInterval(atoi(iterator->second.c_str()));
			else if (iterator->first.compare("detectionTimeout") == 0)
				pNetworkDetection->SetConnectionTimeout(atoi(iterator->second.c_str()));
			else
				LOG(WARNING) + "Unrecognised parameter passed to detectConnection: " + iterator->first;
		}
		pNetworkDetection->SetCallback(oResult);
		pNetworkDetection->StartNetworkChecking();
	}
	else
		LOG(WARNING) + "Unable to start checking for a network connection, a check is already in progress.  First stop the in progress connection check";
}

void CNetworkImpl::stopDetectingConnection(rho::apiGenerator::CMethodResult& oResult)
{
	//  Find the network detector which matches our callback
    /*
	INetworkDetection *pNetworkDetection = NULL;
	std::list<INetworkDetection*>::iterator i;
	for (i = m_networkPollers.begin(); i != m_networkPollers.end(); ++i)
	{
		if ((*i) && (*i)->GetCallback().isEqualCallback(oResult))
		{
			//  We've found the CNetworkDetection object with the callback we're looking for
			pNetworkDetection = (*i);
			break;
		}
	}
	if (pNetworkDetection && pNetworkDetection->IsChecking())
	{
		pNetworkDetection->StopNetworkChecking();
		m_networkPollers.remove(pNetworkDetection);
        pNetworkDetection->Cleanup();
		delete pNetworkDetection;
	}
	else
		LOG(WARNING) + "Unable to stop detecting network connection, could not find specified callback";
    */
    if ( m_networkPoller != 0) {
        if ( m_networkPoller->IsChecking() ) 
        {
            m_networkPoller->StopNetworkChecking();
        }
        //Check again existence of m_networkPoller before calling CleanupAndDeleteSelf,as in some conditions it becomes NULL by other threads
        if(m_networkPoller!=0)
	{
	LOG(INFO)+"CleanupAndDeleteSelf";	
        m_networkPoller->CleanupAndDeleteSelf();
	m_networkPoller = 0;
		
	}
	else
	{
	LOG(INFO)+"NetworkPoler object does not exist.No nedd to cleanup";
	}
        
    }
}


void CNetworkImpl::connectWan( const rho::String& connectionDestination, rho::apiGenerator::CMethodResult& oResult)
{
#if (defined OS_WINCE)//&& !defined(OS_PLATFORM_MOTCE) 
	if(winversion == 1)
	{
		//  Only applicable to WM/CE, specific to connection manager
		//  There is only a single object for connection manager access as you can only have
		//  one physical connection.
		m_pConnectionManager->SetWanCallback(oResult);
		m_pConnectionManager->Connect(convertToStringW(connectionDestination).c_str(), TRUE);
	}
#endif
}

void CNetworkImpl::disconnectWan(rho::apiGenerator::CMethodResult& oResult)
{
#if (defined OS_WINCE)// && !defined(OS_PLATFORM_MOTCE)
	if(winversion == 1)
	{
		//  Only applicable to WM/CE, specific to connection manager
		m_pConnectionManager->Disconnect(TRUE);
	}
#endif
}
    
void CNetworkImpl::setupSecureConnection( 
    const rho::Hashtable<rho::String, rho::String>& propertyMap, 
    NetRequest& oNetRequest, 
    const rho::apiGenerator::CMethodResult& oResult ) const
{
    String clientCertificate = "";
    String clientCertificatePassword = "";

    //will enable server SSL auth if true.
    if ( propertyMap.containsKey("verifyPeerCertificate") ) {
        bool verifyPeer = (propertyMap.get("verifyPeerCertificate") == "true");
        getCurRequest(oNetRequest, oResult).setSslVerifyPeer( verifyPeer );
        
        if ( verifyPeer ) {
            //will enable client-side SSL auth if valid certificate path is provided (Android-only).
            if ( propertyMap.containsKey("clientSSLCertificate") ) {
                clientCertificate = propertyMap.get("clientSSLCertificate");
            }
            
            if ( propertyMap.containsKey("clientSSLCertificatePassword") ) {
                clientCertificatePassword = propertyMap.get("clientSSLCertificatePassword");
            }
        }
        
    }
    
    RHOCONF().setString("clientSSLCertificate",clientCertificate,false);
    RHOCONF().setString("clientSSLCertificatePassword",clientCertificatePassword,false);
/*
    NetRequest& currentRequest = getCurRequest(oNetRequest, oResult);

    if ( propertyMap.get("authType") == AUTH_BASIC ) {
        LOG(TRACE)+"Setting HTTP auth type to BASIC";

        currentRequest.setAuthMethod( rho::net::AUTH_BASIC );
        currentRequest.setAuthUser( propertyMap.get("authUser") );
        currentRequest.setAuthPassword( propertyMap.get("authPassword") );
    } else if ( propertyMap.get("authType") == AUTH_DIGEST ) {
        LOG(TRACE)+"Setting HTTP auth type to DIGEST";

        currentRequest.setAuthMethod( rho::net::AUTH_DIGEST );
        currentRequest.setAuthUser( propertyMap.get("authUser") );
        currentRequest.setAuthPassword( propertyMap.get("authPassword") );
    } else {
        LOG(TRACE)+"Setting HTTP auth type to NONE";

        currentRequest.setAuthMethod( rho::net::AUTH_NONE );
    }
*/
}
    

////////////////////////////////////////////////////////////////////////

class CNetworkFactory: public CNetworkFactoryBase
{
public:
    ~CNetworkFactory(){}

    INetworkSingleton* createModuleSingleton()
    { 
        return new CNetworkImpl(); 
    }
};

}

extern "C" void Init_Network()                                                                                      
{
    rho::CNetworkFactory::setInstance( new rho::CNetworkFactory() );
    rho::Init_Network_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoNetworkApi");
}
