//
//  PushNotificationsListener.h
//  fcm-push
//
//  Created by Alexei Shishkin on 21.03.18.
//
//

#import <Foundation/Foundation.h>
#import "IPushNotificationsReceiver.h"
#import "Firebase.h"
//@import Firebase;

@interface PushNotificationsReceiver : NSObject<IPushNotificationsReceiver, FIRMessagingDelegate>
{
  BOOL isDirectMessage;
}

+ (id<IPushNotificationsReceiver>)sharedInstance;

- (void) onPushRegistrationSucceed:(NSData *)deviceToken;
- (void) onPushRegistrationFailed:(NSError *)error;
- (void) onPushMessageReceived:(NSDictionary *)userInfo;
- (void) messaging:(FIRMessaging *)messaging didReceiveRegistrationToken:(NSString *)fcmToken;
//- (void) messaging:(FIRMessaging *)messaging didReceiveMessage:(FIRMessagingRemoteMessage *)remoteMessage;
- (FIRMessagingMessageInfo *)appDidReceiveMessage:(NSDictionary *)message;
//- (void) messaging:(FIRMessaging *)messaging didRefreshRegistrationToken:(NSString *)fcmToken;
@end


