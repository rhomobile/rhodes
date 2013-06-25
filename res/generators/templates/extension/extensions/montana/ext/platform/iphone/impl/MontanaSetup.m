#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_<%= name.camel_case %>_API();

void Init_<%= name.camel_case %>_extension()
{
    Init_<%= name.camel_case %>_API();
}