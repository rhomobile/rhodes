
#import "BatterySingleton.h"
#import "UIKit/UIDevice.h"

@implementation BatterySingleton

-(id)init {
    self = [super init];
    updatePeriodMSec = 5000;
    timer = nil;
    callbackHolder = nil;
    return self;
}

-(void) getRefreshInterval:(id<IMethodResult>)methodResult {
//    #ifdef DEBUG
//    NSLog(@"BTTR| Getting refresh interval, %d", updatePeriodMSec);
//    #endif
    [methodResult setResult:[NSNumber numberWithBool:updatePeriodMSec]];
}

-(void) setRefreshInterval:(int)refreshInterval methodResult:(id<IMethodResult>)methodResult{
//    #ifdef DEBUG
//    NSLog(@"BTTR| Setting refresh interval, %d", refreshInterval);
//    #endif
    updatePeriodMSec = refreshInterval;
}

-(void) readBatteryStats:(id<IMethodResult>)methodResult {
    NSMutableDictionary* dict = [NSMutableDictionary new];
    
    UIDevice *device = [UIDevice currentDevice];
    // get current monitoring status
    BOOL isMonitoringEnabled = [device isBatteryMonitoringEnabled];
    [device setBatteryMonitoringEnabled:YES];
    
    UIDeviceBatteryState state = [device batteryState];
    BOOL isPlugged =  state == UIDeviceBatteryStateCharging || state == UIDeviceBatteryStateFull;
    double bLevel = [device batteryLevel];
    
    #if TARGET_IPHONE_SIMULATOR
    // in case of simulator battery level is unknown
    if (state == UIDeviceBatteryStateUnknown && bLevel < 0)
    {
        isPlugged = true;
        bLevel = 1.0;
    }
    #endif
    
    int batteryLevel = (int)round(bLevel * 100);
    [dict setObject:[NSNumber numberWithBool:isPlugged] forKey:HK_AC_LINE_STATUS];
    [dict setObject:[NSNumber numberWithInt:batteryLevel] forKey:HK_BATTERY_LIFE_PERCENT];
    [dict setObject:[NSNumber numberWithBool:YES] forKey:HK_BATTERY_LIFE_KNOWN];
    
//    #ifdef DEBUG
//    NSLog(@"BTTR| ReadBatteryStats: isPlugged %@, level %d",  isPlugged ? @"YES" : @"NO", batteryLevel);
//    #endif
    [device setBatteryMonitoringEnabled:isMonitoringEnabled];
    
    [methodResult setResult:dict];
}

-(void) updateBatteryStatus:(id<IMethodResult>)methodResult {
//    #ifdef DEBUG
//    NSLog(@"BTTR| updateBatteryStatus!");
//    #endif
    [self readBatteryStats:callbackHolder];
}

-(void) batteryStatus:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    
    [self readBatteryStats:methodResult];
    
    int updateInterval = updatePeriodMSec;
    
    NSObject* val = [propertyMap objectForKey:HK_REFRESH_INTERVAL];
    
    if ( val != nil && [val isKindOfClass:[NSNumber class]] )
    {
        updateInterval = [(NSNumber*)val intValue];
    }
    
    if ([methodResult hasCallback] && updateInterval > 0)
    {
        callbackHolder = methodResult;
        
        [self performSelectorOnMainThread:@selector(setupTimer:methodResult:) withObject:[NSNumber numberWithInt:updateInterval] waitUntilDone:NO];
    }
}

- (void)setupTimer:(NSNumber*)delay methodResult:(id<IMethodResult>)methodResult {
    
	if (timer != nil) {
        [timer invalidate];
        timer = nil;
    }
    
    int iDelay = [delay intValue];
    
//    #ifdef DEBUG
//    NSLog(@"BTTR| setupTimer: delay %d",  iDelay);
//    #endif
    
    if (iDelay > 0)
    {
        if (iDelay < 100)
        {
            iDelay = 100;
        }
        
        timer = [NSTimer scheduledTimerWithTimeInterval:(iDelay/1000)
                                                 target:self
                                               selector:@selector(updateBatteryStatus:)
                                               userInfo:nil
                                                repeats:YES];
    }
}

-(void) stopBatteryStatus:(id<IMethodResult>)methodResult {
    [self performSelectorOnMainThread:@selector(setupTimer:methodResult:) withObject:[NSNumber numberWithInt:0] waitUntilDone:YES];
    
//    #ifdef DEBUG
//    NSLog(@"BTTR| stopBatteryStatus");
//    #endif
    
    callbackHolder = nil; 
}

-(void) smartBatteryStatus:(id<IMethodResult>)methodResult {
    // not implemented on iOS
}

-(void) showIcon:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    // not implemented on iOS
}

-(void) hideIcon:(id<IMethodResult>)methodResult {
    // not implemented on iOS
}

-(void) getAverageCurrentConsumption:(id<IMethodResult>)methodResult {
    // not implemented on iOS
}

-(void) setAverageCurrentConsumption:(int)averageCurrentConsumption methodResult:(id<IMethodResult>)methodResult {
    // not implemented on iOS
}

-(void) getTripDuration:(id<IMethodResult>)methodResult {
    // not implemented on iOS    
}

-(void) setTripDuration:(int)tripDuration methodResult:(id<IMethodResult>)methodResult {
    // not implemented on iOS
}

-(void) batteryDiagnostics:(id<IMethodResult>)methodResult {
    // not implemented on iOS
}





@end
