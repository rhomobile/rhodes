//
//  HTTPServer.cpp
//  Development
//
//  Created by Dmitry Soldatenkov on 12.08.14.
//
//

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

#if !defined(OS_WINCE)

#ifndef OS_ANDROID
#include <ifaddrs.h>
#endif

#include <netinet/in.h>
#else
//#include <ws2tcpip.h>
#include "net/CompatWince.h"
#endif

#include <string.h>

#if !defined(WINDOWS_PLATFORM)
#include <arpa/inet.h>
#endif

#include "logging/RhoLog.h"

#define DEFAULT_LOGCATEGORY "Development_HTTPServer"

static DevHTTPServer* ourDevHTTPServer = NULL;

//ULONG GetSinIP(struct sockaddr_in *_psin, LPSTR pIPStr, UINT IPMaxLen)
//{
//    unsigned long ipaddr;
//
//    ipaddr = INADDR_NONE;
//
//    if (pIPStr && IPMaxLen)
//        *pIPStr = '\0';
//
//    if ( _psin )
//    {
//#if defined(_WIN32)
//        ipaddr = _psin->sin_addr.S_un.S_addr;
//#else
//        ipaddr = _psin->sin_addr.s_addr;
//#endif
//
//        if (pIPStr && IPMaxLen)
//        {
//            char  *pIP;
//            struct in_addr in;
//
//#if defined(_WIN32)
//            in.S_un.S_addr = ipaddr;
//#else
//            in.s_addr = ipaddr;
//#endif
//
//            pIP = inet_ntoa(in);
//
//            if (pIP)
//                adjust_str(pIP, pIPStr, IPMaxLen);
//        }
//    }
//
//    return ipaddr;
//}




extern "C" rho::String get_local_ip_adress();

void DevHTTPServer::init() {
    
    m_local_IP_adress = "";
    rho::String szRootPath = rho_native_rhopath();
    rho::String szUserPath = rho_native_rhouserpath();
    
    m_httpServer = new rho::net::CHttpServer(RHO_DEVELOPMENT_SERVER_PORT, szRootPath, szUserPath, szRootPath, true, true);
    
    m_httpServer->disableAllLogging();
    
#ifdef OS_ANDROID
    m_local_IP_adress = get_local_ip_adress();
#elif defined(OS_WINCE)
  
    char host[22];
    hostent* host_info = 0 ;

    if(gethostname(host, sizeof(host)) == SOCKET_ERROR) {
        return;
    }

    host_info = gethostbyname(host);

    if(host_info)
    {
        const in_addr* address = (in_addr*)host_info->h_addr_list[0];

        rho::String addrIP = inet_ntoa( *address );

        m_local_IP_adress = addrIP;
    }
    else 
    {
        //error
    }
#else
    //Obtain the computer's IP
    {
        struct ifaddrs * ifAddrStruct=NULL;
        struct ifaddrs * ifa=NULL;
        void * tmpAddrPtr=NULL;
        
        getifaddrs(&ifAddrStruct);
        
        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) 
        {
            if (ifa ->ifa_addr->sa_family == AF_INET) { // check it is IP4
                // is a valid IP4 Address
                tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                unsigned char *n = (unsigned char*)(tmpAddrPtr);
                
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

rho::net::CHttpServer* DevHTTPServer::getHTTPServer() {
    return m_httpServer;
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

rho::String DevHTTPServer::getPort() {
    rho::String res = "";
    char ttt[16];
    int port = m_httpServer->getPort();
    sprintf(ttt, "%d", port);
    res = ttt;
    return res;
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