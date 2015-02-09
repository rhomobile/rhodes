
#import "IDevelopment.h"
#import "DevelopmentFactoryBase.h"

static DevelopmentFactoryBase* ourDevelopmentFactory = nil;

@implementation DevelopmentFactorySingleton

+(id<IDevelopmentFactory>) getDevelopmentFactoryInstance {
    if (ourDevelopmentFactory == nil) {
        ourDevelopmentFactory = [[DevelopmentFactoryBase alloc] init];
    }
    return ourDevelopmentFactory;
}

@end