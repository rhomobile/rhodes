#include "common/RhoPort.h"

extern "C" void Init_System();
extern "C" void Init_Network();
extern "C" void Init_SQLite3();
extern "C" void Init_Log();
extern "C" void Init_WebView();
extern "C" void Init_WebView_extension();
extern "C" void Init_Application();
extern "C" void Init_NativeToolbar();
extern "C" void Init_NativeToolbar_extension();
extern "C" void Init_NativeTabbar();
extern "C" void Init_NativeTabbar_extension();
extern "C" void Init_Navbar();
extern "C" void Init_Notification();
extern "C" void Init_RhoFile();
extern "C" void Init_NativeMenuBar();
extern "C" void Init_Led();
extern "C" void Init_Push();
extern "C" void Init_NewORM_extension();
extern "C" void Init_Intent();
extern "C" void Init_Config_extension();
extern "C" void Init_Timer_extension();

extern "C" void Init_CoreAPI_Extension()
{
    printf("[GTK shim] Init_CoreAPI_Extension impl\n");
    fflush(stdout);

    Init_System();
    Init_Application();

    Init_Network();
    Init_SQLite3();
    Init_NewORM_extension();
    Init_Log();

#if defined(OS_MACOSX) || defined(OS_WINDOWS_DESKTOP) || defined(OS_WINCE) || defined(OS_ANDROID) || defined(OS_SAILFISH) || defined(OS_LINUX)
    Init_WebView();
#elif defined(OS_UWP)
    Init_WebView_extension();
#endif

#if defined(OS_WINDOWS_DESKTOP)  || defined(OS_LINUX) || defined(OS_WINCE) || (defined(OS_MACOSX) && defined(RHODES_EMULATOR)) || defined(OS_ANDROID) || defined(OS_MACOSX)
    Init_NativeToolbar();
    Init_NativeTabbar();
#elif defined(OS_UWP)
    Init_NativeToolbar_extension();
    Init_NativeTabbar_extension();
#endif

#if defined(OS_MACOSX) || defined(RHODES_EMULATOR) || defined (OS_LINUX)
    Init_Navbar();
#endif

#if defined(OS_WINDOWS_DESKTOP)  || defined(OS_LINUX) || defined(RHODES_EMULATOR) || defined(OS_WINCE) || defined(OS_MACOSX) || defined(OS_ANDROID)
    Init_Notification();
#endif

#if defined(OS_MACOSX) || defined(OS_ANDROID) || defined(WINDOWS_PLATFORM)  || defined(OS_LINUX) || defined(OS_SAILFISH)
    Init_RhoFile();
#endif

#if defined(OS_WINDOWS_DESKTOP)  || defined(OS_LINUX) || defined(OS_WINCE) || defined(RHODES_EMULATOR)
    Init_NativeMenuBar();
#endif

#if defined(OS_WINCE) || defined(OS_ANDROID)
    /* Init_Led(); */
#endif

#if defined(OS_ANDROID) || defined(OS_WINCE) || defined(OS_MACOSX) || (defined(OS_WINDOWS_DESKTOP) && !defined(RHODES_EMULATOR))
    Init_Push();
#endif

#if defined(OS_ANDROID) || (defined(OS_MACOSX) && !defined(RHODES_EMULATOR)) || defined(OS_WINCE)
    Init_Intent();
#endif

#if defined(OS_ANDROID) || defined(OS_WINCE) || (defined(OS_MACOSX) && !defined(RHODES_EMULATOR)) || defined(OS_UWP)|| defined(OS_WINDOWS_DESKTOP) || defined(OS_SAILFISH) || defined(RHODES_EMULATOR)  || defined(OS_LINUX)
    Init_Timer_extension();
#endif

#if defined(OS_ANDROID) || defined(OS_UWP) || defined(OS_WINCE) || defined(OS_SAILFISH) || defined(OS_LINUX) || (defined(OS_MACOSX) && !defined(RHODES_EMULATOR)) || defined(OS_WINDOWS_DESKTOP)
    Init_Config_extension();
#endif
}
