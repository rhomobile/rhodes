
#import "IGenPropBag.h"
#import "GenPropBagFactoryBase.h"

static GenPropBagFactoryBase* ourGenPropBagFactory = nil;

@implementation GenPropBagFactorySingleton

+(id<IGenPropBagFactory>) getGenPropBagFactoryInstance {
    if (ourGenPropBagFactory == nil) {
        ourGenPropBagFactory = [[GenPropBagFactoryBase alloc] init];
    }
    return ourGenPropBagFactory;
}

@end