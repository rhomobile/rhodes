#import <Foundation/Foundation.h>

extern void Init_Signature_API();
extern unsigned long rb_require(const char *fname);
extern int  rho_ruby_is_started();

void Init_Signature_extension()
{
    Init_Signature_API();
#ifndef RHO_NO_RUBY_API
    if (rho_ruby_is_started())
    {
        rb_require("RhoSignatureApi");
    }
#endif
}