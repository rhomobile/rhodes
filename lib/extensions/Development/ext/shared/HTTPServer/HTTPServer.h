//
//  HTTPServer.h
//  Development
//
//  Created by Dmitry Soldatenkov on 12.08.14.
//
//

#include "common/RhodesApp.h"

#include "common/ThreadQueue.h"

#include "net/INetRequest.h"

#include "net/HttpServer.h"

#include "../BundleUpdate/BundleUpdate.h"


#define RHO_DEVELOPMENT_SERVER_PORT 37579


class DevHTTPServer : public rho::common::CRhoThread
{

    
private:
    
    rho::common::CAutoPtr<rho::net::CHttpServer> m_httpServer;
    rho::common::CAutoPtr<rho::common::CThreadQueue> m_appCallbacksQueue;
    rho::String m_local_IP_adress;
    
public:

    void init();
    static DevHTTPServer* getInstance();
    
    rho::String getLocalIPAdress();
    
protected:
    virtual void run();

};



extern "C" void init_Development_HTTP_Server();


