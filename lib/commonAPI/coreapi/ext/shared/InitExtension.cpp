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

extern "C" void Init_CoreAPI_Extension()
{
    Init_System();
    Init_Application();

	Init_Network();
    Init_SQLite3();
    Init_Log();
#if defined(OS_MACOSX) || defined(OS_WINDOWS_DESKTOP) || defined(OS_WINCE) || defined(OS_ANDROID)
    Init_WebView();
#elif defined(OS_WP8)
    Init_WebView_extension();
#endif

#if defined(OS_WINDOWS_DESKTOP) || defined(OS_WINCE) || (defined(OS_MACOSX) && defined(RHODES_EMULATOR)) || defined(OS_ANDROID) || defined(OS_MACOSX)
    Init_NativeToolbar();
    Init_NativeTabbar();
#elif defined(OS_WP8)
    Init_NativeToolbar_extension();
    Init_NativeTabbar_extension();
#endif

#if (defined(OS_MACOSX) && !defined(RHODES_EMULATOR))
    Init_Navbar();
#endif

#if defined(OS_WINDOWS_DESKTOP) && !(defined(OS_MACOSX) && defined(RHODES_EMULATOR))
    Init_Notification();
#endif

#if defined(OS_MACOSX) || defined(OS_ANDROID)
    Init_RhoFile();
#endif

#if defined(OS_WINDOWS_DESKTOP) || defined(OS_WINCE)
    Init_NativeMenuBar();
#endif
}
