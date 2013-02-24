
#import <Foundation/Foundation.h>
#import "IMethodResult.h"
#import "ruby/ext/rho/rhoruby.h"


@interface CRubyClass : NSObject {
    NSString* mClassName;
    NSString* mInstanceID;
}

- (id) init:(NSString*)clasRubyFullName iID:(NSString*)iID;

+(CRubyClass*) rubyClassByName:(NSString*)className instanceID:(NSString*)instanceID;

-(NSString*)getClassName;

-(NSString*)getInstanceID;

@end


@interface CRubyConverter : NSObject {
}

+ (VALUE) convertToRuby:(NSObject*)objectiveC_value;

+ (NSObject*) convertFromRuby:(VALUE)ruby_value;

@end
