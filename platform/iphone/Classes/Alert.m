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

@interface RhoAlertShowPopupTask : NSObject<RhoRunnable>
{
    NSString *message;
}

@property (nonatomic,copy) NSString *message;

- (id)initWithMessage:(NSString*)s;
- (void)dealloc;
- (void)run;

@end

@implementation RhoAlertShowPopupTask

@synthesize message;

- (id)initWithMessage:(NSString *)s {
    message = nil;
    self.message = s;
    return self;
}

- (void)dealloc {
    [message release];
    [super dealloc];
}

- (void)run {
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

@interface RhoAlertVibrateTask : NSObject<RhoRunnable>
{
    int duration;
}

- (id)init:(int)d;
- (void)run;

@end

@implementation RhoAlertVibrateTask

- (id)init:(int)d {
    duration = d;
    return self;
}

- (void)run {
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

@end

@interface RhoAlertPlayFileTask : NSObject<RhoRunnable>
{
    NSString *file;
    NSString *type;
}

@property (nonatomic,copy) NSString *file;
@property (nonatomic,copy) NSString *type;

- (id)initWithFileName:(NSString*)f andType:(NSString*)t;
- (void)dealloc;
- (void)run;

@end

@implementation RhoAlertPlayFileTask

@synthesize file, type;

- (id)initWithFileName:(NSString *)f andType:(NSString *)t {
    file = nil;
    self.file = f;
    type = nil;
    self.type = t;
    return self;
}

- (void)dealloc {
    [file release];
    [type release];
    [super dealloc];
}

- (void)run {
    [[Rhodes sharedInstance] playStart:file mediaType:type];
}

@end

@implementation Alert

+ (void)showPopup:(NSString *)message {
    id task = [[[RhoAlertShowPopupTask alloc] initWithMessage:message] autorelease];
    [Rhodes performOnUiThread:task wait:NO];
}

+ (void)vibrate:(int)duration {
    id task = [[[RhoAlertVibrateTask alloc] init:duration] autorelease];
    [Rhodes performOnUiThread:task wait:NO];
}

+ (void)playFile:(NSString *)file mediaType:(NSString *)type {
    id task = [[[RhoAlertPlayFileTask alloc] initWithFileName:file andType:type] autorelease];
    [Rhodes performOnUiThread:task wait:NO];
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
