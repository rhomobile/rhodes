//
//  DateTimePicker.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 29.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "DateTime.h"
#import "Rhodes.h"
#import "DateTimePickerDelegate.h"



void choose_datetime_with_range(char* callback, char* title, long initial_time, int format, char* data, long min_time, long max_time) {
    if (!rho_rhodesapp_check_mode())
        return;
    DateTime* dateTime = [[DateTime alloc] init];
    dateTime.url = [NSString stringWithUTF8String:callback];
    dateTime.title = [NSString stringWithUTF8String:title];
    dateTime.initialTime = initial_time;
    dateTime.format = format;
    dateTime.data = [NSString stringWithUTF8String:data];
	dateTime.minTime = min_time;
	dateTime.maxTime = max_time;
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(chooseDateTime:) withObject:dateTime waitUntilDone:YES];
    [dateTime release];
}

void choose_datetime(char* callback, char* title, long initial_time, int format, char* data) {
    choose_datetime_with_range(callback, title, initial_time, format, data, 0, 0 );
}

void set_change_value_callback_datetime(char* callback) {
    NSString* ns_callback = nil;
    if (callback != NULL) {
        ns_callback = [NSString stringWithUTF8String:callback];
        if ([ns_callback length] <= 0) {
            ns_callback = nil;
        }
    }
    [DateTimePickerDelegate setChangeValueCallback:ns_callback];
}
