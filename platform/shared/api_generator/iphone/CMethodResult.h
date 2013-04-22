
#import <Foundation/Foundation.h>
#import "IMethodResult.h"
#import "ruby/ext/rho/rhoruby.h"


@interface CMethodResultError : NSObject {
    NSString* errorType;
    NSString* errorDescription;
}

-(id)init;

+ (CMethodResultError*) errorWithType:(NSString*)error_type description:(NSString*)description;

-(NSString*)getErrorType;
-(NSString*)getErrorDescription;

-(void)dealloc;

@end

@interface CRhoAPIClassInstance : NSObject {
    NSString* mClassName;
    NSString* mInstanceID;
}

- (id) init:(NSString*)clasRubyFullName iID:(NSString*)iID;

+(CRhoAPIClassInstance*) rubyClassByName:(NSString*)className instanceID:(NSString*)instanceID;

-(NSString*)getClassName;

-(NSString*)getInstanceID;

@end



@protocol IMethodResult_RubyObjectFactory <NSObject>

- (VALUE) makeRubyValue:(NSObject*)obj;

@end


@interface CMethodResult : NSObject<IMethodResult> {
    NSObject* mValue;
    NSString* mRubyCallbackURL;
    unsigned long mRubyCallbackMethod;
    NSString* mJSCallbackUID;
    NSString* mJSWebViewUID;
    int mJSTabIndex;
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
-(void) setRubyCallbackMethod:(unsigned long)methodValue;
-(void) setJSCallback:(NSString*)uid webViewUID:(NSString*)webViewUID;
-(void) setCallbackParam:(NSString*)param;
-(void) setRubyFactory:(id<IMethodResult_RubyObjectFactory>)factory;
-(void) enableObjectCreationModeWithRubyClassPath:(NSString*)classPath;
-(void) enableObjectCreationModeWithJSClassPath:(NSString*)classPath;

+ (NSObject*) getObjectiveCNULL;

-(void) callCallback;

-(void) dealloc;

+ (BOOL)isBoolInsideNumber:(NSNumber*)number;
+ (BOOL)isFloatInsideNumber:(NSNumber*)number;
+ (BOOL)isIntInsideNumber:(NSNumber*)number;

@end
