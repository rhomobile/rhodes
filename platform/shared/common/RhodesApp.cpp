/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoFile.h"
#include "common/RhoConf.h"
#include "common/RhoFilePath.h"
#include "common/RhoAppAdapter.h"
#include "statistic/RhoProfiler.h"
#include "net/INetRequest.h"
#include "sync/RhoconnectClientManager.h"
#include "net/URI.h"

#include "db/DBAdapter.h"
#include "net/HttpServer.h"
#include "ruby/ext/rho/rhoruby.h"
#include "rubyext/WebView.h"
#include "rubyext/GeoLocation.h"
#include "common/app_build_configs.h"
#include "common/app_build_capabilities.h"
#include "unzip/unzip.h"
#include "common/Tokenizer.h"
#include "api_generator/js_helpers.h"
#include "api_generator/StringifyHelper.h"
#include "coreapi/ext/shared/Application.h"

#include <algorithm>


#ifdef OS_WINCE
#include <winsock.h>
#endif 

using rho::net::HttpHeader;
using rho::net::HttpHeaderList;
using rho::net::CHttpServer;

extern "C" {
void rho_map_location(char* query);
void rho_appmanager_load( void* httpContext, const char* szQuery);
void rho_db_init_attr_manager();
void rho_sys_app_exit();
void rho_sys_report_app_started();
void rho_conf_show_log();
bool rho_wmimpl_is_browser_ieforwm();

#ifdef OS_ANDROID
void rho_file_set_fs_mode(int mode);
#endif

#ifdef OS_MACOSX
#ifndef RHODES_EMULATOR
void rho_cocoa_server_init();
void rho_cocoa_server_start();
#endif
#endif

}

// Copy-paste from ApplicationBase.cpp
static const char APP_EVENT_ACTIVATED[] = "Activated";
static const char APP_EVENT_DEACTIVATED[] = "Deactivated";
static const char APP_EVENT_UICREATED[] = "UICreated";
static const char APP_EVENT_UIDESTROYED[] = "UIDestroyed";
static const char APP_EVENT_SYNCUSERCHANGED[] = "SyncUserChanged";
static const char APP_EVENT_CONFIGCONFLICT[] = "ConfigConflict";
static const char APP_EVENT_DBMIGRATESOURCE[] = "DBMigrateSource";
static const char APP_EVENT_UNINITIALIZED[] = "Uninitialized";
static const char APP_EVENT_SCREEN_ON[] = "ScreenOn";
static const char APP_EVENT_SCREEN_OFF[] = "ScreenOff";

static const char APP_EVENT[] = "applicationEvent";
static const char APP_EVENT_DATA[] = "eventData";

namespace rho {
namespace common{

IMPLEMENT_LOGCLASS(CRhodesApp,"RhodesApp");
String CRhodesApp::m_strStartParameters, CRhodesApp::m_strStartParametersOriginal;
boolean CRhodesApp::m_bSecurityTokenNotPassed = false;

class CAppCallbacksQueue : public CThreadQueue
{
    DEFINE_LOGCLASS;
public:
    enum callback_t
    {
        local_server_restart,
        local_server_started,
        ui_created,
        screen_on,
        app_activated,
        app_deactivated
    };

public:
    CAppCallbacksQueue();
	//CAppCallbacksQueue(LogCategory logCat);
	~CAppCallbacksQueue();

    //void call(callback_t type);

    friend class Command;
    struct Command : public IQueueCommand
    {
        callback_t type;
        Command(callback_t t) :type(t) {}

        boolean equals(IQueueCommand const &) {return false;}
        String toString() {return CAppCallbacksQueue::toString(type);}
    };

private:

    enum ui_created_state
    {
    	ui_not_available,
    	ui_created_received,
    	ui_created_processed
    };

    void processCommand(IQueueCommand* pCmd);
    void processUiCreated();

    bool hasCommand(callback_t type);

    static char const *toString(int type);
    void   callCallback(const String& strCallback);

private:
    callback_t m_expected;
    ui_created_state m_uistate;

    Vector<int> m_commands;
    boolean m_bFirstServerStart;
};
IMPLEMENT_LOGCLASS(CAppCallbacksQueue,"AppCallbacks");

char const *CAppCallbacksQueue::toString(int type)
{
    switch (type)
    {
    case local_server_started:
        return "LOCAL-SERVER-STARTED";
    case ui_created:
        return "UI-CREATED";
    case app_activated:
        return "APP-ACTIVATED";
    case app_deactivated:
        return "APP-DEACTIVATED";
    case local_server_restart:
        return "LOCAL-SERVER-RESTART";
    case screen_on:
        return "SCREEN-ON";
    default:
        return "UNKNOWN";
    }
}

CAppCallbacksQueue::CAppCallbacksQueue()
    :CThreadQueue(), m_expected(local_server_started), m_uistate(ui_not_available), m_bFirstServerStart(true)
{
    CThreadQueue::setLogCategory(getLogCategory());
    setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);
    start(epNormal);
}

CAppCallbacksQueue::~CAppCallbacksQueue()
{
}

void CAppCallbacksQueue::callCallback(const String& strCallback)
{
    if ( !rho_ruby_is_started() )
        return;

    String strUrl = RHODESAPP().getBaseUrl();
    strUrl += strCallback;
    
    boolean force_https = false;
#ifdef OS_MACOSX
    if (rho_conf_is_property_exists("ios_https_local_server")!=0) {
        force_https = rho_conf_getBool("ios_https_local_server")!=0;
    }
#endif
    
    NetResponse resp = getNetRequest().pullData( strUrl, NULL );
    if ( !resp.isOK() )
    {
        boolean bTryAgain = false;
#if defined( __SYMBIAN32__ ) || defined( OS_ANDROID )
        if ( String_startsWith( strUrl, "http://localhost:" ) )
        {
            RHODESAPP().setBaseUrl("http://127.0.0.1:");
            bTryAgain = true;
        }
#else
        String addr_number;
        String addr_local;
        if (force_https) {
            addr_number = "mohus";
            addr_local = "mohus";
        }
        else {
            addr_number = "http";
            addr_local = "http";
        }
        addr_number = addr_number + "://127.0.0.1:";
        addr_local = addr_local + "://localhost:";
        if ( String_startsWith( strUrl, addr_number ) )
        {
            RHODESAPP().setBaseUrl(addr_local);
            bTryAgain = true;
        }
#endif

        if ( bTryAgain )
        {
            LOG(INFO) + "Change base url and try again.";
            strUrl = RHODESAPP().getBaseUrl();
            strUrl += strCallback;
            resp = getNetRequest().pullData( strUrl, NULL );
        }

        if ( !resp.isOK() )
            LOG(ERROR) + strCallback + " call failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();
    }
}

bool CAppCallbacksQueue::hasCommand(callback_t type)
{
    for( int i = 0; i < (int)m_commands.size() ; i++)
    {
        if ( m_commands.elementAt(i) == type )
        {
        	return true;
        }
    }
	return false;
}

void CAppCallbacksQueue::processCommand(IQueueCommand* pCmd)
{
    Command *cmd = (Command *)pCmd;
    if (!cmd)
        return;

    LOG(INFO) + toString(cmd->type) + " is received ++++++++++++++++++++++++++++";

    if (cmd->type == ui_created)
    {
        LOG(INFO) + "m_uistate: " + (int)m_uistate;

        if (m_uistate != ui_not_available)
            return;
        m_uistate = ui_created_received;
    }

    if ( m_expected == app_deactivated && cmd->type == app_activated )
    {
        LOG(INFO) + "received duplicate app_activated - skip it";
        return;
    }

    if ( m_expected == local_server_restart )
    {
        if ( cmd->type != local_server_started )
        {
            LOG(INFO) + "Restart local server ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
            RHODESAPP().restartLocalServer(*this);
            sleep(50);
            LOG(INFO) + "Continue after server restart =======================================";
        }
        else
            LOG(INFO) + "Local server restarted before activate.Do not restart it again.";

        m_expected = local_server_started;
    }
  
    Vector<int> commandsLocalCopy;
  
    { //Command queue lock scope
      synchronized(getCommandLock());


      if (cmd->type > m_expected)
      {
          if ( hasCommand(cmd->type) )
          {
              LOG(INFO) + "Received duplicate command " + toString(cmd->type) + "skip it";
              return;
          }else
          {
              // Don't do that now
              LOG(INFO) + "Received command " + toString(cmd->type) + " which is greater than expected (" + toString(m_expected) + ") - postpone it";

              if (cmd->type == app_deactivated && m_expected != local_server_started)
              {
                  m_commands.clear();
                  m_commands.push_back(cmd->type);
              }
              else
              {
                  m_commands.push_back(cmd->type);
                  std::sort(m_commands.begin(), m_commands.end());
                  return;
              }
          }
      }
      else
      {
          m_commands.insert(m_commands.begin(), cmd->type);
      }
      
      m_commands.swap( commandsLocalCopy );
      
    //Command queue lock scope
    }

    for (Vector<int>::const_iterator it = commandsLocalCopy.begin(), lim = commandsLocalCopy.end(); it != lim; ++it)
    {
        int type = *it;
        LOG(INFO) + "process command: " + toString(type) + "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
        switch (type)
        {
        case app_deactivated:
#if !defined( WINDOWS_PLATFORM )
            m_expected = local_server_restart;
#else
            m_expected = app_activated;
#endif
            break;

        case local_server_started:
            if ( m_bFirstServerStart )
            {
                m_expected = ui_created;
                m_bFirstServerStart = false;
            }
            else
                m_expected = app_activated;

            rho_sys_report_app_started();
            break;
        case ui_created:
            if (m_uistate != ui_created_processed)
            {
                processUiCreated();
                m_expected = app_activated;
            }
            break;
        case app_activated:
            {
                if ( m_uistate == ui_created_received )
                {
                    processUiCreated();
                }

                RHODESAPP().getApplicationEventReceiver()->onAppStateChange(rho::common::applicationStateActivated);
                callCallback("/system/activateapp");

                m_expected = app_deactivated;
            }
            break;
        case screen_on:
            RHODESAPP().getApplicationEventReceiver()->onDeviceScreenEvent(rho::common::screenOn);
            break;
        }
    }
    //m_commands.clear();
}

void CAppCallbacksQueue::processUiCreated()
{
    rho::String startPath = RHOCONF().getString("start_path");

    // handle security token validation
    #if !defined(OS_WP8) && !defined(OS_UWP)
    rho::String invalidSecurityTokenStartPath =  RHOCONF().getString("invalid_security_token_start_path");

    if (RHODESAPP().isSecurityTokenNotPassed()) {
        if (invalidSecurityTokenStartPath.length() > 0) {
            startPath = invalidSecurityTokenStartPath;
        } else {
            // exit from application - old way
            LOGC(FATAL, "EROOR" ) + "processApplicationEvent: security_token is not passed - application will closed";
            rho_sys_app_exit();
        }
    }
    #endif

    // at this point JS app is unlikely to set its own handler, just navigate to overriden start path
    RHODESAPP().getApplicationEventReceiver()->onUIStateChange(rho::common::UIStateCreated);

        if ( rho_ruby_is_started() && (!RHODESAPP().isRubyNodeJSApplication()))
            callCallback("/system/uicreated");
#if !defined(APP_BUILD_CAPABILITY_SHARED_RUNTIME) || !defined(OS_ANDROID)
        else
            rho_webview_navigate(startPath.c_str(), 0);
#endif

    m_uistate = ui_created_processed;
}

/*static*/ CRhodesApp* CRhodesApp::Create(const String& strRootPath, const String& strUserPath, const String& strRuntimePath)
{
    if ( m_pInstance != NULL) return (CRhodesApp*)m_pInstance;

    m_pInstance = new CRhodesApp(strRootPath, strUserPath, strRuntimePath);

    return (CRhodesApp*)m_pInstance;
}

/*static*/void CRhodesApp::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

#if defined(WINDOWS_PLATFORM) 
void CRhodesApp::waitAppStarted()
{
    #ifndef WINCE
    std::unique_lock<std::mutex> lock(wait_mutex);
    activated_cond.wait(lock);
    #endif
}

void CRhodesApp::notifyAppStared()
{
    #ifndef WINCE
    activated_cond.notify_one();
    #endif
}
#endif

CRhodesApp::CRhodesApp(const String& strRootPath, const String& strUserPath, const String& strRuntimePath)
    :CRhodesAppBase(strRootPath, strUserPath, strRuntimePath), 
	m_networkStatusReceiver(m_mxNetworkStatus)
{
    m_bExit = false;
    m_bDeactivationMode = false;
    m_bRestartServer = false;
    //m_activateCounter = 0;
    //m_pExtManager = 0;
	m_pNetworkStatusMonitor = 0;

	m_appCallbacksQueue = new CAppCallbacksQueue();
	
#if defined(WINDOWS_PLATFORM)
    //initializing winsock
    WSADATA WsaData;
	m_cameraOpened = false;
    int result = WSAStartup(MAKEWORD(2,2),&WsaData);
#endif

    initAppUrls();
#ifndef AJAXSERVER
    if(!m_isJSFSApp) initHttpServer();
#else
    initHttpServer();
#endif

    getSplashScreen().init();
}

void CRhodesApp::startApp()
{
    start(epNormal);
}

extern "C" void Init_Extensions(void);

void CRhodesApp::RhoJsStart()
{
    LOG(INFO) + "Starting pure JS application...";

    const char* szBlobPath = getBlobsDirPath().c_str();
    const char* szUserPath = rho_native_rhodbpath();
    LOG(INFO) + "Init_RhoBlobs:" + szBlobPath;

    CRhoFile::recursiveCreateDir(szBlobPath, szUserPath);

    PROF_START("EXTENSIONS_INIT");
    Init_Extensions();
    PROF_STOP("EXTENSIONS_INIT");

    void *dbObj = NULL;

    rho::String partName = rho::db::CDBAdapter::USER_PARTITION_NAME();
    rho::String dbPath   = rho::Application::databaseFilePath(partName.c_str());

    rho_db_open(dbPath.c_str(), partName.c_str(), &dbObj);

    //rho_webview_navigate(RHOCONF().getString("start_path").c_str(), 0);
}

void CRhodesApp::run()
{
    LOG(INFO) + "Starting RhodesApp main routine...";
#ifndef RHO_NO_RUBY_API
    if (!isJSApplication() && !isNodeJSApplication())
    {
        RhoRubyStart();
        rubyext::CGeoLocation::Create();
    }
    else
#endif
        RhoJsStart();
    

	if ( sync::RhoconnectClientManager::haveRhoconnectClientImpl() ) {
		LOG(INFO) + "Starting sync engine...";
		sync::RhoconnectClientManager::syncThreadCreate();
	}
#ifndef RHO_NO_RUBY_API
    if (!isJSApplication() && !isNodeJSApplication())
    {
        LOG(INFO) + "RhoRubyInitApp...";
        RhoRubyInitApp();
        
        HashtablePtr<String,Vector<String>* >& mapConflicts = RHOCONF().getConflicts();
        bool handled = mapConflicts.size()==0;
        
        if (!handled)
        {
            handled = m_applicationEventReceiver.onReinstallConfigUpdate(mapConflicts);
        }
        if (!handled)
        {
            rho_ruby_call_config_conflicts();
        }
    }
#endif
    RHOCONF().conflictsResolved();

    PROF_CREATE_COUNTER("READ_FILE");
    PROF_CREATE_COUNTER("LOW_FILE");
	if(m_isJSFSApp)
		RHODESAPP().notifyLocalServerStarted();

#ifdef OS_MACOSX
  bool shouldRunDirectQueue = false;
  bool shouldUseCocoaServer = true;
    
  net::CDirectHttpRequestQueue directQueue(*m_httpServer, *this );

    if (RHOCONF().isExist("ios_direct_local_requests")) {
        shouldRunDirectQueue = RHOCONF().getBool("ios_direct_local_requests");
    }
    if (RHOCONF().isExist("ios_use_old_legacy_socket")) {
        shouldUseCocoaServer = !RHOCONF().getBool("ios_use_old_legacy_socket");
    }
#ifdef RHODES_EMULATOR
    shouldRunDirectQueue = false;
    shouldUseCocoaServer = false;
#endif

    if ((shouldUseCocoaServer) && (!shouldRunDirectQueue)) {
#ifndef RHODES_EMULATOR
        rho_cocoa_server_init();
#endif
    }

#endif


	while (!m_bExit) {
		if(!m_isJSFSApp)
    {
#ifdef OS_MACOSX
      if ( shouldRunDirectQueue || shouldUseCocoaServer)
      {
          if ((shouldUseCocoaServer) && (!shouldRunDirectQueue)) {
#ifndef RHODES_EMULATOR
              rho_cocoa_server_start();
#endif
          }
        directQueue.run();
      }
      else
#endif
      {
        m_httpServer->run();
      }
      
    }
		else
		{
			LOG(INFO) + "RhodesApp thread wait.";
			wait(-1);
		}

		if (m_bExit)
			break;

		if ( !m_bRestartServer )
		{
			LOG(INFO) + "RhodesApp thread wait.";
			wait(-1);
		}
		m_bRestartServer = false;
	}

    PROF_DESTROY_COUNTER("LOW_FILE");
    PROF_DESTROY_COUNTER("READ_FILE");

    LOG(INFO) + "RhodesApp thread shutdown";

    getExtManager().close();

    if (!isJSApplication() && !isNodeJSApplication())
    {
        rubyext::CGeoLocation::Destroy();
    }

    if ( sync::RhoconnectClientManager::haveRhoconnectClientImpl() ) 
    {
      sync::RhoconnectClientManager::clientRegisterDestroy();
      sync::RhoconnectClientManager::syncThreadDestroy();
    }

    db::CDBAdapter::closeAll();
#ifndef RHO_NO_RUBY_API
    if (!isJSApplication() && !isNodeJSApplication())
    {
        RhoRubyStop();
    }
#endif
}

CRhodesApp::~CRhodesApp(void)
{
    stopApp();

	LOGCONF().closeRemoteLog();

#ifdef OS_WINCE
    WSACleanup();
#endif
}

boolean CRhodesApp::callTimerCallback(const String& strUrl, const String& strData)
{
    String strBody = "rho_callback=1";
		
    if ( strData.length() > 0 )
        strBody += "&" + strData;
    
    callCallbackWithData(strUrl, "", strData, false);
    return true;
}

void CRhodesApp::restartLocalServer(common::CThreadQueue& waitThread)
{
    LOG(INFO) + "restart local server.";
    m_bRestartServer = true;
	if(!m_isJSFSApp)
    {
		m_httpServer->stop();
    }else
        RHODESAPP().notifyLocalServerStarted();
    
	stopWait();
}

void CRhodesApp::stopApp()
{
    if (m_bExit)
        return;

   	m_appCallbacksQueue->stop(1000);

    if (!m_bExit)
    {
        m_bExit = true;
		if(!m_isJSFSApp)
			m_httpServer->stop();
        stopWait();
        stop(4000);
    }

    #ifdef OS_ANDROID
        // Switch Android libc hooks to FS only mode
        rho_file_set_fs_mode(0);
    #endif
}

class CRhoCallbackCall
{
    String m_strCallback, m_strBody;
public:
    CRhoCallbackCall(const String& strCallback, const String& strBody)
      : m_strCallback(strCallback), m_strBody(strBody)
    {}

    void run(common::CRhoThread &)
    {
        if ( !rho_ruby_is_started() )
            return;

        getNetRequest().pushData( m_strCallback, m_strBody, NULL );
    }
};

void CRhodesApp::runCallbackInThread(const String& strCallback, const String& strBody)
{
    rho_rhodesapp_call_in_thread(new CRhoCallbackCall(strCallback, strBody ) );
}

static void callback_activateapp(void *arg, String const &strQuery)
{
#ifndef RHO_NO_RUBY_API
    if (!RHODESAPP().isJSApplication() && !RHODESAPP().isNodeJSApplication())
        rho_ruby_activateApp();
#endif

    String strMsg;
    rho_http_sendresponse(arg, strMsg.c_str());
}

static void callback_deactivateapp(void *arg, String const &strQuery)
{
#ifndef RHO_NO_RUBY_API
    if (!RHODESAPP().isJSApplication() && !RHODESAPP().isNodeJSApplication())
        rho_ruby_deactivateApp();
#endif

    String strMsg;
    rho_http_sendresponse(arg, strMsg.c_str());
}

static void callback_uicreated(void *arg, String const &strQuery)
{
#ifndef RHO_NO_RUBY_API
    if (!RHODESAPP().isJSApplication() && !RHODESAPP().isNodeJSApplication())
        rho_ruby_uiCreated();
#endif

    rho_http_sendresponse(arg, "");
}

static void callback_uicloserequest(void *arg, String const &strQuery)
{
#ifndef RHO_NO_RUBY_API
    if (!RHODESAPP().isJSApplication() && !RHODESAPP().isNodeJSApplication())
        rho_ruby_uiCloseRequest();
#endif

    rho_http_sendresponse(arg, "");
}

static void callback_uidestroyed(void *arg, String const &strQuery)
{
#ifndef RHO_NO_RUBY_API
    if (!RHODESAPP().isJSApplication() && !RHODESAPP().isNodeJSApplication())
        rho_ruby_uiDestroyed();
#endif

    rho_http_sendresponse(arg, "");
}

static void callback_loadserversources(void *arg, String const &strQuery)
{
    RhoAppAdapter.loadServerSources(strQuery);
    String strMsg;
    rho_http_sendresponse(arg, strMsg.c_str());
}

static void callback_loadallsyncsources(void *arg, String const &strQuery)
{
    RhoAppAdapter.loadAllSyncSources();
    String strMsg = "";
    rho_http_sendresponse(arg, strMsg.c_str());
}

static void callback_resetDBOnSyncUserChanged(void *arg, String const &strQuery)
{
    RhoAppAdapter.resetDBOnSyncUserChanged();
    String strMsg;
    rho_http_sendresponse(arg, strMsg.c_str());
}

void CRhodesApp::callUiCreatedCallback()
{
    m_appCallbacksQueue->addQueueCommand(new CAppCallbacksQueue::Command(CAppCallbacksQueue::ui_created));
}

int CRhodesApp::callUiCloseRequestCallback()
{
    if ( m_bExit || !rho_ruby_is_started()) return 1;

    String strUrl = m_strHomeUrl + "/system/uicloserequest";
    NetResponse resp = getNetRequest().pullData( strUrl, NULL );
    if ( !resp.isOK() )
    {
        LOG(ERROR) + "UI close request callback failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();
        return 1;
    }else{
        return 0;
    }
    return 1;

}

void CRhodesApp::callUiDestroyedCallback()
{
    if ( m_bExit/* || !rho_ruby_is_started()*/ )
        return;
    
    
    RHODESAPP().getApplicationEventReceiver()->onUIStateChange(rho::common::UIStateDestroyed);
    if ( rho_ruby_is_started() )
    {
        String strUrl = m_strHomeUrl + "/system/uidestroyed";
        NetResponse resp = getNetRequest().pullData( strUrl, NULL );
        if ( !resp.isOK() )
        {
            LOG(ERROR) + "UI destroy callback failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();
        }
    }
}
	
void CRhodesApp::callAppActiveCallback(boolean bActive)
{
    if ( m_bExit )
        return;

    LOG(INFO) + "callAppActiveCallback";
    if (bActive)
    {
        // Restart server each time when we go to foreground
/*        if (m_activateCounter++ > 0)
        {
#if !defined( WINDOWS_PLATFORM )
            m_httpServer->stop();
#endif
            this->stopWait();

        } */

        m_appCallbacksQueue->addQueueCommand(new CAppCallbacksQueue::Command(CAppCallbacksQueue::app_activated));
    }
    else
    {
        // Deactivation callback must be called in place (not in separate thread!)
        // This is because system can kill application at any time after this callback finished
        // So to guarantee user code is executed on deactivation, we must not exit from this function
        // until application finish executing of user-defined deactivation callback.
        // However, blocking UI thread can cause problem with API refering to UI (such as WebView.navigate etc)
        // To fix this problem, new mode 'deactivation' introduced. When this mode active, no UI operations allowed.
        // All such operation will throw exception in ruby code when calling in 'deactivate' mode.
        m_bDeactivationMode = true;
        m_appCallbacksQueue->addQueueCommand(new CAppCallbacksQueue::Command(CAppCallbacksQueue::app_deactivated));
        
        // TODO: Support stop_local_server command to be parsed from callback result
        RHODESAPP().getApplicationEventReceiver()->onAppStateChange(rho::common::applicationStateDeactivated);

#ifndef RHO_NO_RUBY_API 
            if ( rho_ruby_is_started() )
            {
                String strUrl = m_strHomeUrl + "/system/deactivateapp";
                NetResponse resp = getNetRequest().pullData( strUrl, NULL );
                if ( !resp.isOK() )
                {
                    LOG(ERROR) + "deactivate app failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();
                }else
                {
                    const char* szData = resp.getCharData();
                    boolean bStop = szData && strcmp(szData,"stop_local_server") == 0;

                    if (bStop)
                    {
        #if !defined( WINDOWS_PLATFORM )
                        LOG(INFO) + "Stopping local server.";
                        m_httpServer->stop();
        #endif
                    }
                }
            }
#endif

        m_bDeactivationMode = false;
    }
}

void CRhodesApp::callBarcodeCallback(String strCallbackUrl, const String& strBarcode, bool isError) 
{
    strCallbackUrl = canonicalizeRhoUrl(strCallbackUrl);
    String strBody;
    strBody = "barcode=" + strBarcode;

    if (isError)
    {
        strBody += "&status=ok";
    }
    else
    {
        strBody += "&status=fail";
    }

    strBody += "&rho_callback=1";

    runCallbackInThread(strCallbackUrl, strBody);
}
//Due to the Multithreading issue callback was causing problem. That has been fixed in this.
#if ( !defined(OS_MACOSX) && (defined(OS_WINDOWS_DESKTOP) ||  defined(RHODES_EMULATOR)))
String g_strCallbackUrl;
String g_strBody;

	DWORD WINAPI CallbackExecThread( LPVOID lpParam ) 
	{
		 if ( !rho_ruby_is_started() )
	            return 0;
		 getNetRequest().pushData( g_strCallbackUrl, g_strBody, NULL );
		 return 0;
	}

#endif
void CRhodesApp::callCallbackWithData(String strCallbackUrl, String strBody, const String& strCallbackData, bool bWaitForResponse) 
{
    strCallbackUrl = canonicalizeRhoUrl(strCallbackUrl);

    LOG(TRACE) + "Call back URL: " + strCallbackUrl;

    strBody += "&rho_callback=1";

    if (strCallbackData.length() > 0 )
    {
        if ( !String_startsWith( strCallbackData, "&" ) )
            strBody += "&";

        strBody += strCallbackData;
    }

#if ( !defined(OS_MACOSX) && (defined(OS_WINDOWS_DESKTOP) ||  defined(RHODES_EMULATOR)) )
	g_strCallbackUrl = strCallbackUrl;
	g_strBody = strBody;
#endif

    if (bWaitForResponse)
        getNetRequest().pushData( strCallbackUrl, strBody, NULL );
    else
	{
#if ( !defined(OS_MACOSX) && (defined(OS_WINDOWS_DESKTOP) ||  defined(RHODES_EMULATOR)) )
		LOG(INFO) + strCallbackUrl + "in a Windows Specific Thread";
		CloseHandle(::CreateThread(NULL,0,CallbackExecThread,NULL,0,NULL));
#else
		runCallbackInThread(strCallbackUrl, strBody);
#endif
	}
}

void CRhodesApp::callCallbackProcWithData(unsigned long oRubyCallbackProc, String strBody, const String& strCallbackData, bool bWaitForResponse) 
{
    strBody += "&rho_callback=1";

    if (strCallbackData.length() > 0 )
    {
        if ( !String_startsWith( strCallbackData, "&" ) )
            strBody += "&";

        strBody += strCallbackData;
    }

    String strCallbackUrl = "/system/call_ruby_proc_callback?";
    strCallbackUrl += convertToStringA(oRubyCallbackProc);
    strCallbackUrl = canonicalizeRhoUrl(strCallbackUrl);

    if (bWaitForResponse)
        getNetRequest().pushData( strCallbackUrl, strBody, NULL );
    else
        runCallbackInThread(strCallbackUrl, strBody);
}

extern "C" VALUE rjson_tokener_parse(const char *str, char** pszError );

class CJsonResponse : public rho::ICallbackObject
{
    String m_strJson;
public:
    CJsonResponse(const String& strJson) : m_strJson(strJson) { }
    CJsonResponse(const char* szJson) : m_strJson(szJson) { }
#ifndef RHO_NO_RUBY_API
    virtual unsigned long getObjectValue()
    {
        char* szError = 0;
        unsigned long valBody = rjson_tokener_parse(m_strJson.c_str(), &szError);
        if ( valBody != 0 )
            return valBody;

        LOG(ERROR) + "Incorrect json body.Error:" + (szError ? szError : "");
        if ( szError )
            free(szError);

        return rho_ruby_get_NIL();
    }
#else
    virtual unsigned long getObjectValue()
    {
        return 0;
    }

#endif
};

void CRhodesApp::callCallbackWithJsonBody( const char* szCallback, const char* szCallbackBody, const char* szCallbackData, bool bWaitForResponse)
{
    String strBody;
    strBody = addCallbackObject( new CJsonResponse( szCallbackBody ), "__rho_inline" );

    callCallbackWithData(szCallback, strBody, szCallbackData, bWaitForResponse );
}

void CRhodesApp::callCameraCallback(String strCallbackUrl, const String& strImagePath, 
    const String& strError, boolean bCancel )
{
    strCallbackUrl = canonicalizeRhoUrl(strCallbackUrl);
    String strBody;
    if ( bCancel || strError.length() > 0 )
    {
        if ( bCancel )
            strBody = "status=cancel&message=User canceled operation.";
        else
            strBody = "status=error&message=" + strError;
    }else
        strBody = "status=ok&image_uri=db%2Fdb-files%2F" + strImagePath;


    strBody += "&rho_callback=1";
    getNetRequest().pushData( strCallbackUrl, strBody, NULL );
}

void CRhodesApp::callSignatureCallback(String strCallbackUrl, const String& strSignaturePath, 
										const String& strError, boolean bCancel ) 
	{
		strCallbackUrl = canonicalizeRhoUrl(strCallbackUrl);
		String strBody;
		if ( bCancel || strError.length() > 0 )
		{
			if ( bCancel )
				strBody = "status=cancel&message=User canceled operation.";
			else
				strBody = "status=error&message=" + strError;
		}else
			strBody = "status=ok&signature_uri=db%2Fdb-files%2F" + strSignaturePath;
		
		strBody += "&rho_callback=1";
		//getNetRequest().pushData( strCallbackUrl, strBody, NULL );
        runCallbackInThread(strCallbackUrl, strBody);
	}
	
void CRhodesApp::callDateTimeCallback(String strCallbackUrl, long lDateTime, const char* szData, int bCancel )
{
    strCallbackUrl = canonicalizeRhoUrl(strCallbackUrl);
    String strBody;
    if ( bCancel )
        strBody = "status=cancel&message=User canceled operation.";
    else
        strBody = "status=ok&result=" + convertToStringA(lDateTime);

    if ( szData && *szData )
    {
        strBody += "&opaque=";
        strBody += szData;
    }

    strBody += "&rho_callback=1";
    getNetRequest().pushData( strCallbackUrl, strBody, NULL );
}

void CRhodesApp::callBluetoothCallback(String strCallbackUrl, const char* body) {
	strCallbackUrl = canonicalizeRhoUrl(strCallbackUrl);
	String strBody = body;
	strBody += "&rho_callback=1";
	getNetRequest().pushData( strCallbackUrl, strBody, NULL );
}

void CRhodesApp::callPopupCallback(String strCallbackUrl, const String &id, const String &title)
{
    if ( strCallbackUrl.length() == 0 )
        return;

    strCallbackUrl = canonicalizeRhoUrl(strCallbackUrl);
    String strBody = "button_id=" + id + "&button_title=" + title;
    strBody += "&rho_callback=1";
    getNetRequest().pushData( strCallbackUrl, strBody, NULL );
}

static void callback_syncdb(void *arg, String const &/*query*/ )
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {
//    rho_sync_doSyncAllSources(1,"",false);
		rho::sync::RhoconnectClientManager::doSyncAllSources(1,"",false);
	}
    rho_http_sendresponse(arg, "");
}

static void callback_dosync(void *arg, String const &/*query*/ )
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {
//    rho_sync_doSyncAllSources(1,"",false);
		rho::sync::RhoconnectClientManager::doSyncAllSources(1,"",false);

	}
	rho_http_sendresponse(arg, "ok");
}

static void callback_dosync_source(void *arg, String const &strQuery )
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
	{
		size_t nPos = strQuery.find("srcName=");
		if ( nPos != String::npos )
		{
			String strSrcName = strQuery.substr(nPos+8);
			LOG(INFO) + "srcName = '" + strSrcName + "'";
//			rho_sync_doSyncSourceByName(strSrcName.c_str());
			rho::sync::RhoconnectClientManager::doSyncSourceByName(strSrcName.c_str());
		} else {
			LOG(WARNING) + "Unable to find 'srcName' parameter";
		}
	}

    rho_http_sendresponse(arg, "ok");
	 
}

static void callback_logger(void *arg, String const &query )
{
    int nLevel = 0;
    String strMsg, strCategory;

    CTokenizer oTokenizer(query, "&");
    while (oTokenizer.hasMoreTokens()) 
    {
	    String tok = oTokenizer.nextToken();
	    if (tok.length() == 0)
		    continue;

        if ( String_startsWith( tok, "level=") )
        {
            String strLevel = tok.substr(6);
            convertFromStringA( strLevel.c_str(), nLevel ); 
        }else if ( String_startsWith( tok, "msg=") )
        {
            strMsg = rho::net::URI::urlDecode(tok.substr(4));
        }else if ( String_startsWith( tok, "cat=") )
        {
            strCategory = rho::net::URI::urlDecode(tok.substr(4));
        }
    }

    rhoPlainLog( "", 0, nLevel, strCategory.c_str(), strMsg.c_str() );

    rho_http_sendresponse(arg, "");
}

static void callback_redirect_to(void *arg, String const &strQuery )
{
    size_t nUrl = strQuery.find("url=");
    String strUrl;
    if ( nUrl != String::npos )
        strUrl = strQuery.substr(nUrl+4);

    if ( strUrl.length() == 0 )
        strUrl = "/app/";
	
    rho_http_redirect(arg, (rho::net::URI::urlDecode(strUrl)).c_str());
}

static void callback_map(void *arg, String const &query )
{
    rho_map_location( const_cast<char*>(query.c_str()) );
    rho_http_sendresponse(arg, "");
}

static void callback_shared(void *arg, String const &/*query*/)
{
    rho_http_senderror(arg, 404, "Not Found");
}

static void callback_AppManager_load(void *arg, String const &query )
{
    rho_appmanager_load( arg, query.c_str() );
}

static void callback_getrhomessage(void *arg, String const &strQuery)
{
    String strMsg;
#ifndef RHO_NO_RUBY_API
    size_t nErrorPos = strQuery.find("error=");
    if ( nErrorPos != String::npos )
    {
        String strError = strQuery.substr(nErrorPos+6);
        int nError = atoi(strError.c_str());

        strMsg = rho_ruby_internal_getErrorText(nError);
    }else
    {
        size_t nMsgIdPos = strQuery.find("msgid=");
        if ( nMsgIdPos != String::npos )
        {
            String strName = strQuery.substr(nMsgIdPos+6);
            strMsg = rho_ruby_internal_getMessageText(strName.c_str());
        }
    }
#endif
    rho_http_sendresponse(arg, strMsg.c_str());
}

const String& CRhodesApp::getRhoMessage(int nError, const char* szName)
{
    String strUrl = m_strHomeUrl + "/system/getrhomessage?";
    if ( nError )
        strUrl += "error=" + convertToStringA(nError);
    else if ( szName && *szName )
    {
        strUrl += "msgid=";
        strUrl += szName;
    }

    if ( rho_ruby_is_started() )
    {
        NetResponse resp = getNetRequest().pullData( strUrl, NULL );
        if ( !resp.isOK() )
        {
            LOG(ERROR) + "getRhoMessage failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();
            m_strRhoMessage = "";
        }
        else
            m_strRhoMessage = resp.getCharData();
    }else
        m_strRhoMessage = "";

    return m_strRhoMessage;
}

static void callback_logged_in(void *arg, String const &strQuery)
{
    //rho_http_sendresponse(arg, rho_sync_logged_in() ? "true" : "false");
	bool haveclient = rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl();
	rho_http_sendresponse(arg, (haveclient && rho::sync::RhoconnectClientManager::logged_in()) ? "true" : "false");
}

static void callback_logout(void *arg, String const &strQuery)
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {
		rho::sync::RhoconnectClientManager::logout();
	//rho_sync_logout();
	}
    rho_http_sendresponse(arg, "ok");
}

static void callback_stop_sync(void *arg, String const &strQuery)
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {
		rho::sync::RhoconnectClientManager::stop();
	//rho_sync_stop();
	}
    rho_http_sendresponse(arg, "ok");
}

static void callback_set_pollinterval(void *arg, String const &strQuery)
{
	int nInterval = 0;

	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {


		size_t nPos = strQuery.find("interval=");
		if ( nPos != String::npos )
		{
			String strInterval = strQuery.substr(nPos+9);
			nInterval = atoi(strInterval.c_str());
		} else {
			LOG(WARNING) + "Unable to find 'interval' parameter";
		}
		//    nInterval = rho_sync_set_pollinterval(nInterval);
		nInterval = rho::sync::RhoconnectClientManager::set_pollinterval(nInterval);

	}
	rho_http_sendresponse(arg, convertToStringA(nInterval).c_str());
}

static void callback_get_pollinterval(void *arg, String const &strQuery)
{
	String interval = "";
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {
		interval = convertToStringA(rho::sync::RhoconnectClientManager::get_pollinterval());
	}

//	rho_http_sendresponse(arg, convertToStringA(rho_sync_get_pollinterval()).c_str());
	rho_http_sendresponse(arg, interval.c_str());
}

static void callback_set_syncserver(void *arg, String const &strQuery)
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {

		String strSyncserver = "";

		size_t nPos = strQuery.find("syncserver=");
		if ( nPos != String::npos )
		{
			strSyncserver = rho::net::URI::urlDecode(strQuery.substr(nPos+11));
		} else {
			LOG(WARNING) + "Unable to find 'syncserver' parameter";
		}
		//	rho_sync_set_syncserver(strSyncserver.c_str());
		rho::sync::RhoconnectClientManager::set_syncserver(strSyncserver.c_str());
	}
	rho_http_sendresponse(arg, "ok");
}

static void callback_set_pagesize(void *arg, String const &strQuery)
{
    int nSize = 0;
	int nOldSize = 0;
	
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {

		size_t nPos = strQuery.find("pagesize=");
		if ( nPos != String::npos )
		{
			String strSize = strQuery.substr(nPos+9);
			nSize = atoi(strSize.c_str());
		} else {
			LOG(WARNING) + "Unable to find 'pagesize' parameter";
		}
		
		nOldSize = rho::sync::RhoconnectClientManager::get_pagesize();
		rho::sync::RhoconnectClientManager::set_pagesize(nSize);
		
		//    int nOldSize = rho_sync_get_pagesize();
		//    rho_sync_set_pagesize(nSize);

	}

	rho_http_sendresponse(arg, convertToStringA(nOldSize).c_str());
}

static void callback_get_pagesize(void *arg, String const &strQuery)
{
	int size = 0;
	
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {
		size = rho::sync::RhoconnectClientManager::get_pagesize();
	}
//	rho_http_sendresponse(arg, convertToStringA(rho_sync_get_pagesize()).c_str());
	rho_http_sendresponse(arg, convertToStringA(size).c_str());
}

static void callback_get_lastsync_objectcount(void *arg, String const &strQuery)
{
    int nSrcId = -1;
    int nCount = -1;
	
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {

		size_t nPos = strQuery.find("srcName=");
		if ( nPos != String::npos )
		{
			String strSrcId = strQuery.substr(nPos+8);
			LOG(INFO) + "srcName = '" + strSrcId + "'";
			nSrcId = atoi(strSrcId.c_str());
			//        nCount = rho_sync_get_lastsync_objectcount(nSrcId);
			nCount = rho::sync::RhoconnectClientManager::get_lastsync_objectcount(nSrcId);
		} else {
			LOG(WARNING) + "Unable to find 'srcName' parameter";
		}
	}
	rho_http_sendresponse(arg, convertToStringA(nCount).c_str());
}

static void callback_is_syncing(void *arg, String const &strQuery)
{
	bool haveclient = rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl();
 //   rho_http_sendresponse(arg, rho_sync_issyncing() ? "true" : "false");
	rho_http_sendresponse(arg, ( haveclient && rho::sync::RhoconnectClientManager::issyncing() ) ? "true" : "false");
}

static void callback_enable_status_popup(void *arg, String const &strQuery)
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {

		size_t nPos = strQuery.find("enable=");
		if ( nPos != String::npos )
		{
			String strEnable = strQuery.substr(nPos+7);
			//        rho_sync_enable_status_popup(strEnable == "true" ? 2 : 0);
			rho::sync::RhoconnectClientManager::enable_status_popup(strEnable == "true" ? 2 : 0);
		} else {
			LOG(WARNING) + "Unable to find 'enable' parameter";
		}		
	}
	rho_http_sendresponse(arg, "ok");
}

static void callback_set_threaded_mode(void *arg, String const &strQuery)
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {

		size_t nPos = strQuery.find("threaded=");
		if ( nPos != String::npos )
		{
			String strThreaded = strQuery.substr(nPos+9);
			//        rho_sync_set_threaded_mode(strThreaded == "true" ? 2 : 0);
			rho::sync::RhoconnectClientManager::set_threaded_mode(strThreaded == "true" ? 2 : 0);
		} else {
			LOG(WARNING) + "Unable to find 'threaded' parameter";
		}
	}
	rho_http_sendresponse(arg, "ok");
}

static void callback_register_push(void *arg, String const &strQuery)
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {
		//	rho_sync_register_push();
		rho::sync::RhoconnectClientManager::register_push();

	}
    rho_http_sendresponse(arg,  "ok");
}

static void callback_set_source_property(void *arg, String const &strQuery)
{
	//TODO: stub (DmitryP)
    rho_http_sendresponse(arg,  "ok");
}

static void callback_set_ssl_verify_peer(void *arg, String const &strQuery)
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {

		size_t nPos = strQuery.find("verify=");
		if ( nPos != String::npos )
		{
			String strVerify = strQuery.substr(nPos+7);
			//        rho_sync_set_ssl_verify_peer(strVerify == "true" ? 2 : 0);
			rho::sync::RhoconnectClientManager::set_ssl_verify_peer(strVerify == "true" ? 2 : 0);
		} else {
			LOG(WARNING) + "Unable to find 'verify' parameter";
		}
	}
	rho_http_sendresponse(arg, "ok");
}

static void callback_update_blob_attribs(void *arg, String const &strQuery)
{
	//TODO: stub (DmitryP)
    rho_http_sendresponse(arg,  "ok");
}

static void callback_set_objectnotify_url(void *arg, String const &strQuery)
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {

		size_t nPos = strQuery.find("url=");
		if ( nPos != String::npos )
		{
			String strUrl = strQuery.substr(nPos+4);
			//        rho_sync_setobjectnotify_url(strUrl.c_str());
			rho::sync::RhoconnectClientManager::setobjectnotify_url(strUrl.c_str());
		} else {
			LOG(WARNING) + "Unable to find 'url' parameter";
		}
		
	}
	
	rho_http_sendresponse(arg, "ok");
}

static void callback_add_objectnotify(void *arg, String const &strQuery)
{
	//TODO: stub (DmitryP)
    rho_http_sendresponse(arg,  "ok");
}

static void callback_clean_objectnotify(void *arg, String const &strQuery)
{
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {
		//	rho_sync_cleanobjectnotify();
		rho::sync::RhoconnectClientManager::cleanobjectnotify();
	}
    rho_http_sendresponse(arg,  "ok");
}

static void callback_set_notification(void *arg, String const &strQuery)
{
	//TODO: stub (DmitryP)
    rho_http_sendresponse(arg,  "ok");
}

static void callback_clear_notification(void *arg, String const &strQuery)
{
    int nSrcId = -1;
	
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {

		size_t nPos = strQuery.find("srcName=");
		if ( nPos != String::npos )
		{
			String strSrcId = strQuery.substr(nPos+8);
			LOG(INFO) + "srcName = '" + strSrcId + "'";
			nSrcId = atoi(strSrcId.c_str());
			//        rho_sync_clear_notification(nSrcId);
			rho::sync::RhoconnectClientManager::clear_notification(nSrcId);
		} else {
			LOG(WARNING) + "Unable to find 'srcName' parameter";
		}
		
	}
	
    rho_http_sendresponse(arg, "ok");
}

static void callback_login(void *arg, String const &strQuery)
{
    int nLevel = 0;
    String strLogin, strPassword, strCallback;
	
	if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {

		CTokenizer oTokenizer(strQuery, "&");
		while (oTokenizer.hasMoreTokens())
		{
			String tok = oTokenizer.nextToken();
			if (tok.length() == 0)
				continue;

			if ( String_startsWith(tok, "login=") )
			{
				strLogin = tok.substr(6);
			}else if ( String_startsWith( tok, "password=") )
			{
				strPassword = tok.substr(9);
			}else if ( String_startsWith( tok, "callback=") )
			{
				strCallback = rho::net::URI::urlDecode(tok.substr(9));
			}
		}

//    rho_sync_login(strLogin.c_str(), strPassword.c_str(), strCallback.c_str());
		rho::sync::RhoconnectClientManager::login(strLogin.c_str(), strPassword.c_str(), strCallback.c_str());
	}
	
    rho_http_sendresponse(arg,  "ok");
}

static void callback_dosearch(void *arg, String const &strQuery)
{
	//TODO: stub (DmitryP)
    rho_http_sendresponse(arg,  "ok");
}

static void callback_get_src_attrs(void *arg, String const &strQuery)
{
	//TODO: stub (DmitryP)
    rho_http_sendresponse(arg,  "ok");
}

static void callback_is_blob_attr(void *arg, String const &strQuery)
{
	//TODO: stub (DmitryP)
    rho_http_sendresponse(arg,  "ok");
}

void CRhodesApp::registerLocalServerUrl(const String& strUrl, rho::net::CHttpServer::callback_t const &callback)
{
    if ( m_httpServer )
        m_httpServer->register_uri(strUrl.c_str(), callback);    
}

void CRhodesApp::initHttpServer()
{
    String strAppRootPath = getRhoRootPath();
    String strAppUserPath = getRhoUserPath();
    String strRuntimePath = getRhoRuntimePath();
#ifndef RHODES_EMULATOR
    strAppRootPath += "apps";
#endif

    m_httpServer = new net::CHttpServer(atoi(getFreeListeningPort()), strAppRootPath, strAppUserPath, strRuntimePath);
    m_httpServer->register_uri("/system/geolocation", rubyext::CGeoLocation::callback_geolocation);
    m_httpServer->register_uri("/system/syncdb", callback_syncdb);
    m_httpServer->register_uri("/system/redirect_to", callback_redirect_to);
    m_httpServer->register_uri("/system/map", callback_map);
    m_httpServer->register_uri("/system/shared", callback_shared);

#ifndef RHO_NO_JS_API
    m_httpServer->register_uri( "/system/js_api_entrypoint", rho::apiGenerator::rho_http_js_entry_point );
#endif

    m_httpServer->register_uri("/system/call_ruby_proc_callback", rho::net::rho_http_ruby_proc_callback );

    m_httpServer->register_uri("/AppManager/loader/load", callback_AppManager_load);
    m_httpServer->register_uri("/system/getrhomessage", callback_getrhomessage);
    m_httpServer->register_uri("/system/activateapp", callback_activateapp);
    m_httpServer->register_uri("/system/deactivateapp", callback_deactivateapp);
    m_httpServer->register_uri("/system/uicreated", callback_uicreated);
    m_httpServer->register_uri("/system/uicloserequest", callback_uicloserequest);
    m_httpServer->register_uri("/system/uidestroyed", callback_uidestroyed);
    m_httpServer->register_uri("/system/loadserversources", callback_loadserversources);
    m_httpServer->register_uri("/system/resetDBOnSyncUserChanged", callback_resetDBOnSyncUserChanged);
    m_httpServer->register_uri("/system/loadallsyncsources", callback_loadallsyncsources);
    m_httpServer->register_uri("/system/logger", callback_logger);

    m_httpServer->register_uri("/system/syncengine/logged_in", callback_logged_in);
    m_httpServer->register_uri("/system/syncengine/logout", callback_logout);
    m_httpServer->register_uri("/system/syncengine/dosync", callback_dosync);
    m_httpServer->register_uri("/system/syncengine/stop_sync", callback_stop_sync);
    m_httpServer->register_uri("/system/syncengine/set_pollinterval", callback_set_pollinterval);
    m_httpServer->register_uri("/system/syncengine/get_pollinterval", callback_get_pollinterval);
    m_httpServer->register_uri("/system/syncengine/set_syncserver", callback_set_syncserver);
    m_httpServer->register_uri("/system/syncengine/set_pagesize", callback_set_pagesize);
    m_httpServer->register_uri("/system/syncengine/get_pagesize", callback_get_pagesize);
    m_httpServer->register_uri("/system/syncengine/get_lastsync_objectcount", callback_get_lastsync_objectcount);
    m_httpServer->register_uri("/system/syncengine/is_syncing", callback_is_syncing);
    m_httpServer->register_uri("/system/syncengine/dosync_source", callback_dosync_source);
    m_httpServer->register_uri("/system/syncengine/enable_status_popup", callback_enable_status_popup);
    m_httpServer->register_uri("/system/syncengine/set_threaded_mode", callback_set_threaded_mode);
    m_httpServer->register_uri("/system/syncengine/register_push", callback_register_push);

    m_httpServer->register_uri("/system/syncengine/set_source_property", callback_set_source_property);
    m_httpServer->register_uri("/system/syncengine/set_ssl_verify_peer", callback_set_ssl_verify_peer);
    m_httpServer->register_uri("/system/syncengine/update_blob_attribs", callback_update_blob_attribs);
    m_httpServer->register_uri("/system/syncengine/set_objectnotify_url", callback_set_objectnotify_url);
    m_httpServer->register_uri("/system/syncengine/add_objectnotify", callback_add_objectnotify);
    m_httpServer->register_uri("/system/syncengine/clean_objectnotify", callback_clean_objectnotify);
    m_httpServer->register_uri("/system/syncengine/set_notification", callback_set_notification);
    m_httpServer->register_uri("/system/syncengine/clear_notification", callback_clear_notification);
    m_httpServer->register_uri("/system/syncengine/login", callback_login);
    m_httpServer->register_uri("/system/syncengine/dosearch", callback_dosearch);
    m_httpServer->register_uri("/system/syncengine/get_src_attrs", callback_get_src_attrs);
    m_httpServer->register_uri("/system/syncengine/is_blob_attr", callback_is_blob_attr);


}

const char* CRhodesApp::getFreeListeningPort()
{
  if ( m_isJSFSApp )
    return "";

	if ( m_strListeningPorts.length() > 0 )
		return m_strListeningPorts.c_str();

    int nFreePort = determineFreeListeningPort();
    m_strListeningPorts = convertToStringA(nFreePort);

	LOG(INFO) + "Free listening port: " + m_strListeningPorts;

    return m_strListeningPorts.c_str();
}
    
const char* CRhodesApp::getNodeJSListeningPort() {
    if ( m_strNodeJSListeningPorts.length() > 0 )
        return m_strNodeJSListeningPorts.c_str();
    
    int port = rho_conf_getInt("nodejs_local_server_port");
    if (port == 0) {
        port = determineFreeListeningPort();
    }
    if (port < 0)
        port = 0;
    if (port > 65535)
        port = 0;
    
    m_strNodeJSListeningPorts = convertToStringA(port);
    m_nNodeJSListeningPorts = port;
    
    LOG(INFO) + "Free Node.js listening port: " + m_strNodeJSListeningPorts;
    
    return m_strNodeJSListeningPorts.c_str();
}

int CRhodesApp::determineFreeListeningPort()
{
    int sockfd = -1;
    sockaddr_in serv_addr = sockaddr_in();
    int nFreePort = 0, noerrors = 1;

	LOG(INFO) + "Trying to get free listening port.";
	
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( sockfd < 0 )
    {
        LOG(ERROR) + "Unable to open socket";
        noerrors = 0;
    }
    
    int optval = 1;
    if (noerrors && setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval)) != 0)
    {
        LOG(ERROR) + "Unable to set socket option";
        noerrors = 0;
    }
  
#if defined(OS_MACOSX)
    if (noerrors && setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (const char *)&optval, sizeof(optval)) != 0)
    {
        LOG(ERROR) + "Unable to set socket option";
        noerrors = 0;
    }
#endif

    if (noerrors)
    {
        int listenPort = rho_conf_getInt("local_server_port");
        if (listenPort < 0)
            listenPort = 0;
        if (listenPort > 65535)
            listenPort = 0;
        memset((void *) &serv_addr, 0, sizeof(serv_addr));
#if defined(OS_MACOSX)
        serv_addr.sin_len = sizeof(serv_addr);
#endif
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        serv_addr.sin_port = htons((short)listenPort);
        
        LOG(INFO) + "Trying to bind of " + listenPort + " port...";

        int nBindRes = bind( sockfd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) );
        if ( nBindRes != 0 )
        {
            LOG(INFO) + "Bind of " + listenPort + " port is failed with code: " + nBindRes;
            if (listenPort != 0)
            {
                // Fill serv_addr again but with dynamically selected port
#if defined(OS_MACOSX)
                serv_addr.sin_len = sizeof(serv_addr);
#endif
                serv_addr.sin_family = AF_INET;
                serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
                serv_addr.sin_port = htons(0);

                LOG(INFO) + "Trying to bind on dynamic port...";
            
                if ( bind( sockfd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) != 0 )
                {
                    LOG(WARNING) + "Unable to bind";
                    noerrors = 0;
                }
            }
            else
            {
                LOG(WARNING) + "Unable to bind";
                noerrors = 0;
            }
        }
    }

    if ( noerrors )
    {
        char buf[10] = {0};
        
        socklen_t length = sizeof( serv_addr );
        
        if (getsockname( sockfd, (struct sockaddr *)&serv_addr, &length ) != 0)
        {
            LOG(ERROR) + "Can not get socket info";
            nFreePort = 0;
        }
        else
        {
            nFreePort = (int)ntohs(serv_addr.sin_port);
            LOG(INFO) + "Got port to bind on: " + nFreePort;
        }
    }
    
    //Clean up
#if defined(OS_ANDROID)
    close(sockfd);
#else
    closesocket(sockfd);
#endif
	
	return nFreePort;
}

#ifdef OS_WINCE
bool CRhodesApp::isWebkitOutofProcess()
{
    const char* szOutprocess = get_app_build_config_item("webkit_outprocess");
    return szOutprocess && strcmp(szOutprocess, "1") == 0;
}
#endif

void CRhodesApp::initAppUrls() 
{
    CRhodesAppBase::initAppUrls(); 

    m_isJSFSApp = false;

#ifdef RHO_NO_RUBY_API
        m_isJSFSApp = String_startsWith(getStartUrl(), "file:") ? true : false;
#endif

#ifdef OS_WINCE
    if (isWebkitOutofProcess())
        m_isJSFSApp = false; //We need local server for out of process webkit, it use sockets to call common API
#endif

    boolean force_https = false;
#ifdef OS_MACOSX
    if (rho_conf_is_property_exists("ios_https_local_server")!=0) {
        force_https = rho_conf_getBool("ios_https_local_server")!=0;
    }
#endif

#ifdef OS_ANDROID
       const char* value = get_app_build_config_item("local_https_server_with_client_checking");
       force_https = value && strcmp(value, "1") == 0;
#endif

    if (force_https) {
        m_strHomeUrl = "mohus://127.0.0.1:";
    }
    else {
        m_strHomeUrl = "http";
        m_strHomeUrl = m_strHomeUrl + "://127.0.0.1:";
    }
    
    m_strRubyServerHomeURL = m_strHomeUrl+getFreeListeningPort();
    m_strNodeServerHomeURL = m_strHomeUrl+getNodeJSListeningPort();

    if (isNodeJSApplication() || isRubyNodeJSApplication()) {
        m_strHomeUrl += getNodeJSListeningPort();
    }
    else {
        m_strHomeUrl += getFreeListeningPort();
    }

#ifndef RHODES_EMULATOR
    #ifndef OS_SAILFISH
    m_strLoadingPagePath = "file://" + getRhoRootPath() + "apps/app/loading.html";
    #else
    m_strLoadingPagePath = getRhoRootPath() + "apps/app/loading.html";
    #endif
	m_strLoadingPngPath = getRhoRootPath() + "apps/app/loading.png";
#else
    m_strLoadingPagePath = "file://" + getRhoRootPath() + "app/loading.html";
	m_strLoadingPngPath = getRhoRootPath() + "app/loading.png";
#endif

    //write local server url to file
#ifdef OS_WINCE
    if (!m_isJSFSApp)
    {
        /*String strLSPath = CFilePath::join(m_strRuntimePath.substr(0, m_strRuntimePath.length()-4), "RhoLocalserver.txt"); //remove rho/
        CRhoFile::writeStringToFile( strLSPath.c_str(), m_strHomeUrl.substr(7, m_strHomeUrl.length()));*/
        modifyRhoApiFile();
    }
#endif
}

void CRhodesApp::modifyRhoApiFile()
{
    const int bufferLenght = 10240;

    std::vector<String> apiFilePathV;
    apiFilePathV.push_back(m_strRuntimePath);
    apiFilePathV.push_back("apps");
    apiFilePathV.push_back("public");
    apiFilePathV.push_back("api");
    apiFilePathV.push_back("rhoapi-modules.js");
    
    String apiFilePath = CFilePath::join(apiFilePathV);

    std::vector<String> apiFilePathTmpV;
    apiFilePathTmpV.push_back(m_strRuntimePath);
    apiFilePathTmpV.push_back("apps");
    apiFilePathTmpV.push_back("public");
    apiFilePathTmpV.push_back("api");
    apiFilePathTmpV.push_back("rhoapi-modules-tmp.js");

    String apiFilePathTmp = CFilePath::join(apiFilePathTmpV);

    if (common::CRhoFile::isFileExist(apiFilePath.c_str()))
    {
        common::CRhoFile apiFile, tmpFile;

        if (apiFile.open(apiFilePath.c_str(), common::CRhoFile::OpenReadOnly) && 
            tmpFile.open(apiFilePathTmp.c_str(), common::CRhoFile::OpenForWrite))
        {
            common::InputStream* is = apiFile.getInputStream();

            char buffer[bufferLenght];
            buffer[0] = '\0';

            while (is->available())
            {
                int realLen = is->read(buffer, 0, bufferLenght);

                if (realLen == -1)
                    break;

                buffer[realLen] = 0;

                String bufferStr(buffer);

                String::size_type posStart = bufferStr.find("RHO_AJAX");
                String::size_type posEnd   = bufferStr.find("PORT_NUMBER");

                if (posStart != String::npos && posEnd != String::npos)
                {
                    String::size_type replaceLen = posEnd - posStart + 13;

                    String portStr = "\'RHO_AJAX-->";
                    portStr += getFreeListeningPort();
                    portStr += "<--PORT_NUMBER\'";
                    bufferStr.replace(posStart - 1, replaceLen, portStr.c_str());
                }

                tmpFile.write(bufferStr.c_str(), bufferStr.size());
            }

            apiFile.close();
            tmpFile.close();

            common::CRhoFile::deleteFile(apiFilePath.c_str());
            common::CRhoFile::renameFile(apiFilePathTmp.c_str(), apiFilePath.c_str());
        }
    }
}

void CRhodesApp::keepLastVisitedUrl(String strUrl)
{
    //LOG(INFO) + "Current URL: " + strUrl;
    int nIndex = rho_webview_active_tab();
    if (nIndex < 0) nIndex = 0;
    int nToAdd = nIndex - m_currentUrls.size();
    for ( int i = 0; i <= nToAdd; i++ )
    {
        m_currentUrls.addElement("");
    }

    m_currentUrls[nIndex] = canonicalizeRhoUrl(strUrl);
}

const String& CRhodesApp::getCurrentUrl(int index)
{ 
    if (index < 0) index = rho_webview_active_tab();
    if (index < 0) index = 0;
    if ( index < static_cast<int>(m_currentUrls.size()) )
        return m_currentUrls[index]; 

    return m_EmptyString;
}

const String& CRhodesApp::getAppBackUrl()
{
    int index = rho_webview_active_tab();
    if (index < 0)
        index = 0;
    if ( index < static_cast<int>(m_arAppBackUrl.size()) )
        return m_arAppBackUrl[index]; 

    return m_EmptyString;
}

void CRhodesApp::setAppBackUrl(const String& url)
{
    int nIndex = rho_webview_active_tab();
    if (nIndex < 0)
        nIndex = 0;
    int nToAdd = nIndex - m_arAppBackUrl.size();
    for ( int i = 0; i <= nToAdd; i++ )
    {
        m_arAppBackUrl.addElement("");
        m_arAppBackUrlOrig.addElement("");
    }

    if ( url.length() > 0 )
    {
        m_arAppBackUrlOrig[nIndex] = url;
        m_arAppBackUrl[nIndex] = canonicalizeRhoUrl(url);
    }
    else
    {
        m_arAppBackUrlOrig[nIndex] = "";
        m_arAppBackUrl[nIndex] = "";
    }
}

void CRhodesApp::navigateBack()
{
    int nIndex = rho_webview_active_tab();
    if (nIndex < 0)
        nIndex = 0;

    if((nIndex < static_cast<int>(m_arAppBackUrlOrig.size()))
        && (m_arAppBackUrlOrig[nIndex].length() > 0))
    {
        String backUrl = m_arAppBackUrlOrig[nIndex];
        setAppBackUrl("");
        loadUrl(backUrl);
    }
    else// if(strcasecmp(getCurrentUrl(nIndex).c_str(),getStartUrl().c_str()) != 0)
    {
        rho_webview_navigate_back_with_tab(nIndex);
    }
}

String CRhodesApp::getAppName()
{
    String strAppName;

#if (defined(OS_WINDOWS_DESKTOP) || defined(OS_WINCE)) && !defined(RHODES_EMULATOR) && !defined(RHODES_QT_PLATFORM)
    strAppName = rho_native_get_appname();

#elif(defined(RHODES_EMULATOR))
        strAppName = RHOSIMCONF().getString("app_name");

#else
    //TODO: Android - get app name for shared runtime app
    strAppName = RHOCONF().getString("app_name");
#endif

    return strAppName;
}

StringW CRhodesApp::getAppNameW()
{
    return convertToStringW( RHODESAPP().getAppName() );
}

String CRhodesApp::getAppTitle()
{
    String strTitle = RHOCONF().getString("title_text");
    if ( strTitle.length() == 0 )
        strTitle = getAppName();

    return strTitle;
}

const String& CRhodesApp::getStartUrl()
{
    m_strStartUrl = canonicalizeRhoUrl( RHOCONF().getString("start_path") );
    return m_strStartUrl;
}

boolean CRhodesApp::isOnStartPage()
{
    String strStart = getStartUrl();
    String strCurUrl = getCurrentUrl(0);

    if ( strStart.compare(strCurUrl) == 0 )
        return true;

    //check for index
    int nIndexLen = CHttpServer::isIndex(strStart);
    if ( nIndexLen > 0 && String_startsWith(strStart, strCurUrl) )
    {
        return strncmp(strStart.c_str(), strCurUrl.c_str(), strStart.length() - nIndexLen - 1) == 0;
    }

    nIndexLen = CHttpServer::isIndex(strCurUrl);
    if ( nIndexLen > 0 && String_startsWith(strCurUrl, strStart) )
    {
        return strncmp(strCurUrl.c_str(), strStart.c_str(), strCurUrl.length() - nIndexLen - 1) == 0;
    }

    return false;
}

const String& CRhodesApp::getBaseUrl()
{
    return m_strHomeUrl;
}

void CRhodesApp::setBaseUrl(const String& strBaseUrl)
{
    m_strHomeUrl = strBaseUrl + getFreeListeningPort();
}

const String& CRhodesApp::getOptionsUrl()
{
    m_strOptionsUrl = canonicalizeRhoUrl( RHOCONF().getString("options_path") );
    return m_strOptionsUrl;
}

const String& CRhodesApp::getRhobundleReloadUrl() 
{
    m_strRhobundleReloadUrl = RHOCONF().getString("rhobundle_zip_url");
    return m_strRhobundleReloadUrl;
}

void CRhodesApp::navigateToUrl( const String& strUrl)
{
    rho_webview_navigate(strUrl.c_str(), -1);
}

String CRhodesApp::addCallbackObject(ICallbackObject* pCallbackObject, String strName)
{
    synchronized(m_mxCallbackObjects)
    {
        int nIndex = -1;
        for (int i = 0; i < (int)m_arCallbackObjects.size(); i++)
        {
            if ( m_arCallbackObjects.elementAt(i) == 0 )
                nIndex = i;
        }
        if ( nIndex  == -1 )
        {
            m_arCallbackObjects.addElement(pCallbackObject);
            nIndex = m_arCallbackObjects.size()-1;
        }else
            m_arCallbackObjects.setElementAt(pCallbackObject,nIndex);

        String strRes = "__rho_object[" + strName + "]=" + convertToStringA(nIndex);

        return strRes;
    }
}

unsigned long CRhodesApp::getCallbackObject(int nIndex)
{
#ifndef RHO_NO_RUBY_API
    synchronized(m_mxCallbackObjects)
    {
        if ( nIndex < 0 || nIndex > (int)m_arCallbackObjects.size() )
            return rho_ruby_get_NIL();

        ICallbackObject* pCallbackObject = m_arCallbackObjects.elementAt(nIndex);
        m_arCallbackObjects.setElementAt(0,nIndex);

        if ( !pCallbackObject )
            return rho_ruby_get_NIL();

        unsigned long valRes = pCallbackObject->getObjectValue();

        delete pCallbackObject;

        return valRes;
    }
#else
    return 6; //Ruby Qundef
#endif
}

void CRhodesApp::initPushClients()
{
    static bool first = true;
    if(first)
    {
        first = false;
        m_appPushMgr.initClients();
    }
}

void CRhodesApp::setPushNotification(const String& strUrl, const String& strParams, const String& strType )
{
    if(strType == "legacy" || strType == "default")
    {
        LOG(TRACE) + "Set notification for legacy push: " + strUrl;
        synchronized(m_mxPushCallback)
        {
            m_strPushCallback = strUrl;
            if (m_strPushCallback.length())
                m_strPushCallback = canonicalizeRhoUrl(m_strPushCallback);

            m_strPushCallbackParams = strParams;
        }
    }

    if(strType != "legacy")
    {
        LOG(TRACE) + "Set Push Manager notification for " + strType + " push: " + strUrl;

        String canonicalUrl;
        if (strUrl.length())
            canonicalUrl = canonicalizeRhoUrl(strUrl);

        if(strType != "default")
            m_appPushMgr.setNotificationUrl(strType, canonicalUrl, strParams);
        else
            m_appPushMgr.setNotificationUrl(canonicalUrl, strParams);
    }
}


boolean CRhodesApp::callPushCallbackWithJsonBody(const String& strUrl, const String& strJson, const String& strParams)
{
    synchronized(m_mxPushCallback)
    {
        if (strUrl.length() == 0)
            return false;

        String strCanonicalUrl = canonicalizeRhoUrl(strUrl);

        String strBody = strJson.length() ? (addCallbackObject( new CJsonResponse( strJson ), "__rho_inline" ) + "&rho_callback=1") : strJson;
        if (strParams.length() > 0)
        {
            if (strBody.length())
            {
                strBody += "&";
            }
            strBody += strParams;
        }

        NetResponse resp = getNetRequest().pushData( strCanonicalUrl, strBody, NULL );
        if (!resp.isOK())
            LOG(ERROR) + "Push notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();
        else
        {
            const char* szData = resp.getCharData();
            LOG(TRACE) + "Push callback resp data: " + (szData ? szData : "NULL");
            return !(szData && strcmp(szData,"rho_push") == 0);
        }
    }

    return false;
}

boolean CRhodesApp::callPushCallback(const String& strType, const String& strJson, const String& strData)
{
    return m_appPushMgr.callNotification(strType, strJson, strData);
}

void CRhodesApp::setScreenRotationNotification(String strUrl, String strParams)
{
    synchronized(m_mxScreenRotationCallback)
    {
        if (strUrl.length() > 0) {
            m_strScreenRotationCallback = canonicalizeRhoUrl(strUrl);
        }
        else {
            m_strScreenRotationCallback = "";
        }
        m_strScreenRotationCallbackParams = strParams;
    }
}

void CRhodesApp::callScreenRotationCallback(int width, int height, int degrees)
{
    if ( !rho_ruby_is_started() )
        return;

	synchronized(m_mxScreenRotationCallback) 
	{
		if (m_strScreenRotationCallback.length() == 0)
			return;
		
		String strBody = "rho_callback=1";
		
        strBody += "&width=";   strBody += convertToStringA(width);
		strBody += "&height=";  strBody += convertToStringA(height);
		strBody += "&degrees="; strBody += convertToStringA(degrees);
		
        if ( m_strScreenRotationCallbackParams.length() > 0 )
            strBody += "&" + m_strPushCallbackParams;
		
		String i = "calling Screen rotation notification: ";
		i += m_strScreenRotationCallback;
		RAWLOG_ERROR(i.c_str());

			
		NetResponse resp = getNetRequest().pushData( m_strScreenRotationCallback, strBody, NULL);
        if (!resp.isOK()) {
            LOG(ERROR) + "Screen rotation notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();
        }
    }
}

void CRhodesApp::callScreenOnCallbackAsync()
{
  m_appCallbacksQueue->addQueueCommand(new CAppCallbacksQueue::Command(CAppCallbacksQueue::screen_on));
}

void CRhodesApp::loadUrl(String url, int nTabIndex/* = -1*/)
{
    if ( url.length() == 0 )
        return;

    boolean callback = false, js_callback = false;
    if (String_startsWith(url, "callback:") )
    {
        callback = true;
        url = url.substr(9);
    }else if (String_startsWith(url, "javascript:") )
    {
        js_callback = true;
        url = url.substr(11);
    }else if (String_startsWith(url, "__rhoCallback:") )
    {
        js_callback = true;
        url = url.substr(14);

        String strCallback("Rho.callbackHandler( \"");
        strCallback += url;
        strCallback += "\", {},\"\")";

        url = strCallback;
    }else if ( strcasecmp(url.c_str(), "exit")==0 || strcasecmp(url.c_str(), "close") == 0 )
    {
        rho_sys_app_exit();
        return;
    }else if ( strcasecmp(url.c_str(), "options")==0 )
    {
        rho_webview_navigate(getOptionsUrl().c_str(), -1);
        return;
    }else if ( strcasecmp(url.c_str(), "home")==0 )
    {
        rho_webview_navigate(getStartUrl().c_str(), -1);
        return;
    }else if ( strcasecmp(url.c_str(), "refresh")==0 )
    {
        rho_webview_refresh(nTabIndex);
        return;
    }else if ( strcasecmp(url.c_str(), "back")==0 )
    {
        navigateBack();
        return;
    }else if ( strcasecmp(url.c_str(), "log")==0 )
    {
        rho_conf_show_log();
        return;
    }else if ( strcasecmp(url.c_str(), "fullscreen")==0 )
    {
        rho_webview_full_screen_mode(1);
        return;
    }else if ( strcasecmp(url.c_str(), "sync")==0 )
    {
		if ( rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl() ) {
			rho::sync::RhoconnectClientManager::doSyncAllSources(1,"",false);
			
		}

        return;
    }else if ( strcasecmp(url.c_str(), "minimize")==0 )
    {
        getExtManager().minimizeApp();
        return;
    }else if ( strcasecmp(url.c_str(), "SIP")==0 )
    {
#ifdef OS_WINCE
        RHODESAPP().getExtManager().OnSIPState(true);
#endif
        return;
    }

    if (callback)
    {
        url = canonicalizeRhoUrl(url);
        if ( rho_ruby_is_started() )
            getNetRequest().pushData( url,  "rho_callback=1", NULL );
    }
    else if (js_callback)
        rho_webview_execute_js(url.c_str(), nTabIndex);
    else
    {
        url = canonicalizeRhoUrl(url);
        rho_webview_navigate(url.c_str(), nTabIndex);
    }
}

void CRhodesApp::notifyLocalServerStarted()
{
    m_appCallbacksQueue->addQueueCommand(new CAppCallbacksQueue::Command(CAppCallbacksQueue::local_server_started));
}
	
extern "C" int  rho_ruby_is_started();
extern "C" unsigned long rho_ruby_safe_require(const char *fname);

void CExtManager::requireRubyFile( const char* szFilePath )
{
#ifndef RHO_NO_RUBY_API	
    if( rho_ruby_is_started() )
    {
        RAWTRACEC1("CExtManager", "Require ruby file: %s", szFilePath);
        unsigned long val = rho_ruby_safe_require(szFilePath);
        if ( rho_ruby_is_NIL(val) )
            RAWLOGC_WARNING1("CExtManager", "requireRubyFile cannot find file: %s", szFilePath);
    }
#endif
}
	
void CRhodesApp::setNetworkStatusNotify(const apiGenerator::CMethodResult& oResult, int poll_interval)
{
	synchronized(m_mxNetworkStatus)
	{
		m_networkStatusReceiver.setMethodResult(oResult);
		if ( m_pNetworkStatusMonitor != 0 )
		{
			if ( poll_interval <= 0 ) {
				poll_interval = c_defaultNetworkStatusPollInterval;
			}
			m_pNetworkStatusMonitor->setPollInterval(poll_interval);
		}
	}
}

void CRhodesApp::clearNetworkStatusNotify()
{
	synchronized(m_mxNetworkStatus)
	{
		m_networkStatusReceiver.setMethodResult(apiGenerator::CMethodResult());
	}
}

void CRhodesApp::setNetworkStatusMonitor( INetworkStatusMonitor* netMonitor )
{
	synchronized(m_mxNetworkStatus)
	{
		m_pNetworkStatusMonitor = netMonitor;
		if ( m_pNetworkStatusMonitor != 0)
		{
			m_pNetworkStatusMonitor->setNetworkStatusReceiver(&m_networkStatusReceiver);
			m_pNetworkStatusMonitor->setPollInterval( c_defaultNetworkStatusPollInterval );
		}
	}
}
	
	
	NetworkStatusReceiver::NetworkStatusReceiver( common::CMutex& mxAccess ) :
		m_prevStatus( networkStatusUnknown ),
		m_mxAccess(mxAccess)
	{
	}
    
    void CRhodesApp::setApplicationEventHandler(apiGenerator::CMethodResult& oResult)
    {
        m_applicationEventReceiver.setCallback(oResult);
    }
    
    void CRhodesApp::clearApplicationEventHandler()
    {
        apiGenerator::CMethodResult tmp;
        m_applicationEventReceiver.setCallback(tmp);
    }
    
    IApplicationEventReceiver* CRhodesApp::getApplicationEventReceiver()
    {
        return &m_applicationEventReceiver;
    }
	
	
	void NetworkStatusReceiver::onNetworkStatusChanged( enNetworkStatus currentStatus )
	{
		synchronized(m_mxAccess)
		{
			if ( m_prevStatus != currentStatus )
			{
                String sCurrentStatus = networkStatusToString(currentStatus);
                String sPrevStatus = networkStatusToString(m_prevStatus);
                
                Hashtable<String, String> ret;
                
                ret.put("current_status", sCurrentStatus);
                ret.put("prev_status", sPrevStatus);
                
                m_result.set(ret);
			}
			m_prevStatus = currentStatus;
		}
	}
	
	rho::String NetworkStatusReceiver::networkStatusToString( enNetworkStatus status )
	{
		switch (status) 
		{
			case networkStatusConnected:
				return "connected";
				break;
			case networkStatusDisconnected:
				return "disconnected";
				break;
			case networkStatusUnknown:
				return "unknown";
				break;
		}
		return "";
	}
    
    ApplicationEventReceiver::ApplicationEventReceiver() :
        m_app_state(applicationStateUninitialized),
        m_ui_state(UIStateUninitialized)
    {
        
    }
    
    bool ApplicationEventReceiver::onAppStateChange(const enApplicationState& newState)
    {
        if (m_app_state != newState)
        {
            m_app_state = newState;
            if (m_result.hasCallback())
            {
                rho::Hashtable<rho::String, rho::String> callbackData;
                const char* state = APP_EVENT_UNINITIALIZED;
                switch (newState) {
                    case applicationStateActivated:
                        state = APP_EVENT_ACTIVATED;
                        break;
                    case applicationStateDeactivated:
                        // special case, should be processed in foreground
                        m_result.setSynchronousCallback(true);
                        state = APP_EVENT_DEACTIVATED;
                        break;
                    default:
                        state = APP_EVENT_UNINITIALIZED;
                        break;
                }
                callbackData.put(APP_EVENT, state);
                
                m_result.set(callbackData);
                return true;
            }
        }
        return false;
    }
    
    bool ApplicationEventReceiver::onUIStateChange(const enUIState& newState)
    {
        // UIDestroyed is called when app is terminating or going to background mode
        // UICreated is called only when app is created
        // Events should be sent in any case
        if (m_ui_state != newState)
        {
            m_ui_state = newState;
            if (m_result.hasCallback())
            {
                rho::Hashtable<rho::String, rho::String> callbackData;
                const char* state = APP_EVENT_UNINITIALIZED;
                switch (newState) {
                    case UIStateCreated:
                        m_result.setSynchronousCallback(true);
                        state = APP_EVENT_UICREATED;
                        break;
                    case UIStateDestroyed:
                        m_result.setSynchronousCallback(true);
                        state = APP_EVENT_UIDESTROYED;
                        break;
                    default:
                        state = APP_EVENT_UNINITIALIZED;
                        break;
                }
                callbackData.put(APP_EVENT, state);
                m_result.set(callbackData);
                return true;
            }
        }
        return false;
    }
    
    bool ApplicationEventReceiver::onSyncUserChanged()
    {
        if (!m_result.hasCallback())
            return false;

        rho::Hashtable<rho::String, rho::String> callbackData;

        callbackData.put(APP_EVENT, APP_EVENT_SYNCUSERCHANGED);
        m_result.set(callbackData);

        return true;
    }
    
    bool ApplicationEventReceiver::onReinstallConfigUpdate(const HashtablePtr<String,Vector<String>* >& conflicts)
    {
        // at this point http server is not yet started
        // there is no way to call ruby and javascript callbacks
        // TODO: find a way to execute ruby callbacks during initialization
        if (!RHODESAPP().isLocalServerRunning())
        {
            return false;
        }
        
        using namespace rho::apiGenerator;
        if (m_result.hasCallback() && (conflicts.size() > 0) )
        {
            StringifyHash confhash;            
            for ( HashtablePtr<String,Vector<String>* >::const_iterator it=conflicts.begin() ; it != conflicts.end(); it++ )
            {
                String key = it->first;
                Vector<String>& ref = *(it->second);
                StringifyVector vect;
                //Vector<String>& current = conv[key];
                for( Vector<String>::const_iterator it2=ref.begin(); it2 != ref.end(); it2++)
                {
                    vect.push_back(*it2);
                }
                confhash.set(key, vect);
            }
            
            StringifyHash result;
            result.set(APP_EVENT, APP_EVENT_CONFIGCONFLICT);
            result.set(APP_EVENT_DATA, confhash);
            
            rho::String buffer;
            result.toString(buffer);
            
            m_result.setJSON(buffer);
        }

        return false;
    }
    
    bool ApplicationEventReceiver::onMigrateSource(){
        return false;
    }

    bool ApplicationEventReceiver::onDeviceScreenEvent(const int newState) {
        if (m_result.hasCallback())
        {
            rho::Hashtable<rho::String, rho::String> callbackData;
            const char* state = APP_EVENT_UNINITIALIZED;
            m_result.setSynchronousCallback(false);
            switch (newState) {
                case screenOff:
                    state = APP_EVENT_SCREEN_OFF;
                    //special case: need to call screenOff callback synchronously to make it called before device goes to inactive state.
                    m_result.setSynchronousCallback(true);
                    break;
                case screenOn:
                    state = APP_EVENT_SCREEN_ON;
                    break;
                default:
                    state = APP_EVENT_UNINITIALIZED;
                    break;
            }
            callbackData.put(APP_EVENT, state);
            m_result.set(callbackData);
            return true;
        }

        return true;
    }
        
    bool ApplicationEventReceiver::isCallbackSet(){
        return m_result.hasCallback();
    }
    
    void ApplicationEventReceiver::setCallback(apiGenerator::CMethodResult& oResult){
        m_result = oResult;
    }

	

} //namespace common
} //namespace rho

extern "C" {

using namespace rho;
using namespace rho::common;
unsigned long rho_rhodesapp_GetCallbackObject(int nIndex)
{
    return RHODESAPP().getCallbackObject(nIndex);
}

char* rho_http_normalizeurl(const char* szUrl) 
{
    rho::String strRes = RHODESAPP().canonicalizeRhoUrl(szUrl);
    return strdup(strRes.c_str());
}

void rho_http_free(void* data)
{
    free(data);
}
    
void rho_http_redirect( void* httpContext, const char* szUrl)
{
    HttpHeaderList headers;
    headers.push_back(HttpHeader("Location", szUrl));
    headers.push_back(HttpHeader("Content-Length", 0));
    headers.push_back(HttpHeader("Pragma", "no-cache"));
    headers.push_back(HttpHeader("Cache-Control", "must-revalidate"));
    headers.push_back(HttpHeader("Cache-Control", "no-cache"));
    headers.push_back(HttpHeader("Cache-Control", "no-store"));
    headers.push_back(HttpHeader("Expires", 0));
    headers.push_back(HttpHeader("Content-Type", "text/plain"));
    
    CHttpServer *serv = (CHttpServer *)httpContext;
    serv->send_response(serv->create_response("301 Moved Permanently", headers), true);
}

void rho_http_senderror(void* httpContext, int nError, const char* szMsg)
{
    char buf[30];
    snprintf(buf, sizeof(buf), "%d", nError);
    
    rho::String reason = buf;
    reason += " ";
    reason += szMsg;
    
    CHttpServer *serv = (CHttpServer *)httpContext;
    serv->send_response(serv->create_response(reason));
}

void rho_http_sendresponse(void* httpContext, const char* szBody)
{
    size_t nBodySize = strlen(szBody);
    
    HttpHeaderList headers;
    headers.push_back(HttpHeader("Content-Length", nBodySize));
    headers.push_back(HttpHeader("Pragma", "no-cache"));
    headers.push_back(HttpHeader("Cache-Control", "no-cache"));
    headers.push_back(HttpHeader("Expires", 0));
    
    const char *fmt = "%a, %d %b %Y %H:%M:%S GMT";
    char date[64], lm[64], etag[64];
    time_t	_current_time = time(0);
    strftime(date, sizeof(date), fmt, localtime(&_current_time));
    strftime(lm, sizeof(lm), fmt, localtime(&_current_time));
    snprintf(etag, sizeof(etag), "\"%lx.%lx\"", (unsigned long)_current_time, (unsigned long)nBodySize);
    
    headers.push_back(HttpHeader("Date", date));
    headers.push_back(HttpHeader("Last-Modified", lm));
    headers.push_back(HttpHeader("Etag", etag));
    
    CHttpServer *serv = (CHttpServer *)httpContext;
    serv->send_response(serv->create_response("200 OK", headers, szBody));
}

int	rho_http_snprintf(char *buf, size_t buflen, const char *fmt, ...)
{
	va_list		ap;
	int		n;
		
	if (buflen == 0)
		return (0);
		
	va_start(ap, fmt);
	n = vsnprintf(buf, buflen, fmt, ap);
	va_end(ap);
		
	if (n < 0 || (size_t) n >= buflen)
		n = buflen - 1;
	buf[n] = '\0';
		
	return (n);
}

int rho_http_started()
{
  return RHODESAPP().isLocalServerRunning()?1:0;
}

int rho_http_get_port()
{
  return RHODESAPP().getLocalServerPort();
}

int rho_nodejs_get_port()
{
  return RHODESAPP().getNodeJSServerPort();
}
    
    
#ifdef OS_MACOSX
const char* rho_http_direct_request( const char* method, const char* uri, const char* query, const void* headers, const char* body, int bodylen, int* responseLength )
{

  String sMethod;
  String sUri;
  String sQuery;
  String sBody;
  rho::net::HttpHeaderList oHeaders;
  
  if ( method != 0 ) {
    sMethod = method;
  }
  
  if ( uri != 0 ) {
    sUri = uri;
  }
  
  if  ( query != 0 ) {
    sQuery = query;
  }
  
  if ( body != 0 ) {
    sBody.assign(body, bodylen);
  }
  
  if ( headers != 0 ) {
    oHeaders = *(rho::net::HttpHeaderList*)headers;
  }

  String response = RHODESAPP().directHttpRequest( sMethod, sUri, sQuery, oHeaders, sBody );
  
  char* ret = 0;
  
  if ( response.length() != 0 ) {
    ret = new char[response.length() + 1];
    memmove(ret, response.c_str(), response.length());
  }
  
  if ( responseLength != 0 )
  {
    *responseLength = (int)response.length();
  }
  
  return ret;
}

void rho_http_free_response( const char* data )
{
  delete[] data;
}

void* rho_http_init_headers_list()
{
  return new rho::net::HttpHeaderList();
}

void rho_http_add_header( void* list, const char* name, const char* value )
{
  if ( ( name != 0 ) && ( value != 0 ) ) {
    ((rho::net::HttpHeaderList*)list)->addElement( rho::net::HttpHeader(name,value));
  }
}

void rho_http_free_headers_list( void* list )
{
  delete (rho::net::HttpHeaderList*)list;
}
#endif

	
void rho_rhodesapp_create(const char* szRootPath)
{
    rho::common::CRhodesApp::Create(szRootPath, szRootPath, szRootPath);
}

void rho_rhodesapp_create_with_separate_user_path(const char* szRootPath, const char* szUserPath)
{
    rho::common::CRhodesApp::Create(szRootPath, szUserPath, szRootPath);
}

void rho_rhodesapp_create_with_separate_runtime(const char* szRootPath, const char* szRuntimePath)
{
    rho::common::CRhodesApp::Create(szRootPath, szRootPath, szRuntimePath);
}
    
    
void rho_rhodesapp_start()
{
    RHODESAPP().startApp();
}
    
void rho_rhodesapp_destroy()
{
    rho::common::CRhodesApp::Destroy();
}
/*
const char* rho_rhodesapp_getfirststarturl()
{
    return RHODESAPP().getFirstStartUrl().c_str();
}*/

const char* rho_rhodesapp_getstarturl()
{
    return RHODESAPP().getStartUrl().c_str();
}

const char* rho_rhodesapp_gethomeurl()
{
    return RHODESAPP().getHomeUrl().c_str();
}

const char* rho_rhodesapp_getoptionsurl()
{
    return RHODESAPP().getOptionsUrl().c_str();
}

void rho_rhodesapp_keeplastvisitedurl(const char* szUrl)
{
    RHODESAPP().keepLastVisitedUrl(szUrl);
}

const char* rho_rhodesapp_getcurrenturl(int index)
{
    return RHODESAPP().getCurrentUrl(index).c_str();
}

const char* rho_rhodesapp_getloadingpagepath()
{
    return RHODESAPP().getLoadingPagePath().c_str();
}

const char* rho_rhodesapp_getblobsdirpath()
{
    return RHODESAPP().getBlobsDirPath().c_str();
}
    
const char* rho_rhodesapp_getuserrootpath()
{
    return RHODESAPP().getRhoUserPath().c_str();
}
    

const char* rho_rhodesapp_getdbdirpath()
{
    return RHODESAPP().getDBDirPath().c_str();
}

const char* rho_rhodesapp_getapprootpath() 
{
    return RHODESAPP().getAppRootPath().c_str();
}

void rho_rhodesapp_navigate_back()
{
    RHODESAPP().navigateBack();
}

void rho_rhodesapp_callCameraCallback(const char* strCallbackUrl, const char* strImagePath, 
    const char* strError, int bCancel )
{
    RHODESAPP().callCameraCallback(strCallbackUrl, strImagePath, strError, bCancel != 0);
}

void rho_rhodesapp_callSignatureCallback(const char* strCallbackUrl, const char* strSignaturePath, 
									  const char* strError, int bCancel )
{
	RHODESAPP().callSignatureCallback(strCallbackUrl, strSignaturePath, strError, bCancel != 0);
}
	
void rho_rhodesapp_callDateTimeCallback(const char* strCallbackUrl, long lDateTime, const char* szData, int bCancel )
{
    RHODESAPP().callDateTimeCallback(strCallbackUrl, lDateTime, szData, bCancel != 0);
}

void rho_rhodesapp_callBluetoothCallback(const char* strCallbackUrl, const char* body) {
	RHODESAPP().callBluetoothCallback(strCallbackUrl, body);
}

void rho_rhodesapp_callPopupCallback(const char *strCallbackUrl, const char *id, const char *title)
{
    RHODESAPP().callPopupCallback(strCallbackUrl, id, title);
}

void rho_rhodesapp_callAppActiveCallback(int nActive)
{
	if ( rho::common::CRhodesApp::getInstance() )
		RHODESAPP().callAppActiveCallback(nActive!=0);
}

void rho_rhodesapp_callUiCreatedCallback()
{
	if ( rho::common::CRhodesApp::getInstance() )
        RHODESAPP().callUiCreatedCallback();
    else
    {
        RAWLOGC_ERROR("RhodesApp", "UI created callback is missing because application instance is NULL");
    }
}

int rho_rhodesapp_callUiCloseRequestCallback() //return zero if you whant to suppress closing
{
    if ( rho::common::CRhodesApp::getInstance() ){
        return RHODESAPP().callUiCloseRequestCallback();
    }

    return 1;
}


void rho_rhodesapp_callUiDestroyedCallback()
{
    if ( rho::common::CRhodesApp::getInstance() )
        RHODESAPP().callUiDestroyedCallback();
}

void rho_rhodesapp_callScreenOffCallback()
{
    if ( rho::common::CRhodesApp::getInstance() && RHODESAPP().getApplicationEventReceiver() )
        RHODESAPP().getApplicationEventReceiver()->onDeviceScreenEvent(rho::common::screenOff);
}

void rho_rhodesapp_callScreenOnCallback()
{
    if ( rho::common::CRhodesApp::getInstance() && RHODESAPP().getApplicationEventReceiver() )
        RHODESAPP().getApplicationEventReceiver()->onDeviceScreenEvent(rho::common::screenOn);
}

void rho_rhodesapp_callScreenOnCallbackAsync()
{
    RHODESAPP().callScreenOnCallbackAsync();
}

    
const char* rho_rhodesapp_getappbackurl()
{
    return RHODESAPP().getAppBackUrl().c_str();
}

int rho_rhodesapp_callPushCallback(const char* szData)
{
    if ( !rho::common::CRhodesApp::getInstance() )
        return 1;

    return RHODESAPPBASE().callPushCallback(szData?szData:"") ? 1 : 0;
}

//int rho_rhodesapp_callPushCallbackWithJsonBody(const char* szUrl, const char* szData, const char* szParam)
//{
//    if ( !rho::common::CRhodesApp::getInstance() )
//        return 1;
//
//    return RHODESAPP().callPushCallbackWithJsonBody(szUrl, szData, szParam) ? 1 : 0;
//}


void rho_rhodesapp_callScreenRotationCallback(int width, int height, int degrees)
{
    if ( !rho::common::CRhodesApp::getInstance() )
        return;
	RHODESAPP().callScreenRotationCallback(width, height, degrees);
}

const char* rho_ruby_getErrorText(int nError)
{
    return RHODESAPP().getRhoMessage( nError, "").c_str();
}

const char* rho_ruby_getMessageText(const char* szName)
{
    return RHODESAPP().getRhoMessage( 0, szName).c_str();
}

int rho_rhodesapp_isrubycompiler()
{
    return 0;
}

void rho_net_request(const char *url)
{
    String strCallbackUrl = RHODESAPP().canonicalizeRhoUrl(url);
    getNetRequest().pullData(strCallbackUrl.c_str(), NULL);
}
    
//TODO: this function can crash application if you use it many times (it creates threads and probably don't delete them)
void rho_net_request_with_data_in_separated_thread(const char *url, const char *str_body) {
    String strCallbackUrl = RHODESAPP().canonicalizeRhoUrl(url);
    String strBody = str_body;
    RHODESAPP().runCallbackInThread(strCallbackUrl, strBody);
}
    

	
void rho_rhodesapp_load_url(const char *url)
{
    RHODESAPP().loadUrl(url);
}

int rho_rhodesapp_check_mode()
{
    if (RHODESAPP().deactivationMode())
    {
        LOG(ERROR) + "Operation is not allowed in 'deactivation' mode";
        return 0;
    }
    return 1;
}

#if defined(OS_ANDROID) && defined(RHO_LOG_ENABLED)
int rho_log(const char *fmt, ...)
{
  va_list vl;
  va_start(vl, fmt);
  int ret = __android_log_vprint(ANDROID_LOG_INFO, "RhoLog", fmt, vl);
  va_end(vl);
  return ret;
}

unsigned long long rho_cur_time()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    return ((unsigned long long)tv.tv_sec)*1000000 + tv.tv_usec;
}
#endif

void rho_free_callbackdata(void* pData)
{
	//It is used in SyncClient.
}

const char* rho_rhodesapp_getStartParametersOriginal()
{
    return RHODESAPP().getStartParametersOriginal().c_str();
}

void rho_rhodesapp_setStartParametersOriginal(const char* szParams)
{
    RHODESAPP().setStartParametersOriginal(szParams? szParams:"");
}

int rho_rhodesapp_is_application_active() {
    return RHODESAPP().isApplicationActive() ? 1 : 0;
}
    
int rho_rhodesapp_is_nodejs_app() {
    return RHODESAPP().isNodeJSApplication() ? 1 : 0;
}

int rho_rhodesapp_is_rubynodejs_app() {
    return RHODESAPP().isRubyNodeJSApplication() ? 1 : 0;
}

const char* rho_rhodesapp_rubyhomeurl() {
    return RHODESAPP().getRubyHomeURL().c_str();
}

    
int rho_rhodesapp_canstartapp(const char* szCmdLine, const char* szSeparators)
{
    String strCmdLineSecToken;
    String security_key = "security_token=";
    String strCmdLine = szCmdLine ? szCmdLine : "";

    CRhodesApp::setStartParameters(szCmdLine);
    RAWLOGC_INFO1("RhodesApp", "New start params: %s", strCmdLine.c_str());

    CRhodesApp::setSecurityTokenNotPassed(false);
    
	const char* szAppSecToken = get_app_build_config_item("security_token");
    if ( !szAppSecToken || !*szAppSecToken)
        return 1;

	int skpos = strCmdLine.find(security_key);
	if ((String::size_type)skpos != String::npos) 
    {
		String tmp = strCmdLine.substr(skpos+security_key.length(), strCmdLine.length() - security_key.length() - skpos);

		int divider = tmp.find_first_of(szSeparators);
		if ((String::size_type)divider != String::npos)
			strCmdLineSecToken = tmp.substr(0, divider);
		else
			strCmdLineSecToken = tmp;
	}
    int result = strCmdLineSecToken.compare(szAppSecToken) != 0 ? 0 : 1; 
    CRhodesApp::setSecurityTokenNotPassed(!result);

    return result; 
}
    

    //TODO: remove it
    void rho_sys_set_network_status_notify(const char* /*url*/, int /*poll_interval*/)
	{
        // commented out because first parameter should be a MethodResult, not char*; so this call will result in 100% app crash
        //RHODESAPP().setNetworkStatusNotify(url,poll_interval);
	}
	
	void rho_sys_clear_network_status_notify()
	{
		RHODESAPP().clearNetworkStatusNotify();
	}


} //extern "C"

#if !defined(OS_WINDOWS_DESKTOP) && !defined(RHODES_EMULATOR) && !defined(OS_ANDROID)

extern "C" void alert_show_status(const char* title, const char* message, const char* szHide);

extern "C"
{
	void rho_alert_show_status(char* szTitle, char* szText, char* szHideLabel)
	{
		alert_show_status( szTitle ? szTitle : "", szText ? szText : "", szHideLabel ? szHideLabel : "");
	}
}
#endif

extern "C" int rho_is_remote_debug()
{
    return RHOCONF().getBool("remotedebug");
}

extern "C" const char* rho_get_remote_debug_host()
{
    return RHOCONF().getString("debughosturl").c_str();
}
