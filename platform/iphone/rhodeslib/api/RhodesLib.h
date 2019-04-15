
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface RhodesLib : NSObject {}

+(RhodesLib*)getSharedInstance;

//-(BOOL)didFinishLaunchingWithOptions:(NSDictionary *)launchOptions application:(UIApplication *)application
-(void) startRhodes:(UIApplication *)application;

- (void)applicationWillResignActive:(UIApplication *)application;


- (void)applicationDidEnterBackground:(UIApplication *)application;


- (void)applicationWillEnterForeground:(UIApplication *)application;


- (void)applicationDidBecomeActive:(UIApplication *)application;


- (void)applicationWillTerminate:(UIApplication *)application;


@end
