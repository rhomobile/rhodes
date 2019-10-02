#import <Foundation/Foundation.h>
#import "IRhoRubyFloat.h"


@protocol IRhoRubyMutableFloat <IRhoRubyFloat>

-(void) setDouble:(double)value;

@end

