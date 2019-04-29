#import <Foundation/Foundation.h>
#import "IRhoRubyInteger.h"


@protocol IRhoRubyMutableInteger <IRhoRubyInteger>

-(void) setLong:(long int)value;

@end

