
#import "SensorFactory.h"
#import "Sensor.h"
#import "SensorSingleton.h"
#import <CoreMotion/CoreMotion.h>

static BOOL ourIsAccelerometerAvailability = NO;
static BOOL ourIsMagnetometerAvailability = NO;


@interface SensorAvailabilityProcessor: NSObject {
    
}

-(void) initAvailabilityCommand;
-(void) initAvailability;
@end

@implementation SensorAvailabilityProcessor

-(void) initAvailabilityCommand {
    CMMotionManager* motionManager = [[CMMotionManager alloc] init];
    ourIsAccelerometerAvailability = motionManager.accelerometerAvailable;
    ourIsMagnetometerAvailability = motionManager.deviceMotionAvailable && ([CMMotionManager availableAttitudeReferenceFrames] & CMAttitudeReferenceFrameXArbitraryCorrectedZVertical);
    [motionManager release];
}

-(void) initAvailability {
    [self performSelectorOnMainThread:@selector(initAvailabilityCommand) withObject:nil waitUntilDone:YES];
}

@end



@implementation SensorFactory

-(id<ISensor>)createSensorByID:(NSString*)ID {
    Sensor* obj = nil;
    if ([SENSOR_TYPE_ACCELEROMETER isEqualToString:ID] && [SensorFactory isAccelerometerSupported]) {
        obj = [[AccelerometerSensor alloc] init];
    }
    else if ([SENSOR_TYPE_MAGNETOMETER isEqualToString:ID] && [SensorFactory isMagnetometerSupported]) {
        obj = [[MagnetometerSensor alloc] init];
    }
    else {
        //obj = [[Sensor alloc] init];
    }
    if (obj != nil) {
        [obj setProperty:@"ID" propertyValue:ID methodResult:nil];
    }
    return obj;
}

+(void) initAvailability {
    SensorAvailabilityProcessor* proc = [[SensorAvailabilityProcessor alloc] init];
    [proc initAvailability];
}

+(BOOL) isAccelerometerSupported {
    return ourIsAccelerometerAvailability;
}

+(BOOL) isMagnetometerSupported {
    return ourIsMagnetometerAvailability;
}



@end