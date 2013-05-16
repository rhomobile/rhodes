#include "generated/cpp/AppEventsBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoFile.h"

#include <stdio.h>

extern "C" void rho_app_events_simulate_event(const char* applicationEvent)
{
    rho::common::IApplicationEventReceiver* rcvr = RHODESAPP().getApplicationEventReceiver();
    rho::String appEvt(applicationEvent);
    if (appEvt=="Activated")
    {
        rcvr->onAppStateChange(rho::common::applicationStateActivated);
    } else
    if (appEvt=="Deactivated")
    {
        rcvr->onAppStateChange(rho::common::applicationStateDeactivated);
    } else
    if (appEvt=="UICreated")
    {
        rcvr->onUIStateChange(rho::common::UIStateCreated);
    } else
    if (appEvt=="UIDestroyed")
    {
        rcvr->onUIStateChange(rho::common::UIStateDestroyed);
    };
}

extern "C" void rho_app_add_conflict_int(const char* name, int savedValue, int newValue)
{
    RHOCONF().setInt(name, savedValue, false);
    RHOCONF().setInt(name, newValue, true);
}

extern "C" void rho_app_add_conflict_str(const char* name, const char* savedValue, const char* newValue)
{
    RHOCONF().setString(name, savedValue, false);
    RHOCONF().setString(name, newValue, true);
}

extern "C" void rho_app_events_simulate_conflict()
{
    using namespace rho;
    
    rho::common::IApplicationEventReceiver* rcvr = RHODESAPP().getApplicationEventReceiver();
    
    // reset timestamp
    RHOCONF().setString("rho_conf_timestamp", "", true);
    
    RHOCONF().loadFromFile();
    
    HashtablePtr<String,Vector<String>* >& mapConflicts = RHOCONF().getConflicts();

    rcvr->onReinstallConfigUpdate(mapConflicts);
}