#import <Foundation/Foundation.h>
#include "common/app_build_capabilities.h"
#include "SensorFactory.h"

extern void Init_Sensor_API();

void Init_Sensor_extension()
{
    Init_Sensor_API();
    [SensorFactory initAvailability];
}