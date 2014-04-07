
#import "ISensor.h"
#import "SensorBase.h"
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <CoreMotion/CoreMotion.h>

@interface Sensor : SensorBase<ISensor> {
}

-(void) start:(id<IMethodResult>)methodResult;
-(void) readData:(id<IMethodResult>)methodResult;
-(void) stop:(id<IMethodResult>)methodResult;



@end


@interface AccelerometerSensor : Sensor<ISensor> {
    CMMotionManager *motionManager;
    NSTimer* timer;
    id<IMethodResult> callback;
    double acceleration_x;
    double acceleration_y;
    double acceleration_z;
    int updatePeriod;
    BOOL ready;
    
}


-(id) init;

-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue;


@end




@interface MagnetometerSensor : Sensor<ISensor> {
    CMMotionManager *motionManager;
    NSTimer* timer;
    id<IMethodResult> callback;
    double magnetometer_x;
    double magnetometer_y;
    double magnetometer_z;
    int updatePeriod;
    BOOL ready;
    
}

-(id) init;

@end