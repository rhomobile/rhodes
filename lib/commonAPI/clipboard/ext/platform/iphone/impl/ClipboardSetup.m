#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_Clipboard_API();

void Init_Clipboard_extension()
{
    Init_Clipboard_API();
}