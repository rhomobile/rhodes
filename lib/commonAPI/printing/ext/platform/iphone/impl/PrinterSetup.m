#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

extern void Init_Printer_API();

void Init_Printing_extension()
{
    Init_Printer_API();
}