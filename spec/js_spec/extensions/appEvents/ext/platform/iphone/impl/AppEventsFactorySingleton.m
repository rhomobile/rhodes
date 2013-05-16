
#import "IAppEvents.h"
#import "AppEventsFactoryBase.h"

static AppEventsFactoryBase* ourAppEventsFactory = nil;

@implementation AppEventsFactorySingleton

+(id<IAppEventsFactory>) getAppEventsFactoryInstance {
    if (ourAppEventsFactory == nil) {
        ourAppEventsFactory = [[AppEventsFactoryBase alloc] init];
    }
    return ourAppEventsFactory;
}

@end