#import <Foundation/Foundation.h>


#ifndef RHO_NO_RUBY_API
extern unsigned long rb_require(const char *fname);
#endif

extern void Init_NativeTabbar_API();

void Init_NativeTabbar()
{
    Init_NativeTabbar_API();
#ifndef RHO_NO_RUBY_API
    rb_require("RhoNativeTabbarApi");
#endif
}