#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_<%= $cur_module.name %>_API();

void Init_<%= $cur_module.name %>_extension()
{
    Init_<%= $cur_module.name %>_API();
}
