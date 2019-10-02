#import <Foundation/Foundation.h>
#import "IRhoRubyObject.h"
#import "IRhoRubyArray.h"


@protocol IRhoRubyHash <IRhoRubyObject>

// return array of IRhoRubyString
-(id<IRhoRubyArray>) getKeys;

-(id<IRhoRubyObject>) getItemByKey:(NSString*)key;

@end


