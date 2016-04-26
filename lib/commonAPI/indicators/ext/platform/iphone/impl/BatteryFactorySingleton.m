
#import "IBattery.h"
#import "BatteryFactoryBase.h"

static BatteryFactoryBase* ourBatteryFactory = nil;

@implementation BatteryFactorySingleton

+(id<IBatteryFactory>) getBatteryFactoryInstance {
    if (ourBatteryFactory == nil) {
        ourBatteryFactory = [[BatteryFactoryBase alloc] init];
    }
    return ourBatteryFactory;
}

@end