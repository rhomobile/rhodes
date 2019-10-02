
#import "../api/IRhoRubyMutableFloat.h"
#import "RhoRubyObjectImpl.h"



@interface RhoRubyMutableFloatImpl : RhoRubyObjectImpl<IRhoRubyMutableFloat> {
}
- (NSString*) getClassName;

-(double) getDouble;

-(void) setDouble:(double)value;

@end








