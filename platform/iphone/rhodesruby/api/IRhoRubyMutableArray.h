#import <Foundation/Foundation.h>
#import "IRhoRubyArray.h"


@protocol IRhoRubyMutableArray <IRhoRubyArray>

-(void)addItem:(id<IRhoRubyObject>)item;

@end
