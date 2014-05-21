#include "common/RhodesApp.h"

#if !defined(OS_WP8) && !defined(OS_WINRT)
extern "C" void Init_Camera_API();
extern "C" void Init_Videocapture();
#else
extern "C" void Init_Camera_extension();
#endif

extern "C" void Init_Mediacapture_extension()
{
#if !defined(OS_WP8) && !defined(OS_WINRT)
    Init_Camera_API();
#ifndef RHO_NO_RUBY_API
    RHODESAPP().getExtManager().requireRubyFile("RhoCameraApi");
#endif
    LOG(INFO) + __FUNCTION__ + " Loading Videocapture extension";
    Init_Videocapture();
#else
    Init_Camera_extension();
#endif
}
