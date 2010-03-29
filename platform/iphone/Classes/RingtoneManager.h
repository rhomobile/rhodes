//
//  RingtoneManager.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 29.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface RingtoneManager : NSObject {}

+ (void)playStart:(NSString*)fileName;
+ (void)playStop;

@end
