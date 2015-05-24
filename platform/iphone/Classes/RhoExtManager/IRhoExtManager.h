//
//  IRhoExtManager.h
//  RhoAppBaseLib

//
//

#import <Foundation/Foundation.h>
#import "IRhoExtension.h"


@protocol IRhoExtManager <NSObject>

-(void) registerExtension:(id<IRhoExtension>)extension;
-(void) unregisterExtension:(id<IRhoExtension>)extension;

-(BOOL) onBeforeNavigate:(NSString*)url tabIndex:(int)tabIndex;

- (void) applicationDidBecomeActive:(UIApplication *)application;
- (BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation;

@end
