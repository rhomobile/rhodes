
#import "IKeyCapture.h"
#import "KeyCaptureFactoryBase.h"

static KeyCaptureFactoryBase* ourKeyCaptureFactory = nil;

@implementation KeyCaptureFactorySingleton

+(id<IKeyCaptureFactory>) getKeyCaptureFactoryInstance {
    if (ourKeyCaptureFactory == nil) {
        ourKeyCaptureFactory = [[KeyCaptureFactoryBase alloc] init];
    }
    return ourKeyCaptureFactory;
}

@end