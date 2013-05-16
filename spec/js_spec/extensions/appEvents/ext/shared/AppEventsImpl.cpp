#include "generated/cpp/AppEventsBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoFile.h"

#ifdef RHODES_EMULATOR
#define RHO_APPS_DIR ""
#else
#define RHO_APPS_DIR "apps/"
#endif

namespace rho {

using namespace apiGenerator;
using namespace common;

class CAppEventsImpl: public CAppEventsSingletonBase
{
public:

    CAppEventsImpl(): CAppEventsSingletonBase(){}

    virtual void simulateEvent( const rho::String& applicationEvent, rho::apiGenerator::CMethodResult& oResult)
    {
        IApplicationEventReceiver* rcvr = RHODESAPP().getApplicationEventReceiver();
        if (applicationEvent==rho::IAppEvents::APP_EVENT_ACTIVATED)
        {
            rcvr->onAppStateChange(rho::common::applicationStateActivated);
        } else
        if (applicationEvent==rho::IAppEvents::APP_EVENT_DEACTIVATED)
        {
            rcvr->onAppStateChange(rho::common::applicationStateDeactivated);
        } else
        if (applicationEvent==rho::IAppEvents::APP_EVENT_UICREATED)
        {
            rcvr->onUIStateChange(rho::common::UIStateCreated);
        } else
        if (applicationEvent==rho::IAppEvents::APP_EVENT_UICREATED)
        {
            rcvr->onUIStateChange(rho::common::UIStateDestroyed);
        };
    }
};

////////////////////////////////////////////////////////////////////////

class CAppEventsFactory: public CAppEventsFactoryBase
{
public:
    ~CAppEventsFactory(){}

    IAppEventsSingleton* createModuleSingleton()
    { 
        return new CAppEventsImpl();
    }
};

}

extern "C" void Init_AppEvents_extension ()
{
    rho::CAppEventsFactory::setInstance( new rho::CAppEventsFactory() );
    rho::Init_AppEvents_API();
}
