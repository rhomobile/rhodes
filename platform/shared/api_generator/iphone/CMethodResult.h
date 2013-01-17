
#import <Foundation/Foundation.h>
#import "IMethodResult.h"
#import "ruby/ext/rho/rhoruby.h"

@interface CMethodResult : NSObject<IMethodResult> {
    NSObject* mValue;
    NSString* mRubyCallbackURL;
    NSString* mJSCallbackUID;
    NSString* mCallbackParam;
}

-(id) init;

- (void) setResult:(NSObject*)value;

-(NSObject*) getResult;

-(VALUE) toRuby;
-(NSString*) toJSON;

-(void) setRubyCallback:(NSString*)url;
-(void) setJSCallback:(NSString*)uid;
-(void) setCallbackParam:(NSString*)param;

-(void) callCallback;

-(void) dealloc;

@end
