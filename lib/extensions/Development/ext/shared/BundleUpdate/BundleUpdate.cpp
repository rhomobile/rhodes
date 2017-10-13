//
//  BundleUpdate.cpp
//  Development
//
//  Created by Dmitry Soldatenkov on 18.08.14.
//
//

#include <stdio.h>
#include "RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoFile.h"
#include "common/RhoConf.h"
#include "common/RhoFilePath.h"
#include "common/Tokenizer.h"

#include "ruby/ext/rho/rhoruby.h"
#include <algorithm>


#include "net/INetRequest.h"

#include "BundleUpdate.h"

#include "../HTTPServer/HTTPServer.h"


extern "C" int rho_sys_unzip_file(const char* szZipPath, const char* psw);
extern "C" void alert_show_popup(rho_param *p);
extern "C" void alert_show_status(const char* title, const char* message, const char* hideCaption);
extern "C" void rho_sys_replace_current_bundleEx(const char* path, const char* finish_callback, bool do_not_restart_app, bool not_thread_mode, bool check_filelist, bool all_via_callback_only);


extern "C" void rho_webview_refresh(int index);



//static rho::String our_responce_server_url = "";
//static bool our_refresh_webview = false;


class BundleUpdateThreadQueue : public rho::common::CThreadQueue
{
    DEFINE_LOGCLASS;
public:
    struct BUCommand : public IQueueCommand
    {
        rho::String bundle_url;
        rho::String responce_url;
        bool refresh_webview;
        bool canContinue;
        
        
        BUCommand(rho::String &_bundle_url, rho::String &_responce_url, bool _refresh_webview)
        :bundle_url(_bundle_url), responce_url(_responce_url), refresh_webview(_refresh_webview)
        {
            canContinue = false;
        }
        
        bool equals(IQueueCommand const &) {return false;}
        rho::String toString();
    };
public:
    BundleUpdateThreadQueue() {
        start(epNormal);
    }
    ~BundleUpdateThreadQueue() {
        
    }
    
    BUCommand* getCurrentBUCommand() {
        return (BUCommand*)getCurCommand();
    }
    
private:
    void processCommand(IQueueCommand* cmd);
};


rho::String BundleUpdateThreadQueue::BUCommand::toString()
{
    return bundle_url;
}


void BundleUpdateThreadQueue::processCommand(IQueueCommand *pCmd)
{
    BUCommand *cmd = (BUCommand*)pCmd;
    
    
    rho::String fileURL = cmd->bundle_url;
    
    rho::String fileZipLocalDir = rho::common::CFilePath::join(RHODESAPPBASE().getRhoUserPath(), "RhoBundle");
    
    rho::String fileZipLocalPath = rho::common::CFilePath::join(fileZipLocalDir, "upgrade_bundle.zip");
    
    const char* flz = fileZipLocalPath.c_str();
    
    
    // 0 remove old files
    rho::common::CRhoFile::deleteFolder(fileZipLocalDir.c_str());
    //rho_file_impl_delete_folder(fileZipLocalDir.c_str());
    rho::common::CRhoFile::recursiveCreateDir(fileZipLocalDir.c_str(), RHODESAPPBASE().getRhoUserPath().c_str());
    
    // 1 download zip
    
    rho::Hashtable<rho::String, rho::String> mapHeaders;
    mapHeaders["User-Agent"] = "Mozilla-5.0";
    
    rho::apiGenerator::CMethodResult oResult;
    
    bool overwriteFile = true;
    bool createFolders = true;
    
    bool fileExists = false;
    
    NetRequest oNetRequest;
    
    NetResponse resp = getNetRequest(&oNetRequest).pullFile( fileURL, fileZipLocalPath, NULL, &mapHeaders,overwriteFile,createFolders,&fileExists);
    
    
    if (resp.isSuccess()) {
        int o;
        o = 9;
    }
    else {
        int r;
        r = 9;
    }
    
    // 2 unpack zip
    int zip_res = rho_sys_unzip_file(fileZipLocalPath.c_str(), "");
    
    if (zip_res >= 0) {
        int o;
        o = 9;
    }
    else {
        int r;
        r = 9;
        // error
        // send request for full_update
        rho::String message = "ERROR when downloading and unpack Bundle [";
        message = message + fileURL + "] ! Cancel update !";
        alert_show_status("Development Extras", message.c_str(), "OK");
        return;
    }
    
    // 3 call update bundle
    
    bool do_not_restart_app = true;
    bool not_thread_mode = true;
    bool check_filelist = true;
    
    
    rho::String cb_url = /*RHODESAPPBASE().getHomeUrl()*/ "http://127.0.0.1:";//37579/development/update_bundle_callback";
    cb_url = cb_url + DevHTTPServer::getInstance()->getPort() + "/development/update_bundle_callback";
    const char* hu = cb_url.c_str();
    
    
    cmd->canContinue = false;

    rho_sys_replace_current_bundleEx( fileZipLocalDir.c_str(), cb_url.c_str(), do_not_restart_app, not_thread_mode, check_filelist, true );
    
    
    // wait responce
    while (!cmd->canContinue) {
        sleep(30);
    }
}






static BundleUpdateThreadQueue* ourBundleUpdateThreadQueue = NULL;

static BundleUpdateThreadQueue* getBundleUpdateThreadQueueSignletone() {
    if (ourBundleUpdateThreadQueue == NULL) {
        ourBundleUpdateThreadQueue = new BundleUpdateThreadQueue();
    }
    return ourBundleUpdateThreadQueue;
}








void callback_system_update_bundle(void *arg, rho::String const &strQuery)
{
    const char* s = strQuery.c_str();
    
    rho::String qURL = "";
    rho::String qServerIP = "";
    rho::String qServerPort = "";
    bool isRefresh = false;
    
    
    rho::common::CTokenizer oTokenizer(strQuery, "&");
    while (oTokenizer.hasMoreTokens())
    {
        rho::String tok = oTokenizer.nextToken();
        if (tok.length() == 0)
        continue;
        
        size_t nPos = tok.find("package_url=");
        if ( nPos != rho::String::npos )
        {
            qURL = strQuery.substr(nPos+12);
        }
        if ( rho::String_startsWith(tok, "package_url=") )
        {
            qURL = tok.substr(12);
        }else if ( rho::String_startsWith( tok, "server_ip=") )
        {
            qServerIP = tok.substr(10);
        }else if ( rho::String_startsWith( tok, "server_port=") )
        {
            qServerPort = tok.substr(12);
        }else if ( rho::String_startsWith( tok, "refresh_after_update=") )
        {
            rho::String srefresh = tok.substr(21);
            if ((srefresh.compare("false") != 0) && (srefresh.compare("0") != 0) && (srefresh.compare("FALSE") != 0)) {
                isRefresh = true;
            }
        }
    }

    rho::String our_responce_server_url = "http://";
    our_responce_server_url = our_responce_server_url + qServerIP + ":" + qServerPort + "/response_from_device";
    
    rho_http_sendresponse(arg, "");
    
    
    BundleUpdateThreadQueue::BUCommand* cmd = new BundleUpdateThreadQueue::BUCommand(qURL, our_responce_server_url, isRefresh);
    getBundleUpdateThreadQueueSignletone()->addQueueCommand(cmd);
    
}

rho::String make_info_string(bool is_json) {
    
    // "IP_Adress" : ""
    // "Port" : ""
    // "Device_Friendly_Name" : ""
    // "Application_Name" : ""
    // "Platform" : ""
    
    rho::String s_IP_Adress = DevHTTPServer::getInstance()->getLocalIPAdress();
    rho::String s_Port = DevHTTPServer::getInstance()->getPort();
    rho::String s_Device_Friendly_Name = "";
    char hostname[256];
    int r = gethostname(hostname, 256);
    if (r == 0) {
        s_Device_Friendly_Name = hostname;
    }
    rho::String s_Application_Name = RHODESAPP().getAppName();
#ifdef OS_ANDROID
    rho::String s_Platform = "ANDROID";
#else
    
#ifdef OS_MACOSX
    rho::String s_Platform = "APPLE";
#else
    
#ifdef OS_WINCE
    rho::String s_Platform = "WM";
#else
    rho::String s_Platform = "GENERIC";
#endif
    
#endif
    
#endif
    
    // {"jsonrpc": "2.0", };
    rho::String responce = "";
    
    if (is_json) {
        responce = responce + "{\"jsonrpc\": \"2.0\", ";
        responce = responce + "\"ip\": \"" + s_IP_Adress + "\", ";
        responce = responce + "\"port\": \"" + s_Port + "\", ";
        responce = responce + "\"deviceFriendlyName\": \"" + s_Device_Friendly_Name + "\", ";
        responce = responce + "\"applicationName\": \"" + s_Application_Name + "\", ";
        responce = responce + "\"platform\": \"" + s_Platform + "\" ";
        responce = responce + "}";
    }
    else {
        responce = responce + "&ip=" + s_IP_Adress;
        responce = responce + "&port=" + s_Port;
        responce = responce + "&platform=" + s_Platform;
    }
    return responce;
}


void callback_system_update_bundle_callback(void *arg, rho::String const &strQuery) {
    const char* s = strQuery.c_str();
    
    // status    ok,error,need_sync
    // message   "bla bla bla"
    
    rho::String qStatus = "";
    rho::String qMessage = "";
    
    
    rho::common::CTokenizer oTokenizer(strQuery, "&");
    while (oTokenizer.hasMoreTokens())
    {
        rho::String tok = oTokenizer.nextToken();
        if (tok.length() == 0)
        continue;
        
        if ( rho::String_startsWith(tok, "status=") )
        {
            qStatus = tok.substr(7);
        }else if ( rho::String_startsWith( tok, "message=") )
        {
            qMessage = tok.substr(8);
        }
    }
    
    rho::String query = "";
    
    bool our_refresh_webview = false;
    rho::String our_responce_server_url = "";
    
    BundleUpdateThreadQueue::BUCommand* cmd = getBundleUpdateThreadQueueSignletone()->getCurrentBUCommand();
    if (cmd != NULL) {
        our_refresh_webview = cmd->refresh_webview;
        our_responce_server_url = cmd->responce_url;
    }
    
    rho::String message = "Unrecognizing Situation during update bundle! Restart application and reconnect device to server !";
    if (qStatus.compare("ok") == 0) {
       message = "Update bundle was finished !";
       query = "&status=ok";
        
        if (our_refresh_webview) {
            rho_webview_refresh(-1);
        }
    }
    if (qStatus.compare("error") == 0) {
        message = "Error when update Bundle : " + qMessage + " !";
        query = "&status=error&message="+message;
    }
    if (qStatus.compare("need_sync") == 0) {
        message = "Your application files too old. Request for full Bundle update was sended to server !";
        query = "&status=need_full_update";
    }
    
    
    alert_show_status("Development Extras", message.c_str(), "OK");
    
    //alert_show_popup(&p);
    
    rho_http_sendresponse(arg, "");
    
    // send responce to server
    char* norm_url = rho_http_normalizeurl(our_responce_server_url.c_str());
    query = query + make_info_string(false);
    rho_net_request_with_data_in_separated_thread(norm_url, query.c_str());
    rho_http_free(norm_url);
    
    // remove temporary files
    rho::String fileZipLocalDir = rho::common::CFilePath::join(RHODESAPPBASE().getRhoUserPath(), "RhoBundle");
    //rho_file_impl_delete_folder(fileZipLocalDir.c_str());
    rho::common::CRhoFile::deleteFolder(fileZipLocalDir.c_str());
    
    if (cmd != NULL) {
        cmd->canContinue = true;
    }
}


void showInitialMessage() {
    rho::String our_IP = DevHTTPServer::getInstance()->getLocalIPAdress();
    rho::String message = "Connect to Development Extras HTTP server : http://";
    message = message + our_IP + ":" + DevHTTPServer::getInstance()->getPort();
    message = message + "\n 3 fingers tap - force WebView refresh";
    message = message + "\n 4 fingers tap - show this message again";
    alert_show_status("Development Extras", message.c_str(), "OK");
}

void Init_UpdateBundle() {
    
    DevHTTPServer::getInstance()->getHTTPServer()->register_uri("/development/update_bundle", callback_system_update_bundle);
    DevHTTPServer::getInstance()->getHTTPServer()->register_uri("/development/update_bundle_callback", callback_system_update_bundle_callback);
    DevHTTPServer::getInstance()->getHTTPServer()->register_uri("/development/get_info", callback_system_get_info_callback);
    
    showInitialMessage();
}


void callback_system_get_info_callback(void *arg, rho::String const &strQuery) {

    rho::String responce = make_info_string(true);
    
    rho_http_sendresponse(arg, responce.c_str());

}

void Bundle_update_on_triple_tap() {
    rho_webview_refresh(-1);
}

void Bundle_update_on_quadro_tap() {
    showInitialMessage();
}

