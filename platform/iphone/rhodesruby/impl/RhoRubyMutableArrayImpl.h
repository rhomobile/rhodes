
#import "../api/IRhoRubyMutableArray.h"
#import "RhoRubyObjectImpl.h"



@interface RhoRubyMutableArrayImpl : RhoRubyObjectImpl<IRhoRubyMutableArray> {
}
- (NSString*) getClassName;


-(int) getItemsCount;

-(id<IRhoRubyObject>) getItemByIndex:(int)index;

-(void)addItem:(id<IRhoRubyObject>)item;


@end





