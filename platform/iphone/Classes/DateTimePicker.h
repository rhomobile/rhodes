//
//  DateTimePicker.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 29.03.10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface DateTimePicker : NSObject {}

+ (void)chooseDateTime:(NSString*)url title:(NSString*)title initialTime:(long)initial_time
                format:(int)format data:(NSString*)data;

@end
