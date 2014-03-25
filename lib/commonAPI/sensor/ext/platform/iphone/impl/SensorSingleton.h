
#import "ISensor.h"
#import "SensorSingletonBase.h"

@interface SensorSingleton : SensorSingletonBase<ISensorSingleton> {
}



-(void) makeSensorByType:(NSString*)type methodResult:(id<IMethodResult>)methodResult;
-(void) enumerate:(id<IMethodResult>)methodResult;




@end