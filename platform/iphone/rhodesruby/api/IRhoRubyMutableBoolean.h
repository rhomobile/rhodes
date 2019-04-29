#import <Foundation/Foundation.h>
#import "IRhoRubyBoolean.h"


@protocol IRhoRubyMutableBoolean <IRhoRubyBoolean>

-(void) setBool:(BOOL)value;

@end

