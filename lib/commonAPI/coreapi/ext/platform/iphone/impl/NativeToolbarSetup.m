#import <Foundation/Foundation.h>

#ifndef RHO_NO_RUBY_API
extern unsigned long rb_require(const char *fname);
#endif

extern void Init_NativeToolbar_API();

void Init_NativeToolbar()
{
    Init_NativeToolbar_API();
#ifndef RHO_NO_RUBY_API
    rb_require("RhoNativeToolbarApi");
#endif
}