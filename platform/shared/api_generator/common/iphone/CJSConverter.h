
#import <Foundation/Foundation.h>
#import "IMethodResult.h"
#import "ruby/ext/rho/rhoruby.h"

@interface CJSConverter : NSObject {
}

+ (NSString*) convertToJS:(NSObject*)objectiveC_value;

+ (NSObject*) convertFromJS:(NSString*)js_value;

@end
