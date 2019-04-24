
#import "../api/IRhoRubyMutableInteger.h"
#import "RhoRubyObjectImpl.h"



@interface RhoRubyMutableIntegerImpl : RhoRubyObjectImpl<IRhoRubyMutableInteger> {
}
- (NSString*) getClassName;

-(long int) getLong;

-(void) setLong:(long int)value;

@end








