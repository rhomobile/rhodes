#pragma once

#ifdef __cplusplus

#include "net/INetRequest.h"
#include "logging/RhoLog.h"
#include "common/ThreadQueue.h"

namespace rho
{
namespace net
{

class CAsyncHttp : public common::CThreadQueue 
{
    DEFINE_LOGCLASS;

    class CAsyncHttpResponse : public rho::ICallbackObject
    {
        common::CAutoPtr<INetResponse> m_pNetResponse;
        String m_strContentType;
    public:
        CAsyncHttpResponse(INetResponse* pResp, const String& strContentType)
        {
            m_pNetResponse = pResp;
            m_strContentType = strContentType;
        }
        ~CAsyncHttpResponse();

        virtual unsigned long getObjectValue();

    };

public:
    enum EHttpCommands{ hcGet = 0, hcPost, hcDownload, hcUpload };

    class CHttpCommand : public CQueueCommand
    {
    public:
	    EHttpCommands m_eCmd;
        String m_strUrl, m_strBody, m_strCallback, m_strCallbackParams;
        String m_strFilePath;
        boolean m_sslVerifyPeer;
        Hashtable<String,String> m_mapHeaders;

        common::CAutoPtr<INetRequest> m_pNetRequest;
        String m_strResBody;

        CHttpCommand(EHttpCommands eCmd,
            const char* url, unsigned long headers, const char* body,
            const char* file_path,
            const char* callback, const char* callback_params, boolean ssl_verify_peer = true);

        void execute();
        void cancel();

        void callNotify(INetResponse* pResp, int nError );
        unsigned long getRetValue();

	    boolean equals(const CQueueCommand& cmd){ return false; }

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

    };

private:
    static CAsyncHttp* m_pInstance;

    CHttpCommand* m_pCurCmd;

public:

    static CAsyncHttp* Create();
    static void Destroy();
    static CAsyncHttp* getInstance(){ return m_pInstance; }

    void cancelRequest(const char* szCallback, boolean bWait);

    virtual void addQueueCommand(CQueueCommand* pCmd);

private:
    CAsyncHttp(common::IRhoClassFactory* factory);
    ~CAsyncHttp();

    virtual void processCommand(CQueueCommand* pCmd);
};

} // namespace net
} // namespace rho

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
unsigned long rho_asynchttp_get(const char* url, unsigned long headers, const char* callback, const char* callback_params, int ssl_verify_peer);
unsigned long rho_asynchttp_post(const char* url, unsigned long headers, const char* body, const char* callback, const char* callback_params, int ssl_verify_peer);
unsigned long rho_asynchttp_downloadfile(const char* url, unsigned long headers, const char* filename, const char* callback, const char* callback_params, int ssl_verify_peer);
unsigned long rho_asynchttp_uploadfile(const char* url, unsigned long headers, const char* body, const char* filename, const char* callback, const char* callback_params, int ssl_verify_peer);
void rho_asynchttp_cancel(const char* cancel_callback);

#ifdef __cplusplus
};
#endif //__cplusplus
