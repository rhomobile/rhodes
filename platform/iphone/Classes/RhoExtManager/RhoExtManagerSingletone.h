//
//  RhoExtManagerSingletone.h
//  RhoAppBaseLib
//
//  Created by Dmitry Soldatenkov on 12.05.14.
//
//

#import <Foundation/Foundation.h>
#import "IRhoExtManager.h"


@interface RhoExtManagerSingletone : NSObject

+(id<IRhoExtManager>) getExtensionManager;

@end
