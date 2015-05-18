//
//  RhoExtManagerImpl.h
//  RhoAppBaseLib
//
//  Created by Dmitry Soldatenkov on 12.05.14.
//
//

#import <Foundation/Foundation.h>
#import "IRhoExtManager.h"

@interface RhoExtManagerImpl : NSObject <IRhoExtManager> {
    NSMutableSet* mExtensions;
}

@property (retain) NSMutableSet* mExtensions;

-(id) init;

-(void) registerExtension:(id<IRhoExtension>)extension;
-(void) unregisterExtension:(id<IRhoExtension>)extension;

-(BOOL) onBeforeNavigate:(NSString*)url tabIndex:(int)tabIndex;

- (void) applicationDidBecomeActive:(UIApplication *)application;
- (BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation;

@end
