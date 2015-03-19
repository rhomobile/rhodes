
#import "IDeviceMemory.h"
#import "DeviceMemoryFactoryBase.h"

static DeviceMemoryFactoryBase* ourDeviceMemoryFactory = nil;

@implementation DeviceMemoryFactorySingleton

+(id<IDeviceMemoryFactory>) getDeviceMemoryFactoryInstance {
    if (ourDeviceMemoryFactory == nil) {
        ourDeviceMemoryFactory = [[DeviceMemoryFactoryBase alloc] init];
    }
    return ourDeviceMemoryFactory;
}

@end