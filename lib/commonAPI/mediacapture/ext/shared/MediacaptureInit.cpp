#include "common/RhodesApp.h"

extern "C" void Init_Camera_API();
extern "C" void Init_Videocapture_API();
extern "C" void Init_Videocapture();

extern "C" void Init_Mediacapture_extension()
{
#if !defined(OS_ANDROID)
    Init_Camera_API();
#ifndef RHO_NO_RUBY_API
    RHODESAPP().getExtManager().requireRubyFile("RhoCameraApi");
#endif
#endif
	Init_Videocapture();
}
