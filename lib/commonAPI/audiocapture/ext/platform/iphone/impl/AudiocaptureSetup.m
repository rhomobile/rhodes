#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_Audiocapture_API();

void Init_Audiocapture_extension()
{
    Init_Audiocapture_API();
}