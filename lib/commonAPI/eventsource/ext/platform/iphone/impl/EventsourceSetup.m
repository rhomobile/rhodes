#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_Eventsource_API();

void Init_Eventsource_extension()
{
    Init_Eventsource_API();
}