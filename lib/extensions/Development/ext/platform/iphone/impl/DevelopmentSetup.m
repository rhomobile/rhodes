#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_Development_API();

extern void init_Development_HTTP_Server();

extern void Init_UpdateBundle();

static BOOL already_initialised = NO;

void Init_Development_extension()
{
    if (!already_initialised) {
        Init_Development_API();
        init_Development_HTTP_Server();
        Init_UpdateBundle();
    }
    already_initialised = YES;
}