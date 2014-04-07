
#import "ISensor.h"
#import "SensorFactoryBase.h"


@interface SensorFactory : SensorFactoryBase<ISensorFactory> {
}

-(id<ISensor>)createSensorByID:(NSString*)ID;

+(void) initAvailability;


+(BOOL) isAccelerometerSupported;
+(BOOL) isMagnetometerSupported;

@end