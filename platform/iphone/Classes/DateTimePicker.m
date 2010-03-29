//
//  DateTimePicker.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 29.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "DateTimePicker.h"
#import "DateTime.h"
#import "Rhodes.h"

@implementation DateTimePicker

+ (void)chooseDateTime:(NSString*)url title:(NSString*)title initialTime:(long)initial_time
                format:(int)format data:(NSString*)data {
    DateTime* dateTime = [[DateTime alloc] init];
    dateTime.url = url;
    dateTime.title = title;
    dateTime.initialTime = initial_time;
    dateTime.format = format;
    dateTime.data = data;
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(chooseDateTime:) withObject:dateTime waitUntilDone:YES];
    [dateTime release];
}

@end

void choose_datetime(char* callback, char* title, long initial_time, int format, char* data) {
	[DateTimePicker chooseDateTime:[NSString stringWithUTF8String:callback] 
                              title:[NSString stringWithUTF8String:title]
                        initialTime:initial_time 
                             format:format
                               data:[NSString stringWithUTF8String:data]];
}