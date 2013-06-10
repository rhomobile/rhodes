
#import "ISensor.h"
#import "SensorFactory.h"

static SensorFactoryBase* ourSensorFactory = nil;

@implementation SensorFactorySingleton

+(id<ISensorFactory>) getSensorFactoryInstance {
    if (ourSensorFactory == nil) {
        ourSensorFactory = [[SensorFactory alloc] init];
    }
    return ourSensorFactory;
}

@end