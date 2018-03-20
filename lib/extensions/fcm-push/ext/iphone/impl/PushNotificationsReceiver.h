//
//  PushNotificationsListener.h
//  ApplePush
//
//  Created by Alex Epifanoff on 01.07.13.
//
//

#import <Foundation/Foundation.h>
#import "IPushNotificationsReceiver.h"

@interface PushNotificationsReceiver : NSObject<IPushNotificationsReceiver>

+ (id<IPushNotificationsReceiver>)sharedInstance;

- (void) onPushRegistrationSucceed:(NSData *)deviceToken;
- (void) onPushRegistrationFailed:(NSError *)error;
- (void) onPushMessageReceived:(NSDictionary *)userInfo;


@end
