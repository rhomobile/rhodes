#include "common/RhodesApp.h"


extern "C" void Init_Camera_extension();

extern "C" void Init_Mediacapture_extension()
{
#if defined(OS_WINCE) || defined(OS_WP8)
    Init_Camera_extension();
#endif
}
