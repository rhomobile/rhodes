#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_<%= namecamelcase %>_API();

void Init_<%= namecamelcase %>_extension()
{
    Init_<%= namecamelcase %>_API();
}