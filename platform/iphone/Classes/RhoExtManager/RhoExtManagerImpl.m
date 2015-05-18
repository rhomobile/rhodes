//
//  RhoExtManagerImpl.m
//  RhoAppBaseLib
//
//  Created by Dmitry Soldatenkov on 12.05.14.
//
//

#import "RhoExtManagerImpl.h"

@implementation RhoExtManagerImpl

@synthesize mExtensions;

-(id) init {
    self = [super init];
    
    self.mExtensions = [NSMutableSet setWithCapacity:32];
    
    
    return self;
}


-(void) registerExtension:(id<IRhoExtension>)extension {
    [mExtensions addObject:extension];
}

-(void) unregisterExtension:(id<IRhoExtension>)extension {
    [mExtensions removeObject:extension];
}

-(BOOL) onBeforeNavigate:(NSString*)url tabIndex:(int)tabIndex {
    BOOL res = NO;
    
    NSEnumerator* enumerator = [self.mExtensions objectEnumerator];
    
    id<IRhoExtension> ext = [enumerator nextObject];
    while (ext != nil) {
        res = res || [ext onBeforeNavigate:url tabIndex:tabIndex];
        
        ext = [enumerator nextObject];
    }
    
    return res;
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
    NSEnumerator* enumerator = [self.mExtensions objectEnumerator];
    
    id<IRhoExtension> ext = [enumerator nextObject];
    while (ext != nil) {
        [ext applicationDidBecomeActive:application];
        
        ext = [enumerator nextObject];
    }
    
}

- (BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    BOOL res = NO;
    
    NSEnumerator* enumerator = [self.mExtensions objectEnumerator];
    
    id<IRhoExtension> ext = [enumerator nextObject];
    while (ext != nil) {
        res = res || [ext application:application openURL:url sourceApplication:sourceApplication annotation:annotation];
        
        ext = [enumerator nextObject];
    }
    
    return res;
}



@end
