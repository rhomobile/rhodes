//
//  PushNotificationsListener.m
//  ApplePush
//
//  Created by Alex Epifanoff on 01.07.13.
//
//

#import "PushNotificationsReceiver.h"
#include "Push.h"
#import "Firebase.h"

@implementation PushNotificationsReceiver

static PushNotificationsReceiver *instance = nil;

+ (id<IPushNotificationsReceiver>)sharedInstance {
    
    if (instance == nil) {
        instance = [[PushNotificationsReceiver alloc] init];
    }

    [FIRApp configure];
    [FIRMessaging messaging].delegate = self;
    [FIRMessaging messaging].shouldEstablishDirectChannel = true;
    
    return instance;
}


- (void) onPushRegistrationSucceed:(NSData *)deviceToken
{
    NSLog(@"Device token is %@", deviceToken);

    [FIRMessaging messaging].APNSToken = deviceToken;
     
    NSMutableString *stringBuffer = [NSMutableString stringWithCapacity:([deviceToken length] * 2)];
    const unsigned char *dataBuffer = (const unsigned char*)[deviceToken bytes];
    for (int i = 0; i < [deviceToken length]; ++i) {
        [stringBuffer appendFormat:@"%02x", (unsigned int)dataBuffer[ i ]];
    }
    
    char* szpin = strdup([stringBuffer cStringUsingEncoding:[NSString defaultCStringEncoding]]);
    RAWLOG_INFO1("device pin: %s\n", szpin);
    
    rho::push::CPushManager::getInstance()->setDeviceId("google",szpin);
    
    free(szpin);
}

//- (void)messaging:(FIRMessaging *)messaging didReceiveRegistrationToken:(NSString *)fcmToken {
//    NSLog(@"FCM registration token: %@", fcmToken);
    
    // TODO: If necessary send token to application server.
    // Note: This callback is fired at each app startup and whenever a new token is generated.
//}

- (void) onPushRegistrationFailed:(NSError *)error
{
    NSLog(@"Push Notification Error: %@", [error localizedDescription]);
}

+ (NSDictionary*)ansMessageToRhodesMessage:(NSDictionary*)dict
{
    NSMutableDictionary* ret = [[NSMutableDictionary alloc] init];
    
    NSArray *keys = [dict allKeys];
    
    for ( id key in keys ) {
        if (![key isKindOfClass:[NSString class]]) {
            NSLog(@"Non-string key type in push payload");
            continue;
		}
        
        NSString* sKey = (NSString*) key;
        id val = [dict objectForKey:key];
        
        if ( [sKey isEqualToString:@"aps"] && [val isKindOfClass:[NSDictionary class]] ) {
            [ret addEntriesFromDictionary:val];
        } else {
            [ret setValue:val forKey:sKey];
        }
    }
    
    return ret;
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
        
        [context appendFormat:@"\"%@\":",(NSString*)k];
        
        id val = [dict objectForKey:k];
        
        if ([val isKindOfClass:[NSString class]]) {
            [context appendFormat:@"\"%@\"",(NSString*)val];

		} else if ([val isKindOfClass:[NSNumber class]]) {
            [context appendFormat:@"\"%@\"",[(NSNumber*)val stringValue]];
        } else if ([val isKindOfClass:[NSDictionary class]]) {
            NSMutableString* json = [[NSMutableString alloc] init];
            [PushNotificationsReceiver dictToJSON:(NSDictionary*)val context:json];
            [context appendString:json];
            [json dealloc];
        } else if ([val isKindOfClass:[NSArray class]]) {
            
            [context appendString:@"\""];
            //will only support String values in array
            NSArray* vals = (NSArray*)val;
            for ( int j = 0; j < ([vals count]); ++j ) {
                id v = vals[j];
                if ([v isKindOfClass:[NSString class]]) {
                    [context appendFormat:@"%@",(NSString*)v];

                    if ( j < ([vals count]-1) ) {
                        [context appendString:@","];
                    }
                }
            }
            
            [context appendString:@"\""];
            
        } else {
            [context appendString:@"\"\""];
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
    //[[FIRMessaging messaging] appDidReceiveMessage:userInfo];
    NSMutableString* json = [[NSMutableString alloc] init];
    NSDictionary* processedMessage = [PushNotificationsReceiver ansMessageToRhodesMessage:userInfo];
    
    json = [PushNotificationsReceiver dictToJSON:processedMessage context:json];
    
    rho::push::CPushManager::getInstance()->callBack("google", [json UTF8String]);
    
    [processedMessage dealloc];
    [json dealloc];
}




@end
