
#import "IAudioCapture.h"
#import "AudioCaptureSingletonBase.h"

@interface AudioCaptureSingleton : AudioCaptureSingletonBase<IAudioCaptureSingleton> {
}


-(NSString*)getInitialDefaultID;






@end