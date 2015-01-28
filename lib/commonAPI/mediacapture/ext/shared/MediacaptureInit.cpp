#include "common/RhodesApp.h"

#ifndef OS_WP8
extern "C" void Init_Camera_API();
extern "C" void Init_Videocapture();
#else
extern "C" void Init_Camera_extension();
#endif

extern "C" void Init_Mediacapture_extension()
{
#ifndef OS_WP8
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
