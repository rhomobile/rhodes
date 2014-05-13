
#import "IAudioCapture.h"
#import "AudioCaptureFactoryBase.h"

static AudioCaptureFactoryBase* ourAudioCaptureFactory = nil;

@implementation AudioCaptureFactorySingleton

+(id<IAudioCaptureFactory>) getAudioCaptureFactoryInstance {
    if (ourAudioCaptureFactory == nil) {
        ourAudioCaptureFactory = [[AudioCaptureFactoryBase alloc] init];
    }
    return ourAudioCaptureFactory;
}

@end