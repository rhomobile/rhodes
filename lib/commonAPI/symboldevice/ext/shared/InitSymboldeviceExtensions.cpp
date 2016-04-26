#include "common/RhoPort.h"
extern "C" void Init_Device_extension();
extern "C" void Init_DeviceMemory_extension();

extern "C" void Init_Symboldevice_extension()
{
#if defined(OS_WINCE) || defined(OS_ANDROID)
    Init_Device_extension();
#endif
#if defined(OS_ANDROID)
    Init_DeviceMemory_extension();
#endif
}
