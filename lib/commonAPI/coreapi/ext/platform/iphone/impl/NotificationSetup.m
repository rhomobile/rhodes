#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_Notification_API();

#ifndef RHO_NO_RUBY_API
extern unsigned long rb_require(const char *fname);
#endif

void Init_Notification()
{
    Init_Notification_API();
#ifndef RHO_NO_RUBY_API
    rb_require("RhoNotificationApi");
#endif
}