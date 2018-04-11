#include "common/app_build_capabilities.h"


extern void Init_FCMPushClient();


void Init_FCMPush_extension()
{
    Init_FCMPushClient();
}
