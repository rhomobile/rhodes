#include "common/RhoPort.h"
extern "C" void Init_System();
extern "C" void Init_NetworkAccess();
extern "C" void Init_SQLite3();

extern "C" void Init_CoreAPI_Extension()
{
    Init_System();
#if defined(WINDOWS_PLATFORM)
    Init_NetworkAccess();
    Init_SQLite3();
#endif
}
