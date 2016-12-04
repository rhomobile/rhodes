
#import "IDeviceSecurity.h"
#import "DeviceSecurityFactoryBase.h"

static DeviceSecurityFactoryBase* ourDeviceSecurityFactory = nil;

@implementation DeviceSecurityFactorySingleton

+(id<IDeviceSecurityFactory>) getDeviceSecurityFactoryInstance {
    if (ourDeviceSecurityFactory == nil) {
        ourDeviceSecurityFactory = [[DeviceSecurityFactoryBase alloc] init];
    }
    return ourDeviceSecurityFactory;
}

@end