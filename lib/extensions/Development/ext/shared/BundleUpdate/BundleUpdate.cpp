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
extern "C" void rho_sys_replace_current_bundleEx(const char* path, const char* finish_callback, bool do_not_restart_app, bool not_thread_mode );


void callback_system_update_bundle(void *arg, rho::String const &strQuery)
{
    const char* s = strQuery.c_str();
    
    rho::String fileURL = s;
    
    rho::String fileZipLocalDir = RHODESAPPBASE().getRhoUserPath()+"RhoBundle";
    
    rho::String fileZipLocalPath = fileZipLocalDir+"/upgrade_bundle.zip";
    
    const char* flz = fileZipLocalPath.c_str();
    
    // 1 download zip
    
    rho::Hashtable<rho::String, rho::String> mapHeaders;
    mapHeaders["User-Agent"] = "Mozilla-5.0";
    
    rho::apiGenerator::CMethodResult oResult;
    
    bool overwriteFile = true;
    bool createFolders = true;
    
    bool fileExists = false;
    
    NetRequest oNetRequest;
    
    NetResponse resp = getNetRequest(&oNetRequest).pullFile( fileURL, fileZipLocalPath, null, &mapHeaders,overwriteFile,createFolders,&fileExists);
    
    
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
    }
    
    // 3 call update bundle
    
    bool do_not_restart_app = true;
    bool not_thread_mode = false;
    
    
    rho::String cb_url = /*RHODESAPPBASE().getHomeUrl()*/ "http://127.0.0.1:37579/system/update_bundle_callback";
    const char* hu = cb_url.c_str();
    
    rho_sys_replace_current_bundleEx( fileZipLocalDir.c_str(), cb_url.c_str(), do_not_restart_app, not_thread_mode );
    
    
    
    rho_http_sendresponse(arg, "");
}

void callback_system_update_bundle_callback(void *arg, rho::String const &strQuery) {
    const char* s = strQuery.c_str();
    rho_param p;
    p.type = RHO_PARAM_STRING;
    p.v.string = "Update bundle was finished !";
    
    alert_show_status("Development Extras", "Update bundle was finished !", "OK");
    
    //alert_show_popup(&p);
    
    rho_http_sendresponse(arg, "");
}


void Init_UpdateBundle() {
    rho::String our_IP = DevHTTPServer::getInstance()->getLocalIPAdress();
    rho::String message = "Connect to Development Extras HTTP server : http://";
    message = message + our_IP + ":37579";
    alert_show_status("Development Extras", message.c_str(), "OK");
}