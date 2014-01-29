
#import "IntentSingleton.h"

#include "logging/RhoLog.h"

#import "api_generator/iphone/CRubyConverter.h"
#import "api_generator/iphone/CMethodResult.h"
#import "Rhodes.h"

@implementation IntentSingleton

- (id) init {
    self = [super init];
    
    if (self)
    {
        if ([Rhodes sharedInstance])
        {
            [[Rhodes sharedInstance] setAppMessageReceiver:self];
        }
    }
    
    return self;
}

- (void) onAppMessageReceived:(NSString *)message app:(NSString *)app
{
    if (mIntentListener)
    {
        NSMutableDictionary* dict = [[NSMutableDictionary alloc] init];
        
        NSArray* urlAndQuery = [message componentsSeparatedByString:@"?"];
        
        if ([urlAndQuery count]>1)
        {
            NSArray* tokens = [urlAndQuery[1] componentsSeparatedByString:@"&"];
            
            for (NSString* token in tokens)
            {
                NSArray* argAndValue = [token componentsSeparatedByString:@"="];
                if ( [argAndValue count]==2 )
                {
                    [dict setValue: argAndValue[1] forKey:argAndValue[0]];
                }
            }
        }
        
        [mIntentListener setResult: dict];
    }
}


-(void) send:(NSDictionary*)params methodResult:(id<IMethodResult>)methodResult {

    NSObject* o;
    
    NSString* appName   = [o=[params objectForKey:@"appName"]isKindOfClass:[NSString class]]?(NSString*)o:@"";
    NSString* uri       = [o=[params objectForKey:@"uri"]isKindOfClass:[NSString class]]?(NSString*)o:@"";
    NSString* mimeType  = [o=[params objectForKey:@"mimeType"]isKindOfClass:[NSString class]]?(NSString*)o:@"";
    NSString* data      = [o=[params objectForKey:@"data"]isKindOfClass:[NSString class]]?(NSString*)o:@"";
    
    NSString* strFullUrl   = [NSString stringWithFormat:@"%s://?appName=%s&uri=%s&mimeType=%s&data=%s",
                            [appName UTF8String],
                            [appName UTF8String],
                            [uri UTF8String],
                            [mimeType UTF8String],
                            [data UTF8String]
                        ];
    
    NSURL* fullUrl = [NSURL URLWithString:[strFullUrl stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    
    RAWLOG_INFO1("IntentSingleton::send: %s", [strFullUrl UTF8String]);
	    
    BOOL res = FALSE;
    
    if ([[UIApplication sharedApplication] canOpenURL:fullUrl]) {
        res = [[UIApplication sharedApplication] openURL:fullUrl];
    }
    
    NSMutableDictionary* ret = [[NSMutableDictionary alloc] initWithDictionary:params];
	
    if ( res) {
        RAWLOG_INFO("sendApplicationMessage suceeded.");
    } else {
        RAWLOG_INFO("sendApplicationMessage failed.");
        [ret setValue: res forKey:@"responseCode"];
    }
    
    [methodResult setResult: ret];
    

}

-(void) startListening:(id<IMethodResult>)methodResult {
    mIntentListener = methodResult;
}

-(void) stopListening:(id<IMethodResult>)methodResult {
    mIntentListener = nil;
}




@end