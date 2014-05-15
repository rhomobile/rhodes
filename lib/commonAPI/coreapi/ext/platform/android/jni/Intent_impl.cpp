#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Intent_impl"

extern "C" void Init_Intent_API(void);

extern "C" void Init_Intent(void)
{
    RAWTRACE(__FUNCTION__);
    RAWTRACE("Initializing API");
    Init_Intent_API();
    RAWTRACE("Init_Intent succeeded");
}

