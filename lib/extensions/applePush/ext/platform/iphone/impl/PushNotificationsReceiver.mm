//
//  PushNotificationsListener.m
//  ApplePush
//
//  Created by Alex Epifanoff on 01.07.13.
//
//

#import "PushNotificationsReceiver.h"
#include "Push.h"

@implementation PushNotificationsReceiver

static PushNotificationsReceiver *instance = nil;

+ (id<IPushNotificationsReceiver>)sharedInstance {
    
    if (instance == nil) {
        instance = [[PushNotificationsReceiver alloc] init];
    }
    
    return instance;
}


- (void) onPushRegistrationSucceed:(NSData *)deviceToken
{
    NSLog(@"Device token is %@", deviceToken);
     
    NSMutableString *stringBuffer = [NSMutableString stringWithCapacity:([deviceToken length] * 2)];
    const unsigned char *dataBuffer = (const unsigned char*)[deviceToken bytes];
    for (int i = 0; i < [deviceToken length]; ++i) {
        [stringBuffer appendFormat:@"%02x", (unsigned int)dataBuffer[ i ]];
    }
    
    char* szpin = strdup([stringBuffer cStringUsingEncoding:[NSString defaultCStringEncoding]]);
    RAWLOG_INFO1("device pin: %s\n", szpin);
    
    
    rho::push::CPushManager::getInstance()->setDeviceId("apple",szpin);
    
    free(szpin);
}

- (void) onPushRegistrationFailed:(NSError *)error
{
    NSLog(@"Push Notification Error: %@", [error localizedDescription]);
}

+ (NSMutableString*) dictToJSON:(NSDictionary*)dict context:(NSMutableString*)context
{
    if (nil == context) {
        context = [[NSMutableString alloc] init];
    }
    
    [context appendString:@"{"];
    
    NSArray *keys = [dict allKeys];
    
    for ( int i = 0; i < [keys count]; ++ i ) {
        id k = keys[i];

        if (![k isKindOfClass:[NSString class]]) {
            NSLog(@"Non-string key type in push payload");
            continue;
		}
        
        [context appendFormat:@"\"%@\"",(NSString*)k];
        
        id val = [dict objectForKey:k];
        
        if ([val isKindOfClass:[NSString class]]) {
            [context appendFormat:@"\"%@\"",(NSString*)val];

		} else if ([val isKindOfClass:[NSNumber class]]) {
            [context appendFormat:@"\"%@\"",[(NSNumber*)val stringValue]];
        }
        else if ([val isKindOfClass:[NSDictionary class]]) {
            [context appendFormat:@"\"%@\"", [PushNotificationsReceiver dictToJSON:(NSDictionary*)val context:context] ];
        }
        
        if ( i < ([keys count]-1) ) {
            [context appendString:@","];
        }
    }
    
    [context appendString:@"}"];

    return context;
}


- (void) onPushMessageReceived:(NSDictionary *)userInfo
{
    NSMutableString* json = [[NSMutableString alloc] init];
    json = [PushNotificationsReceiver dictToJSON:userInfo context:json];
        
    rho::push::CPushManager::getInstance()->callBack("apple", [json UTF8String]);
    
    [json dealloc];
}




@end
