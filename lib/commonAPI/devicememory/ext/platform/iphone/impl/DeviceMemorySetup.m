#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_DeviceMemory_API();

void Init_DeviceMemory_extension()
{
    Init_DeviceMemory_API();
}