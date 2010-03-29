//
//  Alert.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 29.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "Alert.h"
#import "Rhodes.h"

#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Alert"

@interface RhoAlertShowPopupTask : NSObject {}
+ (void)run:(NSString*)message;
@end

@implementation RhoAlertShowPopupTask
+ (void)run:(NSString*)message {
    UIAlertView *alert = [[UIAlertView alloc]
                          initWithTitle: @"Alert"
                          message: message
                          delegate:nil
                          cancelButtonTitle:@"OK"
                          otherButtonTitles:nil];
    [alert show];
    [alert release];
}
@end

@interface RhoAlertVibrateTask : NSObject {}
+ (void)run;
@end

@implementation RhoAlertVibrateTask
+ (void)run {
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}
@end

@interface RhoAlertPlayFileTask : NSObject {}
+ (void)run:(NSString*)file :(NSString*)type;
@end

@implementation RhoAlertPlayFileTask
+ (void)run:(NSString*)file :(NSString*)type {
    [[Rhodes sharedInstance] playStart:file mediaType:type];
}
@end

@implementation Alert

+ (void)showPopup:(NSString *)message {
    id runnable = [RhoAlertShowPopupTask class];
    [Rhodes performOnUiThread:runnable arg:message wait:NO];
}

+ (void)vibrate:(int)duration {
    id runnable = [RhoAlertVibrateTask class];
    [Rhodes performOnUiThread:runnable wait:NO];
}

+ (void)playFile:(NSString *)file mediaType:(NSString *)type {
    id runnable = [RhoAlertPlayFileTask class];
    [Rhodes performOnUiThread:runnable arg:file arg:type wait:NO];
}

@end

void alert_show_popup(char* message) {
    if (!message) {
        RAWLOG_ERROR("Alert.show_popup - wrong arguments");
        return;
	}
    
    [Alert showPopup:[NSString stringWithUTF8String:message]];
}

void alert_vibrate(int duration) {
    [Alert vibrate:duration];
}

void alert_play_file(char* file_name, char* media_type) {
    if (!file_name) {
        RAWLOG_ERROR("Alert.play_file - please specify file name to play");
        return;
    }
    
    [Alert playFile:[NSString stringWithUTF8String:file_name]
            mediaType:media_type?[NSString stringWithUTF8String:media_type]:NULL];
}
