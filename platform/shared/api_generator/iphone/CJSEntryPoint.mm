
#import "CJSEntryPoint.h"
#import "api_generator/js_helpers.h"
#import "common/RhodesApp.h"
#import "net/URI.h"


#import "json/JSONIterator.h"

@implementation CJSEntryPoint



+ (NSString*) js_entry_point:(NSString*)query {
	rho::String data = rho::net::URI::urlDecode([query UTF8String]);
    rho::String res = rho::apiGenerator::js_entry_point(data.c_str());
    return [NSString stringWithUTF8String:res.c_str()];
}

@end
