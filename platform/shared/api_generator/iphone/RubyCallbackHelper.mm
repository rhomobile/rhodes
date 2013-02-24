
#import "RubyCallbackHelper.h"

#import "rubyext/WebView.h"
#import "common/RhodesApp.h"
#include "ruby/ext/rho/rhoruby.h"
#include <string.h>

class BarcodeRhoCallbackResultContainer : public rho::ICallbackObject
{
public:
	BarcodeRhoCallbackResultContainer(CMethodResult* methodResult)
	{
		mMethodResult = methodResult;
	}
    
	virtual ~BarcodeRhoCallbackResultContainer() {
		[mMethodResult release];
	}
	
	// return Ruby object
	virtual unsigned long getObjectValue() {
		return [mMethodResult toRuby];
	}
	
private:
	CMethodResult*	mMethodResult;
	
};


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus


void RubyCallbackHelper_callCallback(NSString* callbackURL,
                                        CMethodResult* methodResult,
                                        NSString* callbackParam
                                    ) {
    
    
	NSString* strBody = @"&rho_callback=1";
	
    strBody = [strBody stringByAppendingString:@"&"];
    
    if ([methodResult isHash]) {
        NSDictionary* hash = [methodResult getHashResult];
        NSArray* keys = [hash allKeys];
        int i;
        for (i = 0; i < [keys count]; i++) {
            NSString* key = (NSString*)[keys objectAtIndex:i];
            NSObject* value = [hash objectForKey:key];
            CMethodResult* mr = [[CMethodResult alloc] init];
            [mr setResult:value];
            NSString* strParam = [NSString stringWithUTF8String:( RHODESAPP().addCallbackObject( new BarcodeRhoCallbackResultContainer(mr), [key UTF8String]).c_str() ) ];
            strBody = [strBody stringByAppendingString:strParam];
        }
        [methodResult release];
    }
    else {
        NSString* strParam = [NSString stringWithUTF8String:( RHODESAPP().addCallbackObject( new BarcodeRhoCallbackResultContainer(methodResult), "result").c_str() ) ];
        strBody = [strBody stringByAppendingString:strParam];
    }
    
    if (callbackParam != nil) {
        strBody = [strBody stringByAppendingString:callbackParam];
    }
    
    const char* cb = [callbackURL UTF8String];
	const char* b = [strBody UTF8String];
    char* norm_url = rho_http_normalizeurl(cb);
    
	rho_net_request_with_data(rho_http_normalizeurl(cb), b);
    
    rho_http_free(norm_url);
    
    
    
    
}
    
   
	
#ifdef __cplusplus
	}
#endif //__cplusplus
	

