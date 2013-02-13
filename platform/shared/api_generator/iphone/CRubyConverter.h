
#import <Foundation/Foundation.h>
#import "IMethodResult.h"
#import "ruby/ext/rho/rhoruby.h"


@interface CRubyModule : NSObject {
    NSString* mModuleName;
    NSString* mInstanceID;
}

- (id) init:(NSString*)mName iID:(NSString*)iID;

+(CRubyModule*) rubyModuleByName:(NSString*)moduleName instanceID:(NSString*)instanceID;

-(NSString*)getModuleName;

-(NSString*)getInstanceID;

@end


@interface CRubyConverter : NSObject {
}

+ (VALUE) convertToRuby:(NSObject*)objectiveC_value;

+ (NSObject*) convertFromRuby:(VALUE)ruby_value;

@end
