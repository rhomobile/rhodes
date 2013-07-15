#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern "C" void Init_applePush_API();
extern "C" void Init_applePushClient();

extern "C" void Init_applePush_extension()
{
    Init_applePushClient();
    Init_applePush_API();
}