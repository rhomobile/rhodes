
#import "IDeviceMemory.h"
#import "DeviceMemorySingletonBase.h"

@interface DeviceMemorySingleton : DeviceMemorySingletonBase<IDeviceMemorySingleton> {
    id<IMethodResult> callback;
}

@property (readwrite, retain) id<IMethodResult> callback;

-(void) startListeningPlatformMemoryWarning:(id<IMethodResult>)methodResult;
-(void) stopListeningPlatformMemoryWarning:(id<IMethodResult>)methodResult;
-(void) getAvailableMemory:(id<IMethodResult>)methodResult;
-(void) getInternalStorage:(id<IMethodResult>)methodResult;
-(void) getExternalStorage:(id<IMethodResult>)methodResult;



@end