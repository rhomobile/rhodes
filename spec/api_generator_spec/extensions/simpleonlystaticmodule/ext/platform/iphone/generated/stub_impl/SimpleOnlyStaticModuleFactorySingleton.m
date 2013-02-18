
#import "ISimpleOnlyStaticModule.h"
#import "SimpleOnlyStaticModuleFactoryBase.h"

static SimpleOnlyStaticModuleFactoryBase* ourSimpleOnlyStaticModuleFactory = nil;

@implementation SimpleOnlyStaticModuleFactorySingleton

+(id<ISimpleOnlyStaticModuleFactory>) getSimpleOnlyStaticModuleFactoryInstance {
    if (ourSimpleOnlyStaticModuleFactory == nil) {
        ourSimpleOnlyStaticModuleFactory = [[SimpleOnlyStaticModuleFactoryBase alloc] init];
    }
    return ourSimpleOnlyStaticModuleFactory;
}

@end