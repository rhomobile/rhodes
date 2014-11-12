#include "common/RhodesApp.h"


extern "C" void Init_Camera_extension();

extern "C" void Init_Mediacapture_extension()
{
#if defined(OS_WINCE)
    Init_Camera_extension();
#endif
}
