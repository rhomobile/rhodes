#import <Foundation/Foundation.h>

extern void Init_Camera_API();

void Init_Mediacapture_extension()
{
    Init_Camera_API();
#ifndef RHO_NO_RUBY_API
    rb_require("RhoCameraApi");
#endif
}