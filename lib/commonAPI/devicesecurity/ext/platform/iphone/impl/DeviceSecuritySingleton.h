
#import "IDeviceSecurity.h"
#import "DeviceSecuritySingletonBase.h"

@interface DeviceSecuritySingleton : DeviceSecuritySingletonBase<IDeviceSecuritySingleton> {
}



-(void) isDeviceSecured:(id<IMethodResult>)methodResult;
-(void) isDeviceJailbroken:(id<IMethodResult>)methodResult;
-(void) isDeviceRooted:(id<IMethodResult>)methodResult;
-(void) isRunOnSimulator:(id<IMethodResult>)methodResult;
-(void) isDebugMode:(id<IMethodResult>)methodResult;




@end
