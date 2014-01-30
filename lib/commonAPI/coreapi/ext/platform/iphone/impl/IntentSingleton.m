
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

-(NSString*) safeGetDictString:(NSString*)key dict:(NSDictionary*)dict defaultVal:(NSString*)defaultVal {

    if ((dict!=[NSNull null])&&([dict count]>0)){
        id o = [dict objectForKey:key];
        if ( (o!=nil) && ( [o isKindOfClass:[NSString class]]) ) {
            return (NSString*)o;
        }
    }
    
    return defaultVal;
}


-(void) send:(NSDictionary*)params methodResult:(id<IMethodResult>)methodResult {

    NSString* appName   = [self safeGetDictString:@"appName" dict:params defaultVal:@""];
    NSString* uri       = [self safeGetDictString:@"uri" dict:params defaultVal:@""];
    NSString* mimeType  = [self safeGetDictString:@"mimeType" dict:params defaultVal:@""];
    NSString* data      = [self safeGetDictString:@"data" dict:params defaultVal:@""];
    
    BOOL res = FALSE;
    
    if ( appName.length == 0 ) {
    
        NSURL* url = [NSURL URLWithString:[uri stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
        if ([[UIApplication sharedApplication] canOpenURL:url]) {
            res = [[UIApplication sharedApplication] openURL:url];
        }
    } else {
    
        NSString* strFullUrl   = [NSString stringWithFormat:@"%s://?appName=%s&uri=%s&mimeType=%s&data=%s",
                                  [appName UTF8String],
                                  [appName UTF8String],
                                  [uri UTF8String],
                                  [mimeType UTF8String],
                                  [data UTF8String]
                                  ];
        
        NSURL* fullUrl = [NSURL URLWithString:[strFullUrl stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
        
        RAWLOG_INFO1("IntentSingleton::send: %s", [strFullUrl UTF8String]);
        
        
        if ([[UIApplication sharedApplication] canOpenURL:fullUrl]) {
            res = [[UIApplication sharedApplication] openURL:fullUrl];
        }
    }
    
    NSMutableDictionary* ret = nil;
    if ([NSNull null]==params) {
        ret = [[NSMutableDictionary alloc] init];
    } else {
        ret = [[NSMutableDictionary alloc] initWithDictionary:params];
    }
	
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