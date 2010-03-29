//
//  Alert.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 29.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Alert : NSObject {}

+ (void)showPopup:(NSString*)message;
+ (void)vibrate:(int)duration;
+ (void)playFile:(NSString*)file mediaType:(NSString*)type;

@end
