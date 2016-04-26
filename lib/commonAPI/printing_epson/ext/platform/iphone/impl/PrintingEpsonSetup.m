#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_PrintingEpson_API();

void Init_PrintingEpson_extension()
{
    Init_PrintingEpson_API();
}