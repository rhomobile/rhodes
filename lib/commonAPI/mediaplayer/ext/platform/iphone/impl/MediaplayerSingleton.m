
#import "MediaplayerSingleton.h"

extern void rho_sys_open_url(const char* url);

@implementation MediaplayerSingleton



-(void) start:(NSString*)filename {
    if (filename != nil) {
        rho_sys_open_url([filename UTF8String]);
    }
}

-(void) stop {

}

-(void) startvideo:(NSString*)filename {
    [self start:filename];
}

-(void) stopvideo {

}

-(void) getAllRingtones:(id<IMethodResult>)methodResult {
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:1];
    [methodResult setResult:ar];
}

-(void) playRingTone:(NSString*)name {

}

-(void) stopRingTone {

}




@end