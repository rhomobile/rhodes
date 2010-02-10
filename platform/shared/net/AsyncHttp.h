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

class CAsyncHttp : public common::CRhoThread
{
    DEFINE_LOGCLASS;

    static common::CMutex m_mxInstances;
    static Vector<CAsyncHttp*> m_arInstances;
    common::CAutoPtr<common::IRhoClassFactory> m_ptrFactory;

    common::CAutoPtr<INetRequest> m_pNetRequest;
    common::CAutoPtr<INetResponse> m_pNetResponse;
    Hashtable<String,String> m_mapHeaders;

    String m_strUrl, m_strBody, m_strCallback, m_strCallbackParams;
    boolean m_bPost;
    unsigned long m_valBody;

public:
    static boolean m_bNoThreaded;

    CAsyncHttp(common::IRhoClassFactory* factory, const char* url, unsigned long headers, const char* body, const char* callback, const char* callback_params);
    ~CAsyncHttp();

    void cancel();

    static void cancelRequest(const char* szCallback);

private:
    virtual void run();

    void processResponse(INetResponse& resp );
    void callNotify(INetResponse& resp );

    String makeHeadersString();
};

} // namespace net
} // namespace rho

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void rho_asynchttp_get(const char* url, unsigned long headers, const char* callback, const char* callback_params);
void rho_asynchttp_post(const char* url, unsigned long headers, const char* body, const char* callback, const char* callback_params);
void rho_asynchttp_cancel(const char* cancel_callback);
void rho_asynchttp_set_threaded_mode(int b);

#ifdef __cplusplus
};
#endif //__cplusplus
