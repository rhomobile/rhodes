#include "common/RhodesApp.h"

#if !defined(OS_WP8) && !defined(OS_WINRT)
extern "C" void Init_Camera_API();
extern "C" void Init_Videocapture_API();
extern "C" void Init_camera_extension();
extern "C" void Init_Videocapture_extension();
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
	Init_camera_extension();
	Init_Videocapture_extension();
	LOG(INFO) + __FUNCTION__ + " Loading Videocapture extension";
#else
	Init_Camera_extension();
#endif
}