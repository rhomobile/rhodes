#include "common/RhoPort.h"
extern "C" void Init_System();
extern "C" void Init_Network();
extern "C" void Init_SQLite3();
extern "C" void Init_Log();
extern "C" void Init_WebView();

extern "C" void Init_CoreAPI_Extension()
{
    Init_System();
	Init_Network();
    Init_SQLite3();
#if defined( OS_WINDOWS_DESKTOP ) || defined(OS_WINCE) || defined(OS_WP8)
    Init_Log();
#endif
#if defined(OS_MACOSX)
    Init_WebView();
#endif
}
