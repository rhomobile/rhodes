#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_AudioCapture_API();

void Init_AudioCapture_extension()
{
    Init_AudioCapture_API();
}