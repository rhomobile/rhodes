
#import "RawSensors.h"

#include "ruby/ext/rho/rhoruby.h"
#import "common/RhodesApp.h"


static RhoRawSensors* ourRhoSensors = nil;

@implementation RhoRawSensors

+ (RhoRawSensors*) getInstance {
    if (ourRhoSensors == nil) {
        ourRhoSensors = [[RhoRawSensors alloc] init];
    }
    return ourRhoSensors;
}

- (void) initMotionManagerCommand {
    motionManager = [[CMMotionManager alloc] init];
}

- (id) init {
    [super init];
    
     [self performSelectorOnMainThread:@selector(initMotionManagerCommand) withObject:nil waitUntilDone:YES];
    
    enableAccelerometer = NO;
    enableMagnetometer = NO;
    enableAll = NO;
    
    timer = nil;
    
    acceleration_x = 0;
    acceleration_y = 0;
    acceleration_z = 0;
    
    magnetometer_x = 0;
    magnetometer_y = 0;
    magnetometer_z = 0;
    
    callbackURL = nil;
    
    
    updatePeriod = 1000;
    
    
    return self;
}


- (VALUE) isAvailable:(NSString*)service_name {
    if ([RAW_SENSORS_SERVICE_ACCELEROMETER compare:service_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if (motionManager.accelerometerAvailable) {
            return rho_ruby_create_boolean(1);
        }
    }
    if ([RAW_SENSORS_SERVICE_MAGNETOMETER compare:service_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if (motionManager.deviceMotionAvailable && ([CMMotionManager availableAttitudeReferenceFrames] & CMAttitudeReferenceFrameXArbitraryCorrectedZVertical)) {
            return rho_ruby_create_boolean(1);
        }
    }
    return rho_ruby_create_boolean(0);
}

- (VALUE) getProperty:(NSString*)property_name {
    if ([RAW_SENSORS_SERVICE_ACCELEROMETER compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if (enableAccelerometer) {
            return rho_ruby_create_boolean(1);
        }
        else {
            return rho_ruby_create_boolean(0);
        }
    }
    if ([RAW_SENSORS_SERVICE_MAGNETOMETER compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if (enableMagnetometer) {
            return rho_ruby_create_boolean(1);
        }
        else {
            return rho_ruby_create_boolean(0);
        }
    }
    if ([RAW_SENSORS_UPDATE_PERIOD compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        return rho_ruby_create_integer(updatePeriod);
    }
    if ([RAW_SENSORS_SERVICE_ALL compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if (enableAccelerometer && enableMagnetometer) {
            return rho_ruby_create_boolean(1);
        }
        else {
            return rho_ruby_create_boolean(0);
        }
    }
    if ([RAW_SENSORS_CALLBACK_URL compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if (callbackURL != nil) {
            return rho_ruby_create_string([callbackURL UTF8String]);
        }
    }
    return rho_ruby_get_NIL();
}

- (void) setProperty:(NSString*)property_name value:(NSString*)value {
    BOOL need_resetup_update = NO;
    if ([RAW_SENSORS_SERVICE_ACCELEROMETER compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        need_resetup_update = YES;
        if (([@"false" compare:value options:NSCaseInsensitiveSearch] == NSOrderedSame)
            || ([@"disabled" compare:value options:NSCaseInsensitiveSearch] == NSOrderedSame)) {
            enableAccelerometer = NO;
        }
        else {
            enableAccelerometer = YES;
        }
    }
    if ([RAW_SENSORS_SERVICE_MAGNETOMETER compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        need_resetup_update = YES;
        if (([@"false" compare:value options:NSCaseInsensitiveSearch] == NSOrderedSame)
            || ([@"disabled" compare:value options:NSCaseInsensitiveSearch] == NSOrderedSame)) {
            enableMagnetometer = NO;
        }
        else {
            enableMagnetometer = YES;
        }
    }
    if ([RAW_SENSORS_SERVICE_ALL compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        need_resetup_update = YES;
        if (([@"false" compare:value options:NSCaseInsensitiveSearch] == NSOrderedSame)
            || ([@"disabled" compare:value options:NSCaseInsensitiveSearch] == NSOrderedSame)) {
            //enableAll = NO;
            enableAccelerometer = NO;
            enableMagnetometer = NO;
        }
        else {
            //enableAll = YES;
            enableAccelerometer = YES;
            enableMagnetometer = YES;
        }
    }
    if ([RAW_SENSORS_UPDATE_PERIOD compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        
        if (value != nil) {
            int period = [value intValue];
            if (period < 200) {
                period = 200;
            }
            need_resetup_update = YES;
            updatePeriod = period;
        }
    }
    if ([RAW_SENSORS_CALLBACK_URL compare:property_name options:NSCaseInsensitiveSearch] == NSOrderedSame) {
        if ((value != nil) /*&& ([value length] > 0)*/) {
            callbackURL = value;
        }
    }
    if (need_resetup_update) {
        [self stopPlatformUpdates];
        [self startPlatformUpdates];
    }
}

- (void) getSensorData {
    [self performSelectorOnMainThread:@selector(fireCallback) withObject:nil waitUntilDone:NO];
    //[self fireCallback];
}

- (void) startPlatformUpdatesCommand {
    BOOL real_updates = NO;
    if (motionManager.accelerometerAvailable ) {
        if (enableAccelerometer || enableAll) {
            real_updates = YES;
            motionManager.gyroUpdateInterval = 1.0/20.0;
            
            [motionManager startAccelerometerUpdatesToQueue:[NSOperationQueue currentQueue]
                                                withHandler: ^(CMAccelerometerData* accData, NSError *error)
             {
                 [[RhoRawSensors getInstance] onAccelerometerUpdate:accData error:error];
             }];
        }
    } else {
        NSLog(@"No accelerometer on device.");
        [self fireErrorCallback:@"No accelerometer on device."];
        //[motionManager release];
    }
    if (motionManager.deviceMotionAvailable ) {
        if (enableMagnetometer || enableAll) {
            real_updates = YES;
            motionManager.deviceMotionUpdateInterval = 1.0/20.0;
            
            //[motionManager startMagnetometerUpdatesToQueue:[NSOperationQueue currentQueue]
            //                                   withHandler: ^(CMMagnetometerData* magData, NSError *error)
            // {
            //     [[RhoRawSensors getInstance] onMagnetometerUpdate:magData error:error];
            // }];
            
            [motionManager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXArbitraryCorrectedZVertical toQueue:[NSOperationQueue currentQueue] withHandler: ^(CMDeviceMotion *motion, NSError *error)
             {
                 [[RhoRawSensors getInstance] onMotionUpdate:motion error:error];
             }];
        }
    } else {
        NSLog(@"No magnetometer on device.");
        [self fireErrorCallback:@"No magnetometer on device."];
        //[motionManager release];
    }
    if (timer != nil) {
        [timer invalidate];
        timer = nil;
    }
    if (real_updates) {
        timer = [NSTimer scheduledTimerWithTimeInterval:(((float)updatePeriod)/1000.0) target:self selector:@selector(onTimerFired:) userInfo:nil repeats:YES];
    }
}

- (void) startPlatformUpdates {
    [self performSelectorOnMainThread:@selector(startPlatformUpdatesCommand) withObject:nil waitUntilDone:NO];
}

- (void) stopPlatformUpdatesCommand {
    if (timer != nil) {
        [timer invalidate];
        timer = nil;
    }
    [motionManager stopAccelerometerUpdates];
    [motionManager stopDeviceMotionUpdates];
    //acceleration_x = 0;
    //acceleration_y = 0;
    //acceleration_z = 0;
    
    //magnetometer_x = 0;
    //magnetometer_y = 0;
    //magnetometer_z = 0;
}

- (void) stopPlatformUpdates {
    [self performSelectorOnMainThread:@selector(stopPlatformUpdatesCommand) withObject:nil waitUntilDone:NO];
}

- (void) onAccelerometerUpdate:(CMAccelerometerData*)accData error:(NSError*)error {
    acceleration_x = accData.acceleration.x * 9.81;
    acceleration_y = accData.acceleration.y * 9.81;
    acceleration_z = accData.acceleration.z * 9.81;
}

- (void) onMagnetometerUpdate:(CMMagnetometerData*)magData error:(NSError*)error {
    // this is RAW data with device filed include
    //magnetometer_x = magData.magneticField.x;
    //magnetometer_y = magData.magneticField.y;
    //magnetometer_z = magData.magneticField.z;
}

- (void) onMotionUpdate:(CMDeviceMotion*)motion error:(NSError*)error {
    magnetometer_x = motionManager.deviceMotion.magneticField.field.x;// motion.magneticField.field.x;
    magnetometer_y = motionManager.deviceMotion.magneticField.field.y;//motion.magneticField.field.y;
    magnetometer_z = motionManager.deviceMotion.magneticField.field.z;//motion.magneticField.field.z;
    //magnetometer_x = motion.magneticField.field.x;
    //magnetometer_y = motion.magneticField.field.y;
    //magnetometer_z = motion.magneticField.field.z;
}



- (void) fireCallback {
    if (callbackURL == nil) {
        return;
    }
    if ([callbackURL length] <= 0) {
        return;
    }
    if (!enableAccelerometer && !enableMagnetometer && !enableAll) {
        return;
    }
	NSString* strBody = @"&rho_callback=1&status=OK";
    
    if (enableAccelerometer || enableAll) {
        strBody = [strBody stringByAppendingString:@"&accelerometerX="];
        strBody = [strBody stringByAppendingString:[NSString stringWithFormat:@"%f",acceleration_x]];
        strBody = [strBody stringByAppendingString:@"&accelerometerY="];
        strBody = [strBody stringByAppendingString:[NSString stringWithFormat:@"%f",acceleration_y]];
        strBody = [strBody stringByAppendingString:@"&accelerometerZ="];
        strBody = [strBody stringByAppendingString:[NSString stringWithFormat:@"%f",acceleration_z]];
    }
    if (enableMagnetometer || enableAll) {
        strBody = [strBody stringByAppendingString:@"&magnetometerX="];
        strBody = [strBody stringByAppendingString:[NSString stringWithFormat:@"%f",magnetometer_x]];
        strBody = [strBody stringByAppendingString:@"&magnetometerY="];
        strBody = [strBody stringByAppendingString:[NSString stringWithFormat:@"%f",magnetometer_y]];
        strBody = [strBody stringByAppendingString:@"&magnetometerZ="];
        strBody = [strBody stringByAppendingString:[NSString stringWithFormat:@"%f",magnetometer_z]];
    }
	const char* cb = [callbackURL UTF8String];
	const char* b = [strBody UTF8String];
    char* norm_url = rho_http_normalizeurl(cb);
    
	rho_net_request_with_data(rho_http_normalizeurl(cb), b);
    
    rho_http_free(norm_url);
    
}

- (void) fireErrorCallback:(NSString*)message {
    if (callbackURL == nil) {
        return;
    }
    if ([callbackURL length] <= 0) {
        return;
    }
	NSString* strBody = @"&rho_callback=1&status=ERROR";
    
    strBody = [strBody stringByAppendingString:@"&message="];
    strBody = [strBody stringByAppendingString:message];
    
	const char* cb = [callbackURL UTF8String];
	const char* b = [strBody UTF8String];
    char* norm_url = rho_http_normalizeurl(cb);
    
	rho_net_request_with_data(rho_http_normalizeurl(cb), b);
    
    rho_http_free(norm_url);
    
}


- (void) onTimerFired:(NSTimer*)theTimer {
    if (enableAll || enableAccelerometer || enableMagnetometer) {
        [self fireCallback];
    }
}

@end






VALUE rawsensors_get_property(const char* property_name) {
    VALUE result = [[RhoRawSensors getInstance] getProperty:[NSString stringWithUTF8String:property_name]];
    return result;
}

void rawsensors_set_property(const char* property_name, const char* value) {
    [[RhoRawSensors getInstance] setProperty:[NSString stringWithUTF8String:property_name] value:[NSString stringWithUTF8String:value]];
}

VALUE rawsensors_is_available(const char* sensor_name) {
    VALUE result = [[RhoRawSensors getInstance] isAvailable:[NSString stringWithUTF8String:sensor_name]];
    return result;
}

void rawsensors_getSensorData() {
    [[RhoRawSensors getInstance] getSensorData];
}
