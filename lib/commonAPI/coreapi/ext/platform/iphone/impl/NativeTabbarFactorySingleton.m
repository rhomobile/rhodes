
#import "INativeTabbar.h"
#import "NativeTabbarFactoryBase.h"

static NativeTabbarFactoryBase* ourNativeTabbarFactory = nil;

@implementation NativeTabbarFactorySingleton

+(id<INativeTabbarFactory>) getNativeTabbarFactoryInstance {
    if (ourNativeTabbarFactory == nil) {
        ourNativeTabbarFactory = [[NativeTabbarFactoryBase alloc] init];
    }
    return ourNativeTabbarFactory;
}

@end