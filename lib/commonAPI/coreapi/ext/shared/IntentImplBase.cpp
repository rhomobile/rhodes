#include "IntentImplBase.h"

#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "sync/RhoconnectClientManager.h"
#include "common/RhoFilePath.h"
#include "common/RhoFile.h"
#include "unzip/zip.h"
#include "db/DBAdapter.h"
#include "Network.h"

//#undef DEFAULT_LOGCATEGORY
//#define DEFAULT_LOGCATEGORY "Intent"

using namespace rho::common;

namespace rho {

using namespace apiGenerator;

//IMPLEMENT_LOGCLASS(CIntentImplBase, "Intent");

void CIntentImplBase::getApplicationMessage(rho::apiGenerator::CMethodResult& oResult)
{
    common::CMutexLock lock(m_appMessageMutex);
    if(m_appMessageQueue.size() != 0)
    {
        LOG(TRACE) + "src: " + m_appMessageQueue.front()["appName"] + ", msg: " + m_appMessageQueue.front()["message"];

        oResult.set(m_appMessageQueue.front());
        m_appMessageQueue.erase(m_appMessageQueue.begin());
    }
    else
    {
        LOG(TRACE) + "There is no pending app msg";
        m_appMessageResult = oResult;
        m_appMessageWait = true;
    }
}

void CIntentImplBase::startApplicationMessageNotifications(rho::apiGenerator::CMethodResult& oResult)
{
    common::CMutexLock lock(m_appMessageMutex);
    m_appMessageHandler = oResult;
    m_appMessageNotifications = true;
    for(Vector<Hashtable<String, String> >::const_iterator It = m_appMessageQueue.begin(); It != m_appMessageQueue.end(); ++It)
    {
        m_appMessageHandler.set(*It);
    }
    m_appMessageQueue.clear();
}

void CIntentImplBase::stopApplicationMessageNotifications(rho::apiGenerator::CMethodResult& oResult)
{
    common::CMutexLock lock(m_appMessageMutex);
    m_appMessageNotifications = false;
}

void CIntentImplBase::addApplicationMessage(const rho::String& appName, const rho::String& msg)
{
    common::CMutexLock lock(m_appMessageMutex);
    if(!m_appMessageWait && !m_appMessageNotifications)
    {
        LOG(TRACE) + "Push " + appName + " mesage to queue: " + msg;
        m_appMessageQueue.push_back(Hashtable<String, String>());
        m_appMessageQueue.back()["appName"] = appName;
        m_appMessageQueue.back()["message"] = msg;
    }
    else
    {
        Hashtable<String, String> res;
        res["appName"] = appName;
        res["message"] = msg;
        if(m_appMessageWait)
        {
            LOG(TRACE) + "Pass " + appName + " mesage to callback (once): " + msg;
            m_appMessageResult.set(res);
            m_appMessageWait = false;
        }

        if(m_appMessageNotifications)
        {
            LOG(TRACE) + "Pass " + appName + " mesage to callback (subscription): " + msg;
            m_appMessageHandler.set(res);
        }
    }
}

}
