#import <Foundation/Foundation.h>
#import "IRhoRubyObject.h"

@protocol IRhoRubyNativeCallback <NSObject>

-(void) onRubyNativeWithParam:(id<IRhoRubyObject>)param;

@end

