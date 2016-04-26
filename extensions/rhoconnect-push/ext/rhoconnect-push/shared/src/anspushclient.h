
#ifndef ANSPUSHCLIENT_H_
#define ANSPUSHCLIENT_H_

//#include "common/push/IRhoPushClient.h"
#include "Push.h"

#include "logging/RhoLog.h"
#include "common/RhoStd.h"


namespace rho { namespace ans {

using namespace rho::common;

class AnsPushClient: public push::CPushClient
{
    class SyncLoginListener;
private:
    static const String s_Type;
    CMethodResult m_oResult;
    CMethodResult m_deviceIdResult;
public:
    AnsPushClient();
    virtual ~AnsPushClient() {}

    virtual const String& getId() const { return s_Type; }
    virtual void setDeviceId(const String& deviceId);
    virtual void getDeviceId(CMethodResult& result);
    virtual void startNotifications(CMethodResult& result);
    virtual void stopNotifications(CMethodResult& result);
    virtual bool callBack(const String& json);
};


}}

#endif /* ANSPUSHCLIENT_H_ */
