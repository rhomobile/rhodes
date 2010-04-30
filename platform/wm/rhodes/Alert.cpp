#include "stdafx.h"

#if defined(_WIN32_WCE)
#include <soundfile.h>
#endif

#include <common/RhodesApp.h>

#include "Alert.h"
#include "MainWindow.h"
#include "Vibrate.h"

extern "C" HWND getMainWnd();

IMPLEMENT_LOGCLASS(CAlert, "Alert");

void CAlert::showPopup(String message)
{
    HWND main_wnd = getMainWnd();
	::PostMessage(main_wnd, WM_ALERT_SHOWPOPUP, 0,
                  (LPARAM )strdup(message.c_str()));
}

#if defined(_WIN32_WCE)
void CAlert::vibrate()
{
    CVibrate::getCVibrate().toggle();
}

void CAlert::playFile(String fileName)
{
    rho::String path = RHODESAPP().getRhoRootPath() + "apps" + fileName;
    HSOUND hSound;
    
    rho::String::size_type pos = 0;
    while ( (pos = path.find('/', pos)) != rho::String::npos ) {
        path.replace( pos, 1, "\\");
        pos++;
    }

    USES_CONVERSION;
    //SndPlaySync(A2T(path.c_str()),  SND_PLAY_IGNOREUSERSETTINGS);
    HRESULT hr = SndOpen(A2T(path.c_str()), &hSound);
    hr = SndPlayAsync (hSound, 0);
      
    if (hr != S_OK) {
        LOG(WARNING) + "OnAlertPlayFile: failed to play file"; 
    }
    
    WaitForSingleObject(hSound, INFINITE);
                        
    hr = SndClose(hSound);
    SndStop(SND_SCOPE_PROCESS, NULL);
}

#endif //_WIN32_WCE

extern "C" void alert_show_popup(char* message) {
    CAlert::showPopup(message);
}

extern "C" void alert_vibrate(void*) {
#if defined(_WIN32_WCE)
    CAlert::vibrate();
#endif
}

extern "C" void alert_play_file(char* file_name, ...) {
#if defined(_WIN32_WCE)
    CAlert::playFile(file_name);
#endif
}

extern "C" void alert_hide_popup()
{
}