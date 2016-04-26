
#import "IBattery.h"
#import "BatterySingletonBase.h"

@interface BatterySingleton : BatterySingletonBase<IBatterySingleton> {
    NSTimer* timer;
    int updatePeriodMSec;
    id<IMethodResult> callbackHolder;
}

-(id) init;

-(void) getRefreshInterval:(id<IMethodResult>)methodResult;
-(void) setRefreshInterval:(int)refreshInterval methodResult:(id<IMethodResult>)methodResult;
-(void) readBatteryStats:(id<IMethodResult>)methodResult;
-(void) updateBatteryStatus:(id<IMethodResult>)methodResult;
-(void) batteryStatus:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) stopBatteryStatus:(id<IMethodResult>)methodResult;
-(void) smartBatteryStatus:(id<IMethodResult>)methodResult;
-(void) showIcon:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) hideIcon:(id<IMethodResult>)methodResult;


@end