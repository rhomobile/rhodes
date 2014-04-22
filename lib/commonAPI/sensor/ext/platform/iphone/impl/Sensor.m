
#import "Sensor.h"


@implementation Sensor



-(void) start:(id<IMethodResult>)methodResult {

}

-(void) readData:(id<IMethodResult>)methodResult {

}

-(void) stop:(id<IMethodResult>)methodResult {

}




@end




@implementation AccelerometerSensor

-(id) init {
    self = [super init];
    motionManager = nil;
    timer = nil;
    callback = nil;
    acceleration_x = 0;
    acceleration_y = 0;
    acceleration_z = 0;
    updatePeriod = 200;
    ready = NO;
    [self setProperty:@"type" propertyValue:SENSOR_TYPE_ACCELEROMETER methodResult:nil];
    [self internalSetStatus:SENSOR_STATUS_READY isReady:NO];
    return self;
}

-(void) internalSetStatus:(NSString*)status isReady:(BOOL)isReady; {
    [mProperties setObject:status forKey:@"status"];
    ready = isReady;
}

-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue methodResult:(id<IMethodResult>)methodResult {
    if ([@"status" isEqualToString:propertyName] || ([@"type" isEqualToString:propertyName] && ![SENSOR_TYPE_ACCELEROMETER isEqualToString:propertyValue])) {
        return;
    }
    if ([@"minimumGap" isEqualToString:propertyName]) {
        int gap = [propertyValue intValue];
        if (gap < 200) {
            gap = 200;
        }
        updatePeriod = gap;
        propertyValue = [NSString stringWithFormat:@"%@", [NSNumber numberWithInt:gap]];
    }
    [super setProperty:propertyName propertyValue:propertyValue methodResult:methodResult];
}

-(void) postResultTo:(id<IMethodResult>)methodResult {
    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:4];
    [dict setObject:ready?@"ok":@"error" forKey:HK_STATUS];
    [dict setObject:[NSNumber numberWithFloat:(float)acceleration_x] forKey:HK_ACCELEROMETER_X];
    [dict setObject:[NSNumber numberWithFloat:(float)acceleration_y] forKey:HK_ACCELEROMETER_Y];
    [dict setObject:[NSNumber numberWithFloat:(float)acceleration_z] forKey:HK_ACCELEROMETER_Z];
    [methodResult setResult:dict];
}

- (void) onTimerFired:(NSTimer*)theTimer {
    if (callback != nil) {
        [self postResultTo:callback];
    }
}

- (void) onAccelerometerUpdate:(CMAccelerometerData*)accData error:(NSError*)error {
    acceleration_x = accData.acceleration.x * 9.81;
    acceleration_y = accData.acceleration.y * 9.81;
    acceleration_z = accData.acceleration.z * 9.81;
    ready = YES;
}

- (void) startCommand {
    motionManager = [[CMMotionManager alloc] init];
    motionManager.gyroUpdateInterval = 1.0/20.0;
    
    [motionManager startAccelerometerUpdatesToQueue:[NSOperationQueue currentQueue]
                                        withHandler: ^(CMAccelerometerData* accData, NSError *error)
     {
         [self onAccelerometerUpdate:accData error:error];
     }];
    if (timer != nil) {
        [timer invalidate];
        timer = nil;
    }
    timer = [NSTimer scheduledTimerWithTimeInterval:(((float)updatePeriod)/1000.0) target:self selector:@selector(onTimerFired:) userInfo:nil repeats:YES];
}


-(void) start:(id<IMethodResult>)methodResult {
    [self stop:methodResult];
    if (callback != nil) {
        [callback release];
    }
    callback = [methodResult retain];
    [self performSelectorOnMainThread:@selector(startCommand) withObject:nil waitUntilDone:NO];
    [self internalSetStatus:SENSOR_STATUS_STARTED isReady:NO];
}

-(void) readData:(id<IMethodResult>)methodResult {
    [self postResultTo:methodResult];
}


- (void) stopCommand {
    if (timer != nil) {
        [timer invalidate];
        timer = nil;
    }
    [motionManager stopAccelerometerUpdates];
    if (motionManager != nil) {
        [motionManager release];
        motionManager = nil;
    }
}

-(void) stop:(id<IMethodResult>)methodResult {
    acceleration_x = 0;
    acceleration_y = 0;
    acceleration_z = 0;
    [self performSelectorOnMainThread:@selector(stopCommand) withObject:nil waitUntilDone:NO];
    [self internalSetStatus:SENSOR_STATUS_READY isReady:NO];
}


-(void)dealloc {
    [self stop:nil];
    [super dealloc];
}

@end



@implementation MagnetometerSensor

-(id) init {
    self = [super init];
    motionManager = nil;
    timer = nil;
    callback = nil;
    magnetometer_x = 0;
    magnetometer_y = 0;
    magnetometer_z = 0;
    updatePeriod = 200;
    ready = NO;
    [self setProperty:@"type" propertyValue:SENSOR_TYPE_MAGNETOMETER methodResult:nil];
    [self internalSetStatus:SENSOR_STATUS_READY isReady:NO];
    return self;
}

-(void) internalSetStatus:(NSString*)status isReady:(BOOL)isReady; {
    [mProperties setObject:status forKey:@"status"];
    ready = isReady;
}

-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue methodResult:(id<IMethodResult>)methodResult {
    if ([@"status" isEqualToString:propertyName] || ([@"type" isEqualToString:propertyName] && ![SENSOR_TYPE_MAGNETOMETER isEqualToString:propertyValue])) {
        return;
    }
    if ([@"minimumGap" isEqualToString:propertyName]) {
        int gap = [propertyValue intValue];
        if (gap < 200) {
            gap = 200;
        }
        updatePeriod = gap;
        propertyValue = [NSString stringWithFormat:@"%@", [NSNumber numberWithInt:gap]];
    }
    [super setProperty:propertyName propertyValue:propertyValue methodResult:methodResult];
}

-(void) postResultTo:(id<IMethodResult>)methodResult {
    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:4];
    [dict setObject:ready?@"ok":@"error" forKey:HK_STATUS];
    [dict setObject:[NSNumber numberWithFloat:(float)magnetometer_x] forKey:HK_MAGNETOMETER_X];
    [dict setObject:[NSNumber numberWithFloat:(float)magnetometer_y] forKey:HK_MAGNETOMETER_Y];
    [dict setObject:[NSNumber numberWithFloat:(float)magnetometer_z] forKey:HK_MAGNETOMETER_Z];
    [methodResult setResult:dict];
}

- (void) onTimerFired:(NSTimer*)theTimer {
    if (callback != nil) {
        [self postResultTo:callback];
    }
}

- (void) onMotionUpdate:(CMDeviceMotion*)motion error:(NSError*)error {
    if (motionManager != nil) {
        magnetometer_x = motionManager.deviceMotion.magneticField.field.x;
        magnetometer_y = motionManager.deviceMotion.magneticField.field.y;
        magnetometer_z = motionManager.deviceMotion.magneticField.field.z;
    }
    ready = YES;
}

- (void) startCommand {
    motionManager = [[CMMotionManager alloc] init];
    motionManager.gyroUpdateInterval = 1.0/20.0;
    
    [motionManager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXArbitraryCorrectedZVertical toQueue:[NSOperationQueue currentQueue] withHandler: ^(CMDeviceMotion *motion, NSError *error)
     {
         [self onMotionUpdate:motion error:error];
     }];
    if (timer != nil) {
        [timer invalidate];
        timer = nil;
    }
    timer = [NSTimer scheduledTimerWithTimeInterval:(((float)updatePeriod)/1000.0) target:self selector:@selector(onTimerFired:) userInfo:nil repeats:YES];
}


-(void) start:(id<IMethodResult>)methodResult {
    [self stop:methodResult];
    if (callback != nil) {
        [callback release];
    }
    callback = [methodResult retain];
    [self performSelectorOnMainThread:@selector(startCommand) withObject:nil waitUntilDone:NO];
    [self internalSetStatus:SENSOR_STATUS_STARTED isReady:NO];
}

-(void) readData:(id<IMethodResult>)methodResult {
    [self postResultTo:methodResult];
}


- (void) stopCommand {
    if (timer != nil) {
        [timer invalidate];
        timer = nil;
    }
    [motionManager stopAccelerometerUpdates];
    if (motionManager != nil) {
        [motionManager release];
        motionManager = nil;
    }
}

-(void) stop:(id<IMethodResult>)methodResult {
    magnetometer_x = 0;
    magnetometer_y = 0;
    magnetometer_z = 0;
    [self performSelectorOnMainThread:@selector(stopCommand) withObject:nil waitUntilDone:NO];
    [self internalSetStatus:SENSOR_STATUS_READY isReady:NO];
}


-(void)dealloc {
    [self stop:nil];
    [super dealloc];
}
@end