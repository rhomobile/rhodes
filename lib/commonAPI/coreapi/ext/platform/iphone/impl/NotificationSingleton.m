
#import "NotificationSingleton.h"
#import "Rhodes.h"

#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "common/RhodesApp.h"
#include "logging/RhoLog.h"
#import "common/RhoConf.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Alert"

#define NOTIFICATION_ID @"RhoNotification"

static UIAlertView *currentAlert = nil;
static BOOL is_current_alert_status = NO;
static BOOL granted_notification = NO;

@interface NotificationReminder : NSObject<UNUserNotificationCenterDelegate> {
    id<IMethodResult> callbackHolder;
    UNUserNotificationCenter* center;
    NSString* title;
    NSString* message;
    NSInteger hour;
    NSInteger minute;
    NSInteger seconds;
}

- (id)init;
- (void)setCallback:(id<IMethodResult>)value;
- (void)setReminder:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
- (void)dealloc;
- (void)removeScheduler;
- (bool)checkTime;
- (bool)isCurrentTime;
- (void)userNotificationCenter:(UNUserNotificationCenter *)center
          didReceiveNotificationResponse:(UNNotificationResponse *)response
          withCompletionHandler:(void (^)(void))completionHandler;
- (void)userNotificationCenter:(UNUserNotificationCenter *)center 
           willPresentNotification:(UNNotification *)notification 
           withCompletionHandler:(void (^)(UNNotificationPresentationOptions options))completionHandler;

+ (NotificationReminder*)instance;

@end

static NotificationReminder* reminder = nil;

@implementation NotificationReminder

- (id)init {
    callbackHolder = nil;
    center = [UNUserNotificationCenter currentNotificationCenter];
    center.delegate = self;
    hour = -1;
    minute = -1;
    seconds = -1;

    if(granted_notification == NO && center)
    {
        dispatch_semaphore_t question_semaphore = dispatch_semaphore_create(0);
        dispatch_async(dispatch_get_main_queue(), ^{
            [center requestAuthorizationWithOptions:(UNAuthorizationOptionAlert + UNAuthorizationOptionSound + UNAuthorizationOptionBadge)
                completionHandler:^(BOOL granted, NSError * _Nullable error) {
                granted_notification = granted;
                dispatch_semaphore_signal(question_semaphore);
            }];
        });
        dispatch_semaphore_wait(question_semaphore, DISPATCH_TIME_FOREVER);
        dispatch_release(question_semaphore);
    }

    return self;
}

+ (NotificationReminder*)instance {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        reminder = [[NotificationReminder alloc] init];
    });
    return reminder;
}

- (void)setCallback:(id<IMethodResult>)value {
    callbackHolder = value;
}

- (bool)checkTime {
    return (hour >= 0 && hour <= 23) && (minute >= 0 && minute <= 59) && (seconds >= 0 && seconds <= 59);
}

- (bool)isCurrentTime {
    return hour == -1 || minute == -1 || seconds == -1;
}

- (void)removeScheduler {
    @synchronized(self)
    {
        if(center)
        {
            //NSArray* ids = [NSArray arrayWithObjects:NOTIFICATION_ID];
            //[center removePendingNotificationRequestsWithIdentifiers:ids];
            [center removeAllPendingNotificationRequests];
        }
    }
}

- (void)dealloc {
    callbackHolder = nil;
    center = nil;
    title = @"";
    message = @"";
    reminder = nil;
    [super dealloc];
}

- (void)userNotificationCenter:(UNUserNotificationCenter *)center
          didReceiveNotificationResponse:(UNNotificationResponse *)response
          withCompletionHandler:(void (^)(void))completionHandler {
   if ([response.actionIdentifier isEqualToString:UNNotificationDismissActionIdentifier]) {
       // The user dismissed the notification without taking action.
   }
   else if ([response.actionIdentifier isEqualToString:UNNotificationDefaultActionIdentifier]) {
   }

    UNNotification *notification = response.notification;
    if (notification){
        NSDictionary *userInfo = ExtractUserInfo(notification);
        if (userInfo){
            [[Rhodes sharedInstance] processPushMessage:userInfo];
        }
    }
}

- (void)userNotificationCenter:(UNUserNotificationCenter *)center 
           willPresentNotification:(UNNotification *)notification 
           withCompletionHandler:(void (^)(UNNotificationPresentationOptions options))completionHandler 
{
    NSLog(@"Notification recived"); 

    NSDictionary *userInfo = ExtractUserInfo(notification);
    if (userInfo){
        NSDictionary *userInfo = notification.request.content.userInfo;
        [[Rhodes sharedInstance] processPushMessage:userInfo];
    }
    completionHandler( UNAuthorizationOptionSound);
}

NSDictionary * _Nullable ExtractUserInfo(UNNotification * _Nullable notification) {
    if (!notification) {
        NSLog(@"notification is nil");
        return nil;
    }
    UNNotificationRequest *request = notification.request;
    if (!request){
        NSLog(@"notification request is nil");
        return nil;
    }
    UNNotificationContent *content = request.content;
    if (!content) {
        NSLog(@"notification content is nil");
        return nil;
    }
    NSDictionary *userInfo = content.userInfo;
    if (!userInfo) {
        NSLog(@"notification userInfo is nil");
        return nil;
    }

    return userInfo;
}

-(void) setReminder:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {

    if(granted_notification == NO)
    {
        NSLog(@"Waning!!! Notification not granted!!!"); 
    }

    [self removeScheduler];

    NSInteger interval = 0;
    BOOL repeats = NO;

    if ([propertyMap isKindOfClass:[NSDictionary class]])
    {
        NSEnumerator* enumerator = [propertyMap keyEnumerator];
        NSObject* obj = nil;

        while ((obj = [enumerator nextObject]) != nil) 
        {
            NSString* objKey = (NSString*)obj;
            NSObject* objVal = [propertyMap objectForKey:objKey];

            if ([objKey isEqualToString:HK_TITLE]) {
                title = (NSString*)objVal;
            } else if ([objKey isEqualToString:HK_MESSAGE]) {
                message = (NSString*)objVal;            
            } else if ([objKey isEqualToString:HK_INTERVAL]) {
                interval = [(NSString*)objVal integerValue];
            } else if ([objKey isEqualToString:HK_REPEATS]) {
                repeats = [(NSString*)objVal boolValue];
            } else if ([objKey isEqualToString:HK_START]) 
            {
                if([objVal isKindOfClass:[NSDictionary class]])
                {
                    NSDictionary* timeMap = (NSDictionary*)objVal;

                    objVal = [timeMap objectForKey:HK_HOUR];
                    if(objVal && [objVal isKindOfClass:[NSNumber  class]])                
                        hour = [objVal integerValue];
                    else hour = -1;

                    objVal = [timeMap objectForKey:HK_MINUTE];
                    if(objVal && [objVal isKindOfClass:[NSNumber  class]])                
                        minute = [objVal integerValue];
                    else minute = -1;

                   objVal = [timeMap objectForKey:HK_SECONDS];
                   if(objVal && [objVal isKindOfClass:[NSNumber  class]])                
                        seconds = [objVal integerValue];
                   else seconds = -1;
                }
                else
                {
                    hour = -1;
                    minute = -1;
                    seconds = -1;
                }
            }
        }
    }

    UNMutableNotificationContent* content = [[UNMutableNotificationContent alloc] init];
    content.title = title;//[NSString localizedUserNotificationStringForKey:title arguments:nil];
    content.body = message;//[NSString localizedUserNotificationStringForKey:message arguments:nil];
    content.sound = [UNNotificationSound defaultSound];

    UNNotificationTrigger* trigger = nil;

    if(![self isCurrentTime])
    {
        if(![self checkTime])
        {
            NSLog(@"Ivalid parameter for time!!!");
            return;
        }
        NSDateComponents* date = [[NSDateComponents alloc] init];
        date.hour = hour;
        date.minute = minute;
        date.second = seconds;
        trigger = [UNCalendarNotificationTrigger triggerWithDateMatchingComponents:date repeats:repeats];
        [date dealloc];
    }
    else
    {
        trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:interval repeats: repeats];
    }
    
    UNNotificationRequest* request = [UNNotificationRequest
       requestWithIdentifier: NOTIFICATION_ID content:content trigger:trigger];

    [center addNotificationRequest:request withCompletionHandler:^(NSError * _Nullable error) {   
        if (error != nil) {       
            NSLog(@"%@", error.localizedDescription);        
            }
    }];

}

@end

@interface AlertShowPopupTask : NSObject<UIAlertViewDelegate> {
    NSMutableArray *buttons;
    id<IMethodResult> callbackHolder;
}

@property (nonatomic,retain) NSMutableArray *buttons;

- (id)init;
- (void)dealloc;
- (void)setCallback:(id<IMethodResult>)value;
- (void)run:(NSValue*)v;
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

@end

@implementation AlertShowPopupTask

@synthesize buttons;

- (id)init {
    callbackHolder = nil;
    buttons = nil;
    return self;
}

- (void)dealloc {
    callbackHolder = nil;
    self.buttons = nil;
    [super dealloc];
}

- (void)setCallback:(id<IMethodResult>)value {
    callbackHolder = value;
}

- (void)run:(NSObject*)v {
    
    NSString *title = @"Alert";
    NSString *message = @"";
    NSString *imagePath = nil;
    
	is_current_alert_status = NO;
	
    self.buttons = [NSMutableArray arrayWithCapacity:1];
    
    if ([v isKindOfClass:[NSString class]])
    {
        message = (NSString*)v;
        [buttons addObject:[NSMutableArray arrayWithObjects:@"OK", @"OK", nil]];
    }
    else if ([v isKindOfClass:[NSDictionary class]]) {
        NSDictionary* dict = (NSDictionary*)v;
        // dictionary
        NSEnumerator* enumerator = [dict keyEnumerator];
        NSObject* obj = nil;
        
        while ((obj = [enumerator nextObject]) != nil) {
            NSString* objKey = (NSString*)obj;
            NSObject* objVal = [dict objectForKey:objKey];
            NSString* objStr = @"";
            if ([objVal isKindOfClass:[NSString class]])
            {
                objStr = (NSString*)objVal;
            }
            
            if ([objKey isEqualToString:HK_TITLE]) {
                title = objStr;
            } else if ([objKey isEqualToString:HK_MESSAGE]) {
                message = objStr;
            } else if ([objKey isEqualToString:HK_ICON]) {
                imagePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:objStr];
            } else if ([objKey isEqualToString:@"status_type"]) {
                is_current_alert_status = YES;
            }
            else if ([objKey isEqualToString:HK_BUTTONS]) {
                
                if (![objVal isKindOfClass:[NSArray class]]) {
                    RAWLOG_ERROR("'buttons' should be array");
                    continue;
                }
                
                NSArray* arr = (NSArray*) objVal;
                
                for(id buttonVal in arr) {
                    NSString *itemId = nil;
                    NSString *itemTitle = nil;
                    
                    if ([buttonVal isKindOfClass:[NSString class]]) {
                        if ([(NSString*)buttonVal length] > 0)  {
                            itemId = (NSString*) buttonVal;
                            itemTitle = (NSString*) buttonVal;
                        }
                    } else if ([buttonVal isKindOfClass:[NSDictionary class]]) {
                        NSDictionary* btnDict = (NSDictionary*)buttonVal;
                        // get ID
                        NSObject* tmp = [btnDict objectForKey:@"id"];
                        if ([tmp isKindOfClass:[NSString class]]) {
                            itemId = (NSString*) tmp;
                        } else {
                            RAWLOG_ERROR("Illegal type of button id");
                        }
                        // get Titile
                        tmp = [btnDict objectForKey:@"title"];
                        if ([tmp isKindOfClass:[NSString class]]) {
                            itemTitle = (NSString*) tmp;
                        } else {
                            RAWLOG_ERROR("Illegal type of button title");
                        }
                    } else {
                        RAWLOG_ERROR("Illegal type of button item");
                    }
                    
                    if ((itemId == nil) || (itemTitle == nil)) {
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
        if ([buttons count] <= 0) {
            [buttons addObject:[NSMutableArray arrayWithObjects:@"OK", @"OK", nil]];
        }
    }
    [v release];
    
	if ((currentAlert != nil) && (is_current_alert_status)) {
		currentAlert.message = message;
		return;
	}
	
    UIAlertView *alert = [[[UIAlertView alloc]
                           initWithTitle:title
                           message:message
                           delegate:self
                           cancelButtonTitle:nil
                           otherButtonTitles:nil] autorelease];
    

    // please note that there are no easy way to add image to popup
    // following code fails to addSubView
    /*if (imagePath) {
        
        UIImageView *tempImageView = [[UIImageView alloc]initWithFrame:CGRectMake(20,20,50,50)];
        tempImageView.image = [UIImage imageNamed:imagePath];
        [alert addSubView:tempImageView];
    }*/
        
        
    for (int i = 0, lim = [buttons count]; i < lim; ++i) {
        NSArray *btn = [buttons objectAtIndex:i];
        NSString *title = [btn objectAtIndex:1];
        [alert addButtonWithTitle:title];
    }
    
    [alert show];

    currentAlert = alert;
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    currentAlert = nil;
	is_current_alert_status = NO;
    
    if (!callbackHolder)
        return;
    
    if (buttonIndex < 0 || buttonIndex >= [buttons count])
        return;
    
    NSArray *btn = [buttons objectAtIndex:buttonIndex];
    NSString *itemId = [[btn objectAtIndex:0] stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLFragmentAllowedCharacterSet]];
    NSString *itemTitle = [[btn objectAtIndex:1] stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLFragmentAllowedCharacterSet]];
    
    NSMutableDictionary* callbackResult = [NSMutableDictionary dictionaryWithObjectsAndKeys:itemId, HK_BUTTON_ID, itemTitle, HK_BUTTON_TITLE, [NSNumber numberWithInt:buttonIndex], HK_BUTTON_INDEX, nil];
    
    [callbackHolder setResult:callbackResult];
    [self release];
}

@end

@interface AlertHidePopupTask : NSObject {}
+ (void)run;
@end

@implementation AlertHidePopupTask
+ (void)run {
    if (!currentAlert)
        return;
    [currentAlert dismissWithClickedButtonIndex:-1 animated:NO];
    currentAlert = nil;
	is_current_alert_status = NO;
}
@end


@interface AlertVibrateTask : NSObject {}
+ (void)run;
@end

@implementation AlertVibrateTask
+ (void)run {
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}
@end

@interface AlertPlayFileTask : NSObject {}
+ (void)run:(NSString*)file :(NSString*)type;
@end

@implementation AlertPlayFileTask
+ (void)run:(NSString*)file :(NSString*)type {
    [[Rhodes sharedInstance] playStart:file mediaType:type];
}
@end

@implementation NotificationSingleton

-(void) setScheduler:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult
{
    NotificationReminder* scheduler = [NotificationReminder instance];
    [scheduler setReminder:propertyMap methodResult:methodResult];
}

-(void) removeScheduler:(id<IMethodResult>)methodResult;
{
    NotificationReminder* scheduler = [NotificationReminder instance];
    [scheduler removeScheduler];
}

-(void) showPopup:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    
    id runnable = [[AlertShowPopupTask alloc] init];
    [propertyMap retain];
    if ([methodResult hasCallback])
    {
        [runnable setCallback:methodResult];
    }
    [Rhodes performOnUiThread:runnable arg:propertyMap wait:NO];
}

-(void) hidePopup:(id<IMethodResult>)methodResult {
    id runnable = [AlertHidePopupTask class];
    [Rhodes performOnUiThread:runnable wait:NO];
}

-(void) showStatus:(NSString*)title status_text:(NSString*)status_text hide_button_label:(NSString*)hide_button_label methodResult:(id<IMethodResult>)methodResult{
    
    NSMutableDictionary* params = [NSMutableDictionary dictionaryWithObjectsAndKeys:title, HK_TITLE, status_text, HK_MESSAGE, [NSNumber numberWithBool:true], @"status_type", [NSMutableArray arrayWithObject:hide_button_label], @"buttons",  nil];
    
    [params retain];
    id runnable = [[AlertShowPopupTask alloc] init];
    [Rhodes performOnUiThread:runnable arg:params wait:NO];
}

-(void) playFile:(NSString*)path media_type:(NSString*)media_type methodResult:(id<IMethodResult>)methodResult{
    id runnable = [AlertPlayFileTask class];
    [Rhodes performOnUiThread:runnable arg:path arg:media_type wait:NO];
}

-(void) beep:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult{
    // NYI
}

-(void) vibrate:(int)duration methodResult:(id<IMethodResult>)methodResult{
    id runnable = [AlertVibrateTask class];
    [Rhodes performOnUiThread:runnable wait:NO];
}


+(void) init_notification_object {
    if (rho_conf_is_property_exists("ios_request_notification_permission_on_start")!=0) {
        if (rho_conf_getBool("ios_request_notification_permission_on_start")!=0 ) {
            NotificationReminder* rem = [NotificationReminder instance];
        }
    }
}

@end
