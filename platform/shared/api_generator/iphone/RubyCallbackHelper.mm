
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
                                     unsigned long callbackMethod,
                                        CMethodResult* methodResult,
                                        NSString* callbackParam
                                    ) {
    
    
	NSString* strBody = @"";//@"&rho_callback=1";
	
    const char* param_name = "result";
    if (callbackMethod != 0) {
        param_name = "body";
    }
    
    
    if ([methodResult isHash] && (callbackMethod == 0)) {
        NSDictionary* hash = [methodResult getHashResult];
        NSArray* keys = [hash allKeys];
        int i;
        for (i = 0; i < [keys count]; i++) {
            NSString* key = (NSString*)[keys objectAtIndex:i];
            NSObject* value = [hash objectForKey:key];
            CMethodResult* mr = [[CMethodResult alloc] init];
            [mr setResult:value];
            NSString* strParam = [NSString stringWithUTF8String:( RHODESAPP().addCallbackObject( new BarcodeRhoCallbackResultContainer(mr), [key UTF8String]).c_str() ) ];
            strBody = [strBody stringByAppendingString:@"&"];
            strBody = [strBody stringByAppendingString:strParam];
        }
        //[methodResult release];
    }
    else {
        NSString* strParam = [NSString stringWithUTF8String:( RHODESAPP().addCallbackObject( new BarcodeRhoCallbackResultContainer([methodResult retain]), param_name).c_str() ) ];
        //strBody = [strBody stringByAppendingString:@"&"];
        strBody = [strBody stringByAppendingString:strParam];
    }
    
    //if (callbackParam != nil) {
    //    strBody = [strBody stringByAppendingString:callbackParam];
    //}
    
    rho::String callbackParamStr = "";
    if (callbackParam != nil) {
        callbackParamStr = [callbackParam UTF8String];
    }

    if (callbackMethod != 0) {
        VALUE oProc = callbackMethod;
        RHODESAPP().callCallbackProcWithData( oProc, [strBody UTF8String], callbackParamStr, true);
        
    }
    else {
        RHODESAPP().callCallbackWithData( [callbackURL UTF8String], [strBody UTF8String], callbackParamStr, true);
        //const char* cb = [callbackURL UTF8String];
        //const char* b = [strBody UTF8String];
        //char* norm_url = rho_http_normalizeurl(cb);
        
        //rho_net_request_with_data(rho_http_normalizeurl(cb), b);
        
        //rho_http_free(norm_url);
    }
    
    
    
}
    
    void RubyCallbackHelper_holdRubyValue(unsigned long value) {
        rho_ruby_holdValue(value);
    }
    
    void RubyCallbackHelper_releaseRubyValue(unsigned long value) {
        rho_ruby_releaseValue(value);
    }
   
	
#ifdef __cplusplus
	}
#endif //__cplusplus
	

