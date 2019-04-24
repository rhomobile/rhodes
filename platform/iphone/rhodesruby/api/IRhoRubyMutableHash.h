#import <Foundation/Foundation.h>
#import "IRhoRubyHash.h"


@protocol IRhoRubyMutableHash <IRhoRubyHash>

-(void)addItemWithKey:(NSString*)key item:(id<IRhoRubyObject>)item;

@end

