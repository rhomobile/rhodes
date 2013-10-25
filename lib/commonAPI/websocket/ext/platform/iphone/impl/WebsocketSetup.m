#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_Websocket_API();

void Init_Websocket_extension()
{
    Init_Websocket_API();
}