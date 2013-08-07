
#import "CJSEntryPoint.h"
#import "api_generator/js_helpers.h"
#import "common/RhodesApp.h"
#import "net/URI.h"


#import "json/JSONIterator.h"

@implementation CJSEntryPoint



+ (NSString*) js_entry_point:(NSString*)query {
    rho::String res = rho::apiGenerator::js_entry_point([query UTF8String]);
    return [NSString stringWithUTF8String:res.c_str()];
}

@end
