
#import "MediaplayerSingleton.h"

extern void rho_sys_open_url(const char* url);

@implementation MediaplayerSingleton



-(void) start:(NSString*)filename methodResult:(id<IMethodResult>)methodResult {
    if (filename != nil) {
        rho_sys_open_url([filename UTF8String]);
    }
}

-(void) stop:(id<IMethodResult>)methodResult{

}

-(void) startvideo:(NSString*)filename methodResult:(id<IMethodResult>)methodResult {
    [self start:filename methodResult:nil];
}

-(void) stopvideo:(id<IMethodResult>)methodResult {

}

-(void) getAllRingtones:(id<IMethodResult>)methodResult {
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:1];
    [methodResult setResult:ar];
}

-(void) playRingTone:(NSString*)name methodResult:(id<IMethodResult>)methodResult {

}

-(void) stopRingTone:(id<IMethodResult>)methodResult {

}




@end