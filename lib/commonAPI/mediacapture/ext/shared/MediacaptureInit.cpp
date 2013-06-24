#include "common/RhodesApp.h"

extern "C" void Init_Camera_API();
extern "C" void Init_Videocapture_API();
extern "C" void Init_Videocapture_extension();

extern "C" void Init_Mediacapture_extension()
{
    Init_Camera_API();
#ifndef RHO_NO_RUBY_API
    RHODESAPP().getExtManager().requireRubyFile("RhoCameraApi");
#endif
	Init_Videocapture_extension();
	LOG(INFO) + __FUNCTION__ + " Loading Videocapture extension";
}