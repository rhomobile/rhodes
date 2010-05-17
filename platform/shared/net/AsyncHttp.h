#pragma once

#ifdef __cplusplus

#include "net/INetRequest.h"
#include "logging/RhoLog.h"
#include "common/RhoThread.h"
#include "common/IRhoClassFactory.h"

namespace rho
{
namespace net
{

class CAsyncHttp : public common::CRhoThread, rho::ICallbackObject
{
    DEFINE_LOGCLASS;

    static common::CMutex m_mxInstances;
    static VectorPtr<CAsyncHttp*> m_arInstances;
    common::CAutoPtr<common::IRhoClassFactory> m_ptrFactory;
    common::CMutex m_mxRequest;

    common::CAutoPtr<INetRequest> m_pNetRequest;
    common::CAutoPtr<INetResponse> m_pNetResponse;
    Hashtable<String,String> m_mapHeaders;

    String m_strUrl, m_strBody, m_strCallback, m_strCallbackParams;
    String m_strFilePath;
	String m_strResBody;
    boolean m_sslVerifyPeer;
    boolean m_bFinished;
public:
    enum EHttpCommands{ hcGet = 0, hcPost, hcDownload, hcUpload };
private:
    EHttpCommands m_eCmd;
public:

    static boolean m_bNoThreaded;

    CAsyncHttp(common::IRhoClassFactory* factory, EHttpCommands eCmd,
        const char* url, unsigned long headers, const char* body,
        const char* file_path,
        const char* callback, const char* callback_params, boolean ssl_verify_peer = true);

    void cancel(boolean bWait);

    static void cancelRequest(const char* szCallback, boolean bWait);

    //rho::ICallbackObject
    virtual unsigned long getObjectValue();

    unsigned long getRetValue();
private:
    virtual void run();

    void callNotify(INetResponse& resp, int nError );

    String makeHeadersString();
    static void addNewObject(CAsyncHttp* pObj);
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
void rho_asynchttp_destroy();

#ifdef __cplusplus
};
#endif //__cplusplus
