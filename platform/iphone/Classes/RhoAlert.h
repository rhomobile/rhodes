//
//  Alert.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 29.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "common/rhoparams.h"

@interface RhoAlert : NSObject {}

+ (void)showPopup:(rho_param*)p;
+ (void)vibrate:(int)duration;
+ (void)playFile:(NSString*)file mediaType:(NSString*)type;

@end
