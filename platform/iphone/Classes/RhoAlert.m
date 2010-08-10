//
//  Alert.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 29.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "RhoAlert.h"
#import "Rhodes.h"

#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Alert"

static UIAlertView *currentAlert = nil;

@interface RhoAlertShowPopupTask : NSObject<UIAlertViewDelegate> {
    NSString *callback;
    NSMutableArray *buttons;
}

@property (nonatomic,retain) NSString *callback;
@property (nonatomic,retain) NSMutableArray *buttons;

- (id)init;
- (void)dealloc;
- (void)run:(NSValue*)v;
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;
    
@end

@implementation RhoAlertShowPopupTask

@synthesize callback, buttons;

- (id)init {
    callback = nil;
    buttons = nil;
    return self;
}

- (void)dealloc {
    self.callback = nil;
    self.buttons = nil;
    [super dealloc];
}

- (void)run:(NSValue*)v {
    NSString *title = @"Alert";
    NSString *message = nil;
    
    self.buttons = [NSMutableArray arrayWithCapacity:1];
    rho_param *p = [v pointerValue];
    if (p->type == RHO_PARAM_STRING) {
        message = [NSString stringWithUTF8String:p->v.string];
        [buttons addObject:[NSMutableArray arrayWithObjects:@"OK", @"OK", nil]];
    }
    else if (p->type == RHO_PARAM_HASH) {
        for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
            char *name = p->v.hash->name[i];
            rho_param *value = p->v.hash->value[i];
            
            if (strcasecmp(name, "title") == 0) {
                if (value->type != RHO_PARAM_STRING) {
                    RAWLOG_ERROR("'title' should be string");
                    continue;
                }
                title = [NSString stringWithUTF8String:value->v.string];
            }
            else if (strcasecmp(name, "message") == 0) {
                if (value->type != RHO_PARAM_STRING) {
                    RAWLOG_ERROR("'message' should be string");
                    continue;
                }
                message = [NSString stringWithUTF8String:value->v.string];
            }
            else if (strcasecmp(name, "callback") == 0) {
                if (value->type != RHO_PARAM_STRING) {
                    RAWLOG_ERROR("'callback' should be string");
                    continue;
                }
                self.callback = [NSString stringWithUTF8String:value->v.string];
            }
            else if (strcasecmp(name, "buttons") == 0) {
                if (value->type != RHO_PARAM_ARRAY) {
                    RAWLOG_ERROR("'buttons' should be array");
                    continue;
                }
                for (int j = 0, limj = value->v.array->size; j < limj; ++j) {
                    rho_param *arrValue = value->v.array->value[j];
                    
                    NSString *itemId = nil;
                    NSString *itemTitle = nil;
                    switch (arrValue->type) {
                        case RHO_PARAM_STRING:
                            itemId = [NSString stringWithUTF8String:arrValue->v.string];
                            itemTitle = [NSString stringWithUTF8String:arrValue->v.string];
                            break;
                        case RHO_PARAM_HASH:
                            for (int k = 0, limk = arrValue->v.hash->size; k < limk; ++k) {
                                char *sName = arrValue->v.hash->name[k];
                                rho_param *sValue = arrValue->v.hash->value[k];
                                if (sValue->type != RHO_PARAM_STRING) {
                                    RAWLOG_ERROR("Illegal type of button item's value");
                                    continue;
                                }
                                if (strcasecmp(sName, "id") == 0)
                                    itemId = [NSString stringWithUTF8String:sValue->v.string];
                                else if (strcasecmp(sName, "title") == 0)
                                    itemTitle = [NSString stringWithUTF8String:sValue->v.string];
                            }
                            break;
                        default:
                            RAWLOG_ERROR("Illegal type of button item");
                            continue;
                    }
                    
                    if (!itemId || !itemTitle) {
                        RAWLOG_ERROR("Incomplete button item");
                        continue;
                    }
                    
                    NSMutableArray *btn = [NSMutableArray arrayWithCapacity:2];
                    [btn addObject:itemId];
                    [btn addObject:itemTitle];
                    [buttons addObject:btn];
                }
            }

        }
    }
    rho_param_free(p);
    
    UIAlertView *alert = [[[UIAlertView alloc]
                  initWithTitle:title
                  message:message
                  delegate:self
                  cancelButtonTitle:nil
                  otherButtonTitles:nil] autorelease];
    
    for (int i = 0, lim = [buttons count]; i < lim; ++i) {
        NSArray *btn = [buttons objectAtIndex:i];
        NSString *title = [btn objectAtIndex:1];
        [alert addButtonWithTitle:title];
    }
    
    [alert show];
    currentAlert = alert;
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    if (!callback)
        return;
    
    if (buttonIndex < 0 || buttonIndex >= [buttons count])
        return;
    
    NSArray *btn = [buttons objectAtIndex:buttonIndex];
    NSString *itemId = [[btn objectAtIndex:0] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSString *itemTitle = [[btn objectAtIndex:1] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    
    rho_rhodesapp_callPopupCallback([callback UTF8String], [itemId UTF8String], [itemTitle UTF8String]);
    [self release];
    currentAlert = nil;
}

@end

@interface RhoAlertHidePopupTask : NSObject {}
+ (void)run;
@end

@implementation RhoAlertHidePopupTask
+ (void)run {
    if (!currentAlert)
        return;
    [currentAlert dismissWithClickedButtonIndex:-1 animated:NO];
    currentAlert = nil;
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

@implementation RhoAlert

+ (void)showPopup:(rho_param*)p {
    id runnable = [[RhoAlertShowPopupTask alloc] init];
    NSValue *value = [NSValue valueWithPointer:rho_param_dup(p)];
    [Rhodes performOnUiThread:runnable arg:value wait:NO];
}

+ (void)hidePopup {
    id runnable = [RhoAlertHidePopupTask class];
    [Rhodes performOnUiThread:runnable wait:NO];
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

void alert_show_popup(rho_param *p) {
    if (!rho_rhodesapp_check_mode())
        return;
    if (!p || (p->type != RHO_PARAM_STRING && p->type != RHO_PARAM_HASH)) {
        RAWLOG_ERROR("Alert.show_popup - wrong arguments");
        return;
	}
    
    [RhoAlert showPopup:p];
}

void alert_hide_popup() {
    if (!rho_rhodesapp_check_mode())
        return;
    [RhoAlert hidePopup];
}

void alert_vibrate(int duration) {
    if (!rho_rhodesapp_check_mode())
        return;
    [RhoAlert vibrate:duration];
}

void alert_play_file(char* file_name, char* media_type) {
    if (!rho_rhodesapp_check_mode())
        return;
    if (!file_name) {
        RAWLOG_ERROR("Alert.play_file - please specify file name to play");
        return;
    }
    
    [RhoAlert playFile:[NSString stringWithUTF8String:file_name]
            mediaType:media_type?[NSString stringWithUTF8String:media_type]:NULL];
}
