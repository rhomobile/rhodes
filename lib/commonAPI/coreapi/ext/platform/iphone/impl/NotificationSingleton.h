
#import "INotification.h"
#import "NotificationSingletonBase.h"



@interface NotificationSingleton : NotificationSingletonBase<INotificationSingleton> {
    
}



-(void) showPopup:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) hidePopup:(id<IMethodResult>)methodResult;
-(void) showStatus:(NSString*)title status_text:(NSString*)status_text hide_button_label:(NSString*)hide_button_label methodResult:(id<IMethodResult>)methodResult;
-(void) playFile:(NSString*)path media_type:(NSString*)media_type methodResult:(id<IMethodResult>)methodResult;
-(void) beep:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) vibrate:(int)duration methodResult:(id<IMethodResult>)methodResult;
-(void) setScheduler:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;

+(void) init_notification_object;


@end

