#include "common/RhoPort.h"
extern "C" void Init_System();
extern "C" void Init_NetworkAccess();
extern "C" void Init_SQLite3();

extern "C" void Init_CoreAPI_Extension()
{
    Init_System();
#if defined(OS_WINDOWS_DESKTOP) || defined(OS_WINCE)
    Init_NetworkAccess();
    Init_SQLite3();
#endif
}
