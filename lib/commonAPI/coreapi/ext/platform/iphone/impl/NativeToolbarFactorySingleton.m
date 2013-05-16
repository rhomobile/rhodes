
#import "INativeToolbar.h"
#import "NativeToolbarFactoryBase.h"

static NativeToolbarFactoryBase* ourNativeToolbarFactory = nil;

@implementation NativeToolbarFactorySingleton

+(id<INativeToolbarFactory>) getNativeToolbarFactoryInstance {
    if (ourNativeToolbarFactory == nil) {
        ourNativeToolbarFactory = [[NativeToolbarFactoryBase alloc] init];
    }
    return ourNativeToolbarFactory;
}

@end