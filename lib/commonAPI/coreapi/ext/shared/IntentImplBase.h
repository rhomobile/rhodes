#include "generated/cpp/IntentBase.h"
#include "logging/RhoLog.h"

namespace rho {

using namespace apiGenerator;

class CIntentImplBase: public CIntentSingletonBase
{
public:
    CIntentImplBase(): CIntentSingletonBase(), m_appMessageNotifications(false), m_appMessageWait(false) {}

    void addApplicationMessage(const rho::String& appName, const rho::String& msg);

protected:
    void sendApplicationMessage( const rho::String& appName, const rho::String& params, rho::apiGenerator::CMethodResult& oResult);
    void getApplicationMessage(rho::apiGenerator::CMethodResult& oResult);
    void startApplicationMessageNotifications(rho::apiGenerator::CMethodResult& oResult);
    void stopApplicationMessageNotifications(rho::apiGenerator::CMethodResult& oResult);

    DEFINE_LOGCLASS;

    Vector<Hashtable<String, String> > m_appMessageQueue;
    common::CMutex m_appMessageMutex;
    bool m_appMessageNotifications;
    CMethodResult m_appMessageHandler;
    bool m_appMessageWait;
    CMethodResult m_appMessageResult;
};

}
