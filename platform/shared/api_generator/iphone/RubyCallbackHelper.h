#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "CMethodResult.h"


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void RubyCallbackHelper_callCallback(NSString* callbackURL,
                                     CMethodResult* methodResult,
                                     NSString* callbackParam
                                     );
    

#ifdef __cplusplus
}
#endif //__cplusplus
