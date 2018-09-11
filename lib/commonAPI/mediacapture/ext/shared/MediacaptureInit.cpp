#include "common/RhodesApp.h"

#if !defined(OS_WP8) && !defined(OS_WINDOWS_DESKTOP) && !defined(OS_UWP) && !defined(OS_LINUX)
extern "C" void Init_Camera_API();
extern "C" void Init_Videocapture();
#endif
#if defined(OS_WINCE) || defined(OS_WP8) || defined(OS_UWP) || defined(OS_WINDOWS_DESKTOP) || defined(OS_LINUX)
extern "C" void Init_Camera_extension();
#endif
extern "C" void Init_Mediacapture_extension()
{
#if !defined(OS_WP8) && !defined(OS_WINDOWS_DESKTOP) && !defined(OS_UWP) && !defined(OS_LINUX)
    Init_Camera_API();
#ifndef RHO_NO_RUBY_API
    RHODESAPP().getExtManager().requireRubyFile("RhoCameraApi");
#endif
    LOG(INFO) + __FUNCTION__ + " Loading Videocapture extension";
    Init_Videocapture(); 
#endif

#if defined(OS_WINCE) || defined(OS_WP8) || defined(OS_UWP) || defined(OS_WINDOWS_DESKTOP) || defined(OS_LINUX)
    Init_Camera_extension();
#endif
}
