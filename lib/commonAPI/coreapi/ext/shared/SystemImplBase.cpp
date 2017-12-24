#include "SystemImplBase.h"

#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "sync/RhoconnectClientManager.h"
#include "common/RhoFilePath.h"
#include "common/RhoFile.h"
#include "unzip/zip.h"
#include "db/DBAdapter.h"
#include "Network.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"

using namespace rho::common;
int rho_sys_zip_files_with_path_array_ptr(const char* szZipFilePath, const char *base_path, const rho::Vector<rho::String>& arFiles, const char* psw)
{
	ZRESULT res;
	HZIP hz = 0;
	
#if defined(UNICODE) && defined(WIN32) && !defined(OS_WP8)
	hz = CreateZip( convertToStringW(szZipFilePath).c_str(), psw);
#else
	hz = CreateZip((TCHAR*)szZipFilePath, psw);
#endif
	
	if ( !hz )
		return -1;
	
	for ( int i = 0; i < (int)arFiles.size(); i++ )
	{
		rho::String filePath = arFiles.elementAt(i);
		bool isDir = CRhoFile::isDirectory(filePath.c_str());
		rho::String zipPath = base_path ? filePath.substr(strlen(base_path)) : filePath;
        if ( rho::String_startsWith( zipPath, "/") || rho::String_startsWith( zipPath, "\\") )
            zipPath = zipPath.substr(1);

#if defined(UNICODE) && defined(WIN32) && !defined(OS_WP8)
		if ( isDir )
			res = ZipAddFolder( hz, convertToStringW(zipPath).c_str(), convertToStringW(filePath).c_str() );
		else
			res = ZipAdd( hz, convertToStringW(zipPath).c_str(), convertToStringW(filePath).c_str() );
#else
		if ( isDir )
			res = ZipAddFolder( hz, (TCHAR*)zipPath.c_str(), (TCHAR*)filePath.c_str() );
		else
			res = ZipAdd(hz, (TCHAR*)zipPath.c_str(), (TCHAR*)filePath.c_str() );
#endif
		
		if (res != 0)
			LOG(ERROR) + "Zip file failed: " + res + "; " + filePath;
	}
	
	res = CloseZip(hz);
	
	return res;
}

namespace rho {

using namespace apiGenerator;

IMPLEMENT_LOGCLASS(CSystemImplBase, "System");

void CSystemImplBase::getPlatform(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    rho::String strPlatform = RHOSIMCONF().getString("platform");

    if ( strPlatform.compare("bb") == 0 || strPlatform.compare("bb6") == 0 )
        oResult.set( L"Blackberry" );
    else if ( strPlatform.compare("wm") == 0 )
        oResult.set( L"WINDOWS" );
    else if ( strPlatform.compare("win32") == 0 )
        oResult.set( L"WINDOWS_DESKTOP" );
    else if ( strPlatform.compare("wp") == 0 )
        oResult.set( L"WP7" );
    else if ( strPlatform.compare("wp8") == 0 )
        oResult.set( L"WP8");
    else if ( strPlatform.compare("uwp") == 0 )
        oResult.set( L"UWP");
    else if ( strPlatform.compare("android") == 0 )
        oResult.set( L"ANDROID" );
    else if ( strPlatform.compare("iphone") == 0 )
        oResult.set( L"APPLE" );
    else
        oResult.set( strPlatform );

#else //!RHODES_EMULATOR

#if defined(OS_MACOSX)
	oResult.set( PLATFORM_IOS );
#elif defined(OS_WINDOWS_DESKTOP)
	oResult.set( PLATFORM_WINDOWS_DESKTOP );
#elif defined(WINDOWS_PLATFORM)
#if defined(OS_WP8)
	oResult.set( PLATFORM_WP8 );
#elif defined(OS_UWP)
	oResult.set( PLATFORM_UWP );
#else
	oResult.set( PLATFORM_WM_CE );
#endif
#elif defined(OS_ANDROID)
    oResult.set( PLATFORM_ANDROID );
#elif defined(OS_LINUX)
    oResult.set( L"LINUX" );
#else
	oResult.set( L"UNKNOWN" );
#endif			

#endif
}

void CSystemImplBase::getIsRhoSimulator(rho::apiGenerator::CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    oResult.set(true);
#else
    oResult.set(false);
#endif
}

void CSystemImplBase::getHasCamera(CMethodResult& oResult)
{
    oResult.set(true);
}

void CSystemImplBase::getPhoneNumber(CMethodResult& oResult)
{
    oResult.set(L"");
}

void CSystemImplBase::getDevicePushId(CMethodResult& oResult)
{
    rho::String strDeviceID;
	if ( rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl() ) 
		strDeviceID = rho::sync::RhoconnectClientManager::clientRegisterGetDevicePin();

    oResult.set( strDeviceID );
}

void CSystemImplBase::getOsVersion(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    oResult.set( RHOSIMCONF().getString("os_version") );
#endif
}

void CSystemImplBase::getIsSymbolDevice(CMethodResult& oResult)
{
    oResult.set(false);
}

void CSystemImplBase::getLocalServerPort(CMethodResult& oResult)
{
    oResult.set( atoi(RHODESAPP().getFreeListeningPort()) );
}

void CSystemImplBase::setLocalServerPort( int value, CMethodResult& oResult)
{
    //Local port can be set only in confuguration file
}

void CSystemImplBase::getNodejsServerPort(CMethodResult& oResult)
{
    oResult.set( atoi(RHODESAPP().getNodeJSListeningPort()) );
}

void CSystemImplBase::setNodejsServerPort( int value, CMethodResult& oResult)
{
    //Local port can be set only in confuguration file
}


void CSystemImplBase::getFreeServerPort(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set( RHODESAPP().determineFreeListeningPort() );
}

void CSystemImplBase::getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(true);
}

void CSystemImplBase::getHasSqlite(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(true);
}

void CSystemImplBase::getRealScreenWidth(CMethodResult& oResult)
{
    #if defined(OS_ANDROID)
		getRealScreenWidth(oResult);
    #else
		getScreenWidth(oResult);
    #endif
}

void CSystemImplBase::getRealScreenHeight(CMethodResult& oResult)
{
    #if defined(OS_ANDROID)
		getRealScreenHeight(oResult);
    #else
		getScreenHeight(oResult);
    #endif
}

void CSystemImplBase::getDeviceOwnerEmail(CMethodResult& oResult)
{
    oResult.set("");
}

void CSystemImplBase::getDeviceOwnerName(CMethodResult& oResult)
{
    oResult.set("");
}

void CSystemImplBase::getApplicationIconBadge(CMethodResult& oResult)
{
    oResult.set(0);
}

void CSystemImplBase::setApplicationIconBadge( int value, CMethodResult& oResult)
{
}

void CSystemImplBase::getStartParams(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set( RHODESAPP().getStartParameters() );
}

//TODO: move rho_sys_unzip_file here
extern "C" int rho_sys_unzip_file(const char* szZipPath, const char* psw, const char* outputFilename);
void CSystemImplBase::unzipFile( const rho::String& localPathToZip, const rho::String& password, const rho::String& outputFilename, rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set( rho_sys_unzip_file( localPathToZip.c_str(), password.c_str(), outputFilename.c_str() ) );
}

void CSystemImplBase::zipFile( const rho::String& localPathToZip,  const rho::String& localPathToFile,  const rho::String& password, CMethodResult& oResult)
{
    ZRESULT res;

#if defined(UNICODE) && defined(WIN32) && (!defined(OS_WP8) && !defined(OS_UWP))
    rho::StringW strZipFilePathW;
    convertToStringW(localPathToZip.c_str(), strZipFilePathW);

    CFilePath oPath(localPathToFile);
    rho::StringW strFileNameW;
    convertToStringW(oPath.getBaseName(), strFileNameW);

    rho::StringW strToZipPathW;
    convertToStringW(localPathToFile.c_str(), strToZipPathW);

    HZIP hz = CreateZip(strZipFilePathW.c_str(), password.c_str());
    if ( !hz )
        res = -1;
    else
    {
        res = ZipAdd( hz, strFileNameW.c_str(), strToZipPathW.c_str() );

        res = CloseZip(hz);
    }

#else
    HZIP hz = CreateZip((TCHAR*)localPathToZip.c_str(), password.c_str());
    if(!hz)
    {
        res = -1;
    } else
    {
        CFilePath oPath(localPathToFile);
        res = ZipAdd(hz, (TCHAR*)oPath.getBaseName(), (TCHAR*)localPathToFile.c_str());
        res = CloseZip(hz);
    }
#endif

    oResult.set(res);
}

void CSystemImplBase::zipFiles( const rho::String& localPathToZip,  const rho::String& basePath,  const rho::Vector<rho::String>& filePathsToZip,  const rho::String& password, CMethodResult& oResult)
{
    int nRes = rho_sys_zip_files_with_path_array_ptr( localPathToZip.c_str(), basePath.c_str(), filePathsToZip, password.c_str() );
    oResult.set(nRes);
}

struct rho_param;
extern "C" void rho_sys_replace_current_bundleEx(const char* path, const char* finish_callback, bool do_not_restart_app, bool not_thread_mode, bool check_filelist, bool all_via_callback_only);
void CSystemImplBase::replaceCurrentBundle( const rho::String& pathToBundle,  const rho::Hashtable<rho::String, rho::String>& params, rho::apiGenerator::CMethodResult& oResult)
{
    bool do_not_restart_app = false, not_thread_mode = false;
    if( params.containsKey("do_not_restart_app") )
        convertFromStringA( params.get("do_not_restart_app").c_str(), do_not_restart_app );
    if( params.containsKey("not_thread_mode") )
        convertFromStringA( params.get("not_thread_mode").c_str(), not_thread_mode );

    rho_sys_replace_current_bundleEx( pathToBundle.c_str(), params.containsKey("callback") ? params.get("callback").c_str():0, do_not_restart_app, not_thread_mode, false, false );
}

void CSystemImplBase::deleteFolder( const rho::String& pathToFolder, rho::apiGenerator::CMethodResult& oResult)
{
    CRhoFile::deleteFolder( pathToFolder.c_str() );
}


void CSystemImplBase::setDoNotBackupAttribute( const rho::String& pathToFile,  bool doNotBackup, rho::apiGenerator::CMethodResult& oResult)
{
    //iOS only
}

void CSystemImplBase::setRegistrySetting( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    //Windows only
	oResult.set(false);
}

void CSystemImplBase::getRegistrySetting( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    //Windows only
    oResult.set("");
}

void CSystemImplBase::deleteRegistrySetting( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
    //Windows only
	oResult.set(false);
}


//TODO: move to Database
void CSystemImplBase::isBlobAttr( const rho::String& partition,  int sourceID,  const rho::String& attrName, rho::apiGenerator::CMethodResult& oResult)
{
    bool bRes = db::CDBAdapter::getDB( partition.c_str()).getAttrMgr().isBlobAttr((int)sourceID, attrName.c_str());
    oResult.set(bRes);
}

//TODO: move to Database
extern "C" void rho_sys_update_blob_attribs(const char* szPartition, int source_id);
void CSystemImplBase::updateBlobAttribs( const rho::String& partition,  int sourceID, rho::apiGenerator::CMethodResult& oResult)
{
    rho_sys_update_blob_attribs( partition.c_str(), (int)sourceID );
}

void CSystemImplBase::get_locale(rho::apiGenerator::CMethodResult& oResult)
{
    getLocale(oResult);    
}

void CSystemImplBase::setPushNotification( const rho::String& url,  const rho::String& url_params,  const rho::String& push_client, rho::apiGenerator::CMethodResult& oResult)
{
    String strTypes = push_client.length() > 0 ? push_client : "default";

    String item;
    String::size_type pos = 0;

    while (String_getline(strTypes, item, pos, ';')) {
        RHODESAPP().setPushNotification( url, url_params, item);
    }
}

void CSystemImplBase::getHasNetwork(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(Network::hasNetwork());
}

void CSystemImplBase::getHasWifiNetwork(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(Network::hasWifiNetwork());
}

void CSystemImplBase::getHasCellNetwork(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(Network::hasCellNetwork());
}

void CSystemImplBase::setNetworkStatusNotify( const rho::String& url, int poll_interval, rho::apiGenerator::CMethodResult& oResult)
{
    oResult.setRubyCallback(url);
    RHODESAPP().setNetworkStatusNotify( oResult, (int)poll_interval );
}

void CSystemImplBase::clearNetworkStatusNotify(rho::apiGenerator::CMethodResult& oResult)
{
    RHODESAPP().clearNetworkStatusNotify();
}

void CSystemImplBase::setScreenRotationNotification( const rho::String& url,  const rho::String& url_params, rho::apiGenerator::CMethodResult& oResult)
{
	RHODESAPP().setScreenRotationNotification( url, url_params );
}

extern "C" void rho_sys_app_exit();
void CSystemImplBase::exit(rho::apiGenerator::CMethodResult& oResult)
{
    rho_sys_app_exit();
}

void CSystemImplBase::set_sleeping( bool enable, rho::apiGenerator::CMethodResult& oResult)
{
    setScreenSleeping(enable, oResult);
}

void CSystemImplBase::set_application_icon_badge( int badgeNumber, rho::apiGenerator::CMethodResult& oResult)
{
    setApplicationIconBadge(badgeNumber, oResult);
}

void CSystemImplBase::startTimer( int interval,  const rho::String& url,  const rho::String& url_params, rho::apiGenerator::CMethodResult& oResult)
{
    RHODESAPP().getTimer().addTimer( (int)interval, url.c_str(), url_params.c_str() );
}

void CSystemImplBase::stopTimer( const rho::String& url, rho::apiGenerator::CMethodResult& oResult)
{
    RHODESAPP().getTimer().stopTimer( url.c_str());
}

extern "C" void rho_sys_set_http_proxy_url(const char* url);
void CSystemImplBase::set_http_proxy_url( const rho::String& proxyURI, rho::apiGenerator::CMethodResult& oResult)
{
    rho_sys_set_http_proxy_url( proxyURI.c_str() );
}

extern "C" void rho_sys_unset_http_proxy();
void CSystemImplBase::unset_http_proxy(rho::apiGenerator::CMethodResult& oResult)
{
    rho_sys_unset_http_proxy();
}

extern "C" const char* rho_sys_get_http_proxy_url();
void CSystemImplBase::getHttpProxyURI(CMethodResult& oResult)
{
    oResult.set(rho_sys_get_http_proxy_url());
}

void CSystemImplBase::setHttpProxyURI( const rho::String& value, CMethodResult& oResult)
{
    if ( value.length() )
        rho_sys_set_http_proxy_url( value.c_str() );
    else
        rho_sys_unset_http_proxy();
}

void CSystemImplBase::getKeyboardState(CMethodResult& oResult)
{
    //windows only
    oResult.set("automatic");
}

void CSystemImplBase::setKeyboardState( const rho::String & value, CMethodResult& oResult)
{
    //windows only
}

void CSystemImplBase::getMain_window_closed(rho::apiGenerator::CMethodResult& oResult)
{
    //windows only
}

void CSystemImplBase::hideSplashScreen(rho::apiGenerator::CMethodResult& result)
{
    //Android only
}

void CSystemImplBase::getSystemInfo(rho::apiGenerator::CMethodResult& oResult)
{
    //Override for specific platform in corresponding subclasses
       rho::Hashtable<rho::String, rho::String> retVal;
       oResult.set( retVal );
}

}

#if defined(OS_WINDOWS_DESKTOP) || defined(RHODES_EMULATOR)
extern "C" void rho_qt_unset_window_proxy();
extern "C" void rho_qt_set_window_proxy(const char* host, const char* port, const char* login, const char* password);
#endif

rho::String g_strHttpProxy;


extern "C" void rho_sys_unset_http_proxy()
{
    g_strHttpProxy = "";
#if defined(OS_WINDOWS_DESKTOP) || defined(RHODES_EMULATOR)
	rho_qt_unset_window_proxy();
#endif
	RHOCONF().removeProperty("http_proxy_host", false);
	RHOCONF().removeProperty("http_proxy_port", false);
	RHOCONF().removeProperty("http_proxy_login", false);
	RHOCONF().removeProperty("http_proxy_password", false);
	RAWLOG_INFO("Unsetting HTTP proxy");
}

void parseHttpProxyURI(const rho::String &http_proxy)
{
	// http://<login>:<passwod>@<host>:<port>
	const char *default_port = "8080";

	if (http_proxy.length() == 0)
		rho_sys_unset_http_proxy();

	if (http_proxy.length() < 8) {
		RAWLOG_ERROR("invalid http proxy url");
		return;
	}

	int index = http_proxy.find("http://", 0, 7);
	if (index == std::string::npos) {
		RAWLOG_ERROR("http proxy url should starts with \"http://\"");
		return;
	}
	index = 7;

	enum {
		ST_START,
		ST_LOGIN,
		ST_PASSWORD,
		ST_HOST,
		ST_PORT,
		ST_FINISH
	};

	rho::String token, login, password, host, port;
	char c, state = ST_START, prev_state = state;
	int length = http_proxy.length();

	for (int i = index; i < length; i++) {
		c = http_proxy[i];

		switch (state) {
		case ST_START:
			if (c == '@') {
				prev_state = state; state = ST_HOST;
			} else if (c == ':') {
				prev_state = state; state = ST_PASSWORD;
			} else {
				token +=c;
				state = ST_HOST;
			}
			break;
		case ST_HOST:
			if (c == ':') {
				host = token; token.clear();			
				prev_state = state; state = ST_PORT;
			} else if (c == '@') {
				host = token; token.clear();		
				prev_state = state;	state = ST_LOGIN;					
			} else {
				token += c;
				if (i == (length - 1)) {
					host = token; token.clear();								
				}
			}
			break;
		case ST_PORT:
			if (c == '@') {
				port = token; token.clear();			
				prev_state = state; state = ST_LOGIN;
			} else {
				token += c;
				if (i == (length - 1)) {
					port = token; token.clear();
				}
			}
			break;
		case ST_LOGIN:
			if (prev_state == ST_PORT || prev_state == ST_HOST) {
				login    = host; host.clear();
				password = port; port.clear();
				prev_state = state; state = ST_HOST;
				token += c;
			} else {
				token += c;
				if (i == (length - 1)) {
					login = token; token.clear();								
				}
			}
			break;
		case ST_PASSWORD:
			if (c == '@') {
				password = token; token.clear();			
				prev_state = state; state = ST_HOST;
			} else {
				token += c;
				if (i == (length - 1)) {
					password = token; token.clear();								
				}
			}
			break;
		default:
			;
		}
	}

	RAWLOG_INFO("Setting up HTTP proxy:");
	RAWLOG_INFO1("URI: %s", http_proxy.c_str());
	RAWLOG_INFO1("HTTP proxy login    = %s", login.c_str());
	RAWLOG_INFO1("HTTP proxy password = %s", password.c_str());
	RAWLOG_INFO1("HTTP proxy host     = %s", host.c_str());
	RAWLOG_INFO1("HTTP proxy port     = %s", port.c_str());

	if (host.length()) {
#if defined(OS_WINDOWS_DESKTOP) || defined(RHODES_EMULATOR)
		rho_qt_set_window_proxy(host.c_str(), port.c_str(), login.c_str(), password.c_str());
#endif
		RHOCONF().setString ("http_proxy_host", host, false);

		if (port.length()){
			RHOCONF().setString ("http_proxy_port", port, false);
		} else {
			RAWLOG_WARNING("there is no proxy port defined");
		}

		if (login.length())
			RHOCONF().setString ("http_proxy_login", login, false);

		if (password.length())
			RHOCONF().setString ("http_proxy_password", password, false);

	} else {
		RAWLOG_ERROR("empty host name in HTTP-proxy URL");
	}
}

extern "C" void rho_sys_set_http_proxy_url(const char* url)
{
	g_strHttpProxy = url;
    parseHttpProxyURI(g_strHttpProxy);
}

extern "C" const char* rho_sys_get_http_proxy_url()
{
    return g_strHttpProxy.c_str();
}
