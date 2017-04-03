#include "common/RhoPort.h"
extern "C" void Init_SignalIndicators_extension();
extern "C" void Init_Keystate_extension();
extern "C" void Init_Battery_extension();

extern "C" void Init_Indicators_extension()
{
#if defined(OS_WINCE) || defined(OS_WP8) || defined(OS_UWP) || defined(OS_MACOSX) || defined(OS_ANDROID)
    Init_Battery_extension();
#endif
#if defined(OS_WINCE) || defined(OS_ANDROID)
	Init_SignalIndicators_extension();
#endif
#if defined(OS_WINCE) 
	Init_Keystate_extension();
#endif

}
