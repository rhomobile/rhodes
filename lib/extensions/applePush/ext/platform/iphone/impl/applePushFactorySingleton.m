
#import "IapplePush.h"
#import "applePushFactoryBase.h"

static ApplePushFactoryBase* ourapplePushFactory = nil;

@implementation ApplePushFactorySingleton

+(id<IApplePushFactory>) getApplePushFactoryInstance {
    if (ourapplePushFactory == nil) {
        ourapplePushFactory = [[ApplePushFactoryBase alloc] init];
    }
    return ourapplePushFactory;
}

@end