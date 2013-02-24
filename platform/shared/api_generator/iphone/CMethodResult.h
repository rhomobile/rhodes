
#import <Foundation/Foundation.h>
#import "IMethodResult.h"
#import "ruby/ext/rho/rhoruby.h"


@protocol IMethodResult_RubyObjectFactory <NSObject>

- (VALUE) makeRubyValue:(NSObject*)obj;

@end


@interface CMethodResult : NSObject<IMethodResult> {
    NSObject* mValue;
    NSString* mRubyCallbackURL;
    NSString* mJSCallbackUID;
    NSString* mCallbackParam;
    NSString* mRubyModulePath;
    id<IMethodResult_RubyObjectFactory> mRubyFactory;
}

-(id) init;

- (void) setResult:(NSObject*)value;

-(BOOL) isHash;
-(NSDictionary*) getHashResult;

-(NSObject*) getResult;

-(VALUE) toRuby;
-(NSString*) toJSON;

-(void) setRubyCallback:(NSString*)url;
-(void) setJSCallback:(NSString*)uid;
-(void) setCallbackParam:(NSString*)param;
-(void) setRubyFactory:(id<IMethodResult_RubyObjectFactory>)factory;
-(void) enableObjectCreationModeWithRubyClassPath:(NSString*)classPath;


+ (NSObject*) getObjectiveCNULL;

-(void) callCallback;

-(void) dealloc;

+ (BOOL)isBoolInsideNumber:(NSNumber*)number;
+ (BOOL)isFloatInsideNumber:(NSNumber*)number;
+ (BOOL)isIntInsideNumber:(NSNumber*)number;

@end
