//
//  IRhoExtension.h
//  RhoAppBaseLib

//
//

#import <Foundation/Foundation.h>

@class UIApplication;

@protocol IRhoExtension <NSObject>

-(BOOL) onBeforeNavigate:(NSString*)url tabIndex:(int)tabIndex;

- (void) applicationDidBecomeActive:(UIApplication *)application;
- (BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation;

@end
