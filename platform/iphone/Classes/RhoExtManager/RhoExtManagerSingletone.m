//
//  RhoExtManagerSingletone.m
//  RhoAppBaseLib
//
//  Created by Dmitry Soldatenkov on 12.05.14.
//
//

#import "RhoExtManagerSingletone.h"
#import "RhoExtManagerImpl.h"


static RhoExtManagerImpl* ourExtManager = nil;

@implementation RhoExtManagerSingletone

+(id<IRhoExtManager>) getExtensionManager {
    if (ourExtManager == nil) {
        ourExtManager = [[RhoExtManagerImpl alloc] init];
    }
    return ourExtManager;
}

@end
