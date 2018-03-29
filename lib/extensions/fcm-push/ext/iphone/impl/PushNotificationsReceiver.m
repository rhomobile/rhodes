//
//  PushNotificationsListener.m
//  ApplePush
//
//  Created by Alex Epifanoff on 01.07.13.
//
//

#import "PushNotificationsReceiver.h"

@implementation PushNotificationsReceiverBase

- (void)messaging:(FIRMessaging *)messaging didReceiveRegistrationToken:(NSString *)fcmToken {
    NSLog(@"FCM registration token: %@", fcmToken);
}

- (void)messaging:(FIRMessaging *)messaging didReceiveMessage:(FIRMessagingRemoteMessage *)remoteMessage {
    NSLog(@"FCM Received data message: %@", remoteMessage.appData);
}
    
- (void)messaging:(FIRMessaging *)messaging didRefreshRegistrationToken:(NSString *)fcmToken {
        NSLog(@"FCM refresh token: %@", fcmToken);
}
    

@end

