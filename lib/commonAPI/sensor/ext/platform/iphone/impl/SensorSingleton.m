
#import "SensorSingleton.h"
#import "SensorFactory.h"

@implementation SensorSingleton



-(void) makeSensorByType:(NSString*)type methodResult:(id<IMethodResult>)methodResult {
    if ([SENSOR_TYPE_ACCELEROMETER isEqualToString:type] && [SensorFactory isAccelerometerSupported]) {
        [methodResult setResult:SENSOR_TYPE_ACCELEROMETER];
    }
    if ([SENSOR_TYPE_MAGNETOMETER isEqualToString:type] && [SensorFactory isMagnetometerSupported]) {
        [methodResult setResult:SENSOR_TYPE_MAGNETOMETER];
    }
}

-(void) enumerate:(id<IMethodResult>)methodResult {
    NSMutableArray* list = [NSMutableArray arrayWithCapacity:2];
    if ([SensorFactory isAccelerometerSupported]) {
        [list addObject:SENSOR_TYPE_ACCELEROMETER];
    }
    if ([SensorFactory isMagnetometerSupported]) {
        [list addObject:SENSOR_TYPE_MAGNETOMETER];
    }
    [methodResult setResult:list];
}




@end