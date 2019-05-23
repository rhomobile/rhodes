#import <Foundation/Foundation.h>
#import "IRhoRubyObject.h"


@protocol IRhoRubyArray <IRhoRubyObject>

-(int) getItemsCount;

-(id<IRhoRubyObject>) getItemByIndex:(int)index;

@end

