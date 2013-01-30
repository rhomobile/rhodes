
#import <Foundation/Foundation.h>
#import "IMethodResult.h"


@interface CJSConverter : NSObject {
}

+ (NSString*) convertToJS:(NSObject*)objectiveC_value;

+ (NSObject*) convertFromJS:(void*)json_entry;

+ (NSObject*) getObjectiveCNULL;

@end
