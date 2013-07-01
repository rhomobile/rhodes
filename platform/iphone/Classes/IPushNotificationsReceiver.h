//
//  IPushNotificationsReceiver.h
//  rhorunner
//
//  Created by Alex Epifanoff on 01.07.13.
//
//

#import <Foundation/Foundation.h>

@protocol IPushNotificationsReceiver <NSObject>

- (void) onPushRegistrationSucceed:(NSData *)deviceToken;
- (void) onPushRegistrationFailed:(NSError *)error;
- (void) onPushMessageReceived:(NSDictionary *)userInfo;

@end
