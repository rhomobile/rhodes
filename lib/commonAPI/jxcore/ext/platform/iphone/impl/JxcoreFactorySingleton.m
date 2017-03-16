
#import "IJxcore.h"
#import "JxcoreFactoryBase.h"

static JxcoreFactoryBase* ourJxcoreFactory = nil;

@implementation JxcoreFactorySingleton

+(id<IJxcoreFactory>) getJxcoreFactoryInstance {
    if (ourJxcoreFactory == nil) {
        ourJxcoreFactory = [[JxcoreFactoryBase alloc] init];
    }
    return ourJxcoreFactory;
}

@end