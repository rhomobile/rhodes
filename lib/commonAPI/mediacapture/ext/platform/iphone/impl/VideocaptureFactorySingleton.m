
#import "IVideocapture.h"
#import "VideocaptureFactoryBase.h"

static VideocaptureFactoryBase* ourVideocaptureFactory = nil;

@implementation VideocaptureFactorySingleton

+(id<IVideocaptureFactory>) getVideocaptureFactoryInstance {
    if (ourVideocaptureFactory == nil) {
        ourVideocaptureFactory = [[VideocaptureFactoryBase alloc] init];
    }
    return ourVideocaptureFactory;
}

@end