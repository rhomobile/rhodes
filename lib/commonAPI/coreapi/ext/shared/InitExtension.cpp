#include "common/RhoPort.h"
extern "C" void Init_System();
extern "C" void Init_Network();
extern "C" void Init_SQLite3();
extern "C" void Init_Log();
extern "C" void Init_WebView();
extern "C" void Init_Application();
extern "C" void Init_NativeToolbar();
extern "C" void Init_NativeTabbar();

extern "C" void Init_CoreAPI_Extension()
{
    Init_System();
    Init_Application();

	Init_Network();
    Init_SQLite3();
    Init_Log();
#if defined(OS_MACOSX) || defined(OS_WINDOWS_DESKTOP) || defined(OS_WINCE) || defined(OS_ANDROID)
    Init_WebView();
#endif

#if defined(OS_WINDOWS_DESKTOP) || defined(OS_WINCE)
    Init_NativeToolbar();
    Init_NativeTabbar();
#endif
}
