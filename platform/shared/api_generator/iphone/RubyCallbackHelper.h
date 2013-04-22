#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "CMethodResult.h"


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void RubyCallbackHelper_callCallback(NSString* callbackURL,
                                     unsigned long callbackMethod,
                                     CMethodResult* methodResult,
                                     NSString* callbackParam
                                     );
    

    void RubyCallbackHelper_holdRubyValue(unsigned long value);
    void RubyCallbackHelper_releaseRubyValue(unsigned long value);
    
#ifdef __cplusplus
}
#endif //__cplusplus
