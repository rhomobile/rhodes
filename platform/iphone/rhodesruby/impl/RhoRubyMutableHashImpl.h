
#import "../api/IRhoRubyMutableHash.h"
#import "RhoRubyObjectImpl.h"



@interface RhoRubyMutableHashImpl : RhoRubyObjectImpl<IRhoRubyMutableHash> {
}
- (NSString*) getClassName;


// return array of IRhoRubyString
-(id<IRhoRubyArray>) getKeys;

-(id<IRhoRubyObject>) getItemByKey:(NSString*)key;

-(void)addItemWithKey:(NSString*)key item:(id<IRhoRubyObject>)item;

@end





