#pragma once

#ifdef __cplusplus

#include "net/INetRequest.h"
#include "logging/RhoLog.h"
#include "common/ThreadQueue.h"
#include "common/rhoparams.h"

namespace rho
{
namespace net
{

class CAsyncHttp : public common::CThreadQueue 
{
    DEFINE_LOGCLASS;

    class CAsyncHttpResponse : public rho::ICallbackObject
    {
        NetResponse m_NetResponse;
        String m_strContentType;
    public:
        CAsyncHttpResponse(NetResponse& resp, const String& strContentType)
        {
            m_NetResponse = resp;
            m_strContentType = strContentType;
        }
        ~CAsyncHttpResponse();

        virtual unsigned long getObjectValue();

    };

public:
    enum EHttpCommands{ hcNone = 0, hcGet, hcPost, hcDownload, hcUpload };

    class CHttpCommand : public IQueueCommand
    {
    public:
	    EHttpCommands m_eCmd;
        String m_strCallback, m_strCallbackParams;
        Hashtable<String,String> m_mapHeaders;

        NetRequest m_NetRequest;
        String m_strResBody;

        CRhoParams    m_params;

        CHttpCommand(String strCmd, rho_param *p);

        EHttpCommands translateCommand(String strCmd)
        {
            if ( strCmd.compare("GET") == 0 )
                return hcGet;
            else if ( strCmd.compare("POST") == 0 )
                return hcPost;
            else if ( strCmd.compare("Download") == 0 )
                return hcDownload;
            else if ( strCmd.compare("Upload") == 0 )
                return hcUpload;

            return hcNone;
        }

        void execute();
        void cancel();

        void callNotify(NetResponse& resp, int nError );
        unsigned long getRetValue();

	    boolean equals(const IQueueCommand& cmd){ return false; }

        String toString()
        {
            switch(m_eCmd)
            {
            case hcGet:
                return "GET";
            case hcPost:
                return "POST";
            case hcDownload:
                return "Download";
            case hcUpload:
                return "Upload";
            }
            return "Unknown";
        }

    private:
        String makeHeadersString();
        net::CNetRequestWrapper getNet(){ return getNetRequest(&m_NetRequest); }
    
    };

private:
    static CAsyncHttp* m_pInstance;

    CHttpCommand* m_pCurCmd;

public:

    static CAsyncHttp* Create();
    static void Destroy();
    static CAsyncHttp* getInstance(){ return m_pInstance; }

    void cancelRequest(const char* szCallback, boolean bWait);

    unsigned long addHttpCommand(IQueueCommand* pCmd);

private:
    CAsyncHttp();
    ~CAsyncHttp();

    virtual void processCommand(IQueueCommand* pCmd);
};

} // namespace net
} // namespace rho

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

unsigned long rho_asynchttp_request(const char* command, rho_param *p);
void rho_asynchttp_cancel(const char* cancel_callback);

#ifdef __cplusplus
};
#endif //__cplusplus
