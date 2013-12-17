#include <Windows.h>

#include "RhoPort.h"
#include "RhoStd.h"
#include "common/RhoFilePath.h"
#include "common/StringConverter.h"

#include "Intents.h"

extern "C" const wchar_t* rho_wmimpl_get_window_nameW();

namespace
{
HANDLE g_intentEvent;
}

namespace rho
{

void waitIntentEvent(const rho::String& appName)
{
    common::CFilePath oPath(appName);    
    rho::String appNamePath = oPath.getBaseName();
    appNamePath.resize(appNamePath.length() - 4);
    rho::String eventName = appNamePath + ".MainWindow";
    rho::StringW eventNameW = common::convertToStringW(eventName) + EVENTNAME_PREFIX;

    HANDLE anotherEvent = CreateEvent( NULL, false, false, eventNameW.c_str());

    if (anotherEvent != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
    {
        WaitForSingleObject(anotherEvent, INFINITE);
    }

    CloseHandle(anotherEvent);
    return;
}

bool createIntentEvent()
{
    rho::StringW eventName = rho_wmimpl_get_window_nameW();
    eventName += EVENTNAME_PREFIX;

    g_intentEvent = CreateEvent( NULL, false, false, eventName.c_str() );

    if (g_intentEvent != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return false;
    }

    return true;
}

//
void fireIntentEvent()
{
    SetEvent(g_intentEvent);
}

} //end of rho