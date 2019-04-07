
#import "../api/IRhoRubyMutableBoolean.h"
#import "RhoRubyObjectImpl.h"



@interface RhoRubyMutableBooleanImpl : RhoRubyObjectImpl<IRhoRubyMutableBoolean> {
}
- (NSString*) getClassName;

-(BOOL) getBool;

-(void) setBool:(BOOL)value;

@end








