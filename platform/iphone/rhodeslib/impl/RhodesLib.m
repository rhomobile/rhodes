
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


-(void) startRhodes:(UIApplication *)application; {
    [Rhodes makeSharedInstance];
    [[Rhodes sharedInstance] didFinishLaunchingWithOptionsInternal:[NSMutableDictionary dictionaryWithCapacity:5] application:application];

}

- (void)applicationWillResignActive:(UIApplication *)application {
    [[Rhodes sharedInstance] applicationWillResignActive:application];
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    [[Rhodes sharedInstance] applicationDidEnterBackground:application];
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    [[Rhodes sharedInstance] applicationWillEnterForeground:application];
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    [[Rhodes sharedInstance] applicationDidBecomeActive:application];
}


- (void)applicationWillTerminate:(UIApplication *)application {
    [[Rhodes sharedInstance] applicationWillTerminate:application];
}


@end

