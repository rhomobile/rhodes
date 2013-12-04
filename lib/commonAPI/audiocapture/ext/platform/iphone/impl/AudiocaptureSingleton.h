
#import "IAudiocapture.h"
#import "AudiocaptureSingletonBase.h"

@interface AudiocaptureSingleton : AudiocaptureSingletonBase<IAudiocaptureSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end