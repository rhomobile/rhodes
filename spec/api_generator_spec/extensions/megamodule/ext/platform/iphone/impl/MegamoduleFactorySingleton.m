
#import "IMegamodule.h"
#import "MegamoduleFactoryBase.h"

static MegamoduleFactoryBase* ourMegamoduleFactory = nil;

@implementation MegamoduleFactorySingleton

+(id<IMegamoduleFactory>) getMegamoduleFactoryInstance {
    if (ourMegamoduleFactory == nil) {
        ourMegamoduleFactory = [[MegamoduleFactoryBase alloc] init];
    }
    return ourMegamoduleFactory;
}

@end