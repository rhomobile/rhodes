#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_Camera_API();

void Init_Mediacapture_extension()
{
    Init_Camera_API();
#ifndef RHO_NO_RUBY_API
    rb_require("RhoCameraApi");
#endif
}