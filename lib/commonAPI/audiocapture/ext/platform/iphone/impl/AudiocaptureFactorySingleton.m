
#import "IAudiocapture.h"
#import "AudiocaptureFactoryBase.h"

static AudiocaptureFactoryBase* ourAudiocaptureFactory = nil;

@implementation AudiocaptureFactorySingleton

+(id<IAudiocaptureFactory>) getAudiocaptureFactoryInstance {
    if (ourAudiocaptureFactory == nil) {
        ourAudiocaptureFactory = [[AudiocaptureFactoryBase alloc] init];
    }
    return ourAudiocaptureFactory;
}

@end