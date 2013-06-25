
#import "IMediaplayer.h"
#import "MediaplayerSingletonBase.h"

@interface MediaplayerSingleton : MediaplayerSingletonBase<IMediaplayerSingleton> {
}



-(void) start:(NSString*)filename;
-(void) stop;
-(void) startvideo:(NSString*)filename;
-(void) stopvideo;
-(void) getAllRingtones:(id<IMethodResult>)methodResult;
-(void) playRingTone:(NSString*)name;
-(void) stopRingTone;




@end