#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"

#import <UIKit/UIKit.h>

#include "../../../shared/jxcoretau.h"

#include "common/RhodesApp.h"


extern void Init_Jxcore_API();



void Init_Jxcore_extension()
{
    jxcoretau_init();
    Init_Jxcore_API();
}
