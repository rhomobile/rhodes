//
//  HTTPServer.cpp
//  Development
//
//  Created by Dmitry Soldatenkov on 12.08.14.
//
//

//#include <stdio.h>

#include "HTTPServer.h"

#include "net/HttpServer.h"
#include "common/RhodesApp.h"
#include "common/RhoFilePath.h"
#include "common/RhoConf.h"
#include "net/URI.h"
#include "ruby/ext/rho/rhoruby.h"
#include "common/Tokenizer.h"
#include "sync/RhoconnectClientManager.h"
#include "statistic/RhoProfiler.h"


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef OS_ANDROID
#include "../../platform/android/jni/ifaddrs.h"
#else
#include <ifaddrs.h>
#endif
#include <netinet/in.h>
#include <string.h>

#if !defined(WINDOWS_PLATFORM)
#include <arpa/inet.h>
#endif

#include "logging/RhoLog.h"

#define DEFAULT_LOGCATEGORY "Development_HTTPServer"

static DevHTTPServer* ourDevHTTPServer = NULL;


extern "C" rho::String get_local_ip_adress();

void DevHTTPServer::init() {
    
    m_local_IP_adress = "";
    rho::String szRootPath = rho_native_rhopath();
    rho::String szUserPath = rho_native_rhouserpath();
    
    m_httpServer = new rho::net::CHttpServer(RHO_DEVELOPMENT_SERVER_PORT, szRootPath, szUserPath, szRootPath, true, true);
    
    m_httpServer->register_uri("/system/update_bundle", callback_system_update_bundle);
    m_httpServer->register_uri("/system/update_bundle_callback", callback_system_update_bundle_callback);

    
#ifdef OS_ANDROID
    m_local_IP_adress = get_local_ip_adress();
#else
    {
        struct ifaddrs * ifAddrStruct=NULL;
        struct ifaddrs * ifa=NULL;
        void * tmpAddrPtr=NULL;
        
        getifaddrs(&ifAddrStruct);
        
        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
            if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
                // is a valid IP4 Address
                tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                unsigned char *n = (unsigned char*)tmpAddrPtr;
                

                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                if ((ifa->ifa_name[0] == 'e') && (ifa->ifa_name[1] == 'n')) {
                    //    printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
                    m_local_IP_adress = addressBuffer;
                }
            }
            //   else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            //   // is a valid IP6 Address
            //   tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            //   char addressBuffer[INET6_ADDRSTRLEN];
            //   inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            //   printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
            //   }
     
        }
        if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    }
    
    
#endif
    
    RAWTRACE("$$$ Our IP adress is : $$$");
    RAWTRACE(m_local_IP_adress.c_str());
    RAWTRACE("$$$$$$$$$$$$$$$$$$$$$$$$$$");
    
}



DevHTTPServer* DevHTTPServer::getInstance() {
    if (ourDevHTTPServer == NULL) {
        ourDevHTTPServer = new DevHTTPServer();
    }
    return ourDevHTTPServer;
}

rho::String DevHTTPServer::getLocalIPAdress() {
    return m_local_IP_adress;
}

void DevHTTPServer::run() {
    for (;;) {
        m_httpServer->run();
    }
}






void init_Development_HTTP_Server() {
    DevHTTPServer * ds = DevHTTPServer::getInstance();
    ds->init();
    ds->start(rho::common::IRhoRunnable::epNormal);
    
    

}