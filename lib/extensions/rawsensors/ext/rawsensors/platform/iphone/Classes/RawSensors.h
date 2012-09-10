
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <CoreMotion/CoreMotion.h>

#include "ruby/ext/rho/rhoruby.h"

#define RAW_SENSORS_SERVICE_ALL @"all"
#define RAW_SENSORS_SERVICE_ACCELEROMETER @"accelerometer"
#define RAW_SENSORS_SERVICE_MAGNETOMETER @"magnetometer"
#define RAW_SENSORS_UPDATE_PERIOD @"minimumInterval"
#define RAW_SENSORS_CALLBACK_URL @"sensorEvent"



@interface RhoRawSensors: NSObject {
    CMMotionManager *motionManager;
    BOOL enableAccelerometer;
    BOOL enableMagnetometer;
    BOOL enableAll;
    
    NSTimer* timer;
    
    NSString* callbackURL;
    
    double acceleration_x;
    double acceleration_y;
    double acceleration_z;

    double magnetometer_x;
    double magnetometer_y;
    double magnetometer_z;
    
    
    int updatePeriod;
}


+ (RhoRawSensors*) getInstance;

- (VALUE) isAvailable:(NSString*)service_name;

- (VALUE) getProperty:(NSString*)property_name;

- (void) setProperty:(NSString*)property_name value:(NSString*)value;

- (void) getSensorData;

- (void) startPlatformUpdates;

- (void) stopPlatformUpdates;

- (void) onAccelerometerUpdate:(CMAccelerometerData*)accData error:(NSError*)error;

- (void) onMagnetometerUpdate:(CMMagnetometerData*)magData error:(NSError*)error;

- (void) onMotionUpdate:(CMDeviceMotion*)motion error:(NSError*)error;

- (void) fireCallback;

- (void) fireErrorCallback:(NSString*)message;

- (void) onTimerFired:(NSTimer*)theTimer;


@end