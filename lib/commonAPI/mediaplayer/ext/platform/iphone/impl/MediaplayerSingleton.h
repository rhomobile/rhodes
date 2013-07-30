
#import "IMediaplayer.h"
#import "MediaplayerSingletonBase.h"

@interface MediaplayerSingleton : MediaplayerSingletonBase<IMediaplayerSingleton> {
}



-(void) start:(NSString*)filename methodResult:(id<IMethodResult>)methodResult;
-(void) stop:(id<IMethodResult>)methodResult;
-(void) startvideo:(NSString*)filename methodResult:(id<IMethodResult>)methodResult;
-(void) stopvideo:(id<IMethodResult>)methodResult;
-(void) getAllRingtones:(id<IMethodResult>)methodResult;
-(void) playRingTone:(NSString*)name methodResult:(id<IMethodResult>)methodResult;
-(void) stopRingTone:(id<IMethodResult>)methodResult;




@end