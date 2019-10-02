#import <Foundation/Foundation.h>
#import "IRhoRubyString.h"


@protocol IRhoRubyMutableString <IRhoRubyString>

-(void) setString:(NSString*)value;

@end

