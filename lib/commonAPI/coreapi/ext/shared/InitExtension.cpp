#include "common/RhoPort.h"
extern "C" void Init_System();
extern "C" void Init_NetworkAccess();
extern "C" void Init_SQLite3();

extern "C" void Init_CoreAPI_Extension()
{
    Init_System();

#if defined(OS_WINDOWS_DESKTOP) || defined(OS_WINCE) || defined(OS_WP8) || defined(OS_MACOSX)

#ifndef OS_WP8
	Init_NetworkAccess();
#endif

#ifndef OS_MACOSX
    Init_SQLite3();
#endif

#endif
}
