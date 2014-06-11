#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_ConnectionChecking_API();

void Init_ConnectionChecking_extension()
{
    Init_ConnectionChecking_API();
}