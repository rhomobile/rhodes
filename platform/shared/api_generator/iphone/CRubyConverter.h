
#import <Foundation/Foundation.h>
#import "IMethodResult.h"
#import "ruby/ext/rho/rhoruby.h"




@interface CRubyConverter : NSObject {
}

+ (VALUE) convertToRuby:(NSObject*)objectiveC_value;

+ (NSObject*) convertFromRuby:(VALUE)ruby_value;

@end
