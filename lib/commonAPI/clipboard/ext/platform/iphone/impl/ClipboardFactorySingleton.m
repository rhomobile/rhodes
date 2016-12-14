
#import "IClipboard.h"
#import "ClipboardFactoryBase.h"

static ClipboardFactoryBase* ourClipboardFactory = nil;

@implementation ClipboardFactorySingleton

+(id<IClipboardFactory>) getClipboardFactoryInstance {
    if (ourClipboardFactory == nil) {
        ourClipboardFactory = [[ClipboardFactoryBase alloc] init];
    }
    return ourClipboardFactory;
}

@end