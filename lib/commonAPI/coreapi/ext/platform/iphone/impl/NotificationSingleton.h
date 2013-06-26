
#import "INotification.h"
#import "NotificationSingletonBase.h"



@interface NotificationSingleton : NotificationSingletonBase<INotificationSingleton> {
    
}



-(void) showPopup:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) hidePopup;
-(void) showStatus:(NSString*)title status_text:(NSString*)status_text hide_button_label:(NSString*)hide_button_label;
-(void) playFile:(NSString*)path media_type:(NSString*)media_type;
-(void) beep:(NSDictionary*)propertyMap;
-(void) vibrate:(int)duration;




@end