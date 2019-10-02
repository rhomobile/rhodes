
#import "../api/IRhoRubyMutableString.h"
#import "RhoRubyObjectImpl.h"



@interface RhoRubyMutableStringImpl : RhoRubyObjectImpl<IRhoRubyMutableString> {
}
- (NSString*) getClassName;

-(NSString*) getString;

-(void) setString:(NSString*)value;

@end








