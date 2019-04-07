
#import "../api/RhodesLib.h"

#import "../../Classes/Rhodes.h"


static RhodesLib* sharedInstance = nil;

@implementation RhodesLib


+(RhodesLib*)getSharedInstance {
    if (sharedInstance == nil) {
        sharedInstance = [[RhodesLib alloc] init];
    }
    return sharedInstance;
}


-(void) startRhodes {
    [Rhodes makeSharedInstance];
    [[Rhodes sharedInstance] didFinishLaunchingWithOptionsInternal:[NSMutableDictionary dictionaryWithCapacity:5] application:[UIApplication sharedApplication]];

}

-(void) onActivate {
    
}

-(void) onDeactivate {
    
}


@end

