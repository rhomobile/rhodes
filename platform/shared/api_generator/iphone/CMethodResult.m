
#import "CMethodResult.h"


#import "CRubyConverter.h"
#import "CJSConverter.h"
#import "RubyCallbackHelper.h"

#include "../js_helpers.h"

extern const char* rho_webview_execute_js(const char* js, int index);

extern int rho_webview_active_tab();

// ToDo: Support CMethodResultError
@implementation CMethodResultError


-(id)init:(int)error_code description:(NSString*)description {
    self = [super init];
    errorCode = error_code;
    errorDescription = description;
    return self;
}

+ (CMethodResultError*) errorWithCode:(int)error_code description:(NSString*)description {
    CMethodResultError* b = [[CMethodResultError alloc] init:error_code description:description];
    return b;
}

-(int)getErrorCode {
    return errorCode;
}

-(NSString*)getErrorDescription {
    return errorDescription;
}

-(void)dealloc {
    [errorDescription release];
    [super dealloc];
}


@end


@implementation CRhoAPIClassInstance

- (id) init:(NSString*)clasRubyFullName iID:(NSString*)iID {
    self = [super init];
    mClassName = [clasRubyFullName retain];
    mInstanceID = [iID retain];
    return self;
}

+(CRhoAPIClassInstance*) rubyClassByName:(NSString*)className instanceID:(NSString*)instanceID {
    CRhoAPIClassInstance* m = [[CRhoAPIClassInstance alloc] init:className iID:instanceID];
    return m;
}

-(NSString*)getClassName {
    return mClassName;
}

-(NSString*)getInstanceID {
    return mInstanceID;
}

-(void)dealloc {
    [mClassName release];
    [mInstanceID release];
    [super dealloc];
}

@end





@implementation CMethodResult

-(id) init {
    self = [super init];
    mValue = [NSNull null];
    mRubyCallbackURL = nil;
    mJSCallbackUID = nil;
    mCallbackParam = nil;
    mRubyFactory = nil;
    mRubyModulePath = nil;
    mRubyCallbackMethod = 0;
    mJSTabIndex = -1;
    mMethodSignature = nil;
    return self;
}

- (void) setResult:(NSObject*)value {
    if (mRubyModulePath != nil) {
        // convert all strings into CRubyModule !
        if ([value isKindOfClass:[NSString class]]) {
            mValue = [CRhoAPIClassInstance rubyClassByName:mRubyModulePath instanceID:((NSString*)value)];
        }
        else if ([value isKindOfClass:[NSArray class]]) {
            NSArray* value_ar = (NSArray*)value;
            NSMutableArray* aValue = [NSMutableArray arrayWithCapacity:[value_ar count]];
            int i;
            for (i = 0; i < [value_ar count]; i++) {
                NSObject* item = [value_ar objectAtIndex:i];
                if ([item isKindOfClass:[NSString class]]) {
                    item = [CRhoAPIClassInstance rubyClassByName:mRubyModulePath instanceID:((NSString*)item)];
                }
                [aValue addObject:item];
            }
            mValue = aValue;
        }
        else {
            mValue = [value retain];
        }
    }
    else {
        mValue = [value retain];
    }
    [self callCallback];
}

-(NSObject*) getResult {
    return mValue;
}

-(void) setMethodSignature:(NSString*)methodSignature {
    mMethodSignature = methodSignature;
}

-(NSString*) getErrorMessage {
    if ([mValue isKindOfClass:[CMethodResultError class]]) {
        CMethodResultError* errObj = (CMethodResultError*)mValue;
        return [errObj getErrorDescription];
    }
    return @"";
}



-(VALUE) toRuby {
    if ([self isError]) {
        CMethodResultError* errObj = (CMethodResultError*)mValue;
        NSString* errMsg = [errObj getErrorDescription];
        if ([errObj getErrorCode] == METHOD_RESULT_ERROR_CODE_INVALID_PARAMS) {
            rho_ruby_raise_argerror([errMsg UTF8String]);
        }
        else {
            rho_ruby_raise_runtime([errMsg UTF8String]);
        }
        return rho_ruby_get_NIL();
    }
    if (mRubyFactory != nil) {
        return [mRubyFactory makeRubyValue:mValue];
    }
    return [CRubyConverter convertToRuby:mValue];
}

-(NSString*) toJSON {
    NSString* res = [CJSConverter convertToJS:mValue level:0];
    //const char* ttt = [res UTF8String];
    return res;
}

-(BOOL) isHash {
    return [mValue isKindOfClass:[NSDictionary class]];
}

-(BOOL) isError {
    return [mValue isKindOfClass:[CMethodResultError class]];
}

-(NSDictionary*) getHashResult {
    return (NSDictionary*)mValue;
}


-(void) setRubyCallback:(NSString*)url {
    if (mRubyCallbackURL) {
        [mRubyCallbackURL release];
    }
    mRubyCallbackURL = [url retain];
}

-(void) setRubyCallbackMethod:(unsigned long)methodValue {
    if (mRubyCallbackMethod != 0) {
        RubyCallbackHelper_releaseRubyValue(mRubyCallbackMethod);
    }
    if (methodValue != 0) {
        mRubyCallbackMethod = methodValue;
        RubyCallbackHelper_holdRubyValue(mRubyCallbackMethod);
    }
}

-(void) setJSCallback:(NSString*)uid webViewUID:(NSString*)webViewUID {
    if (mJSCallbackUID) {
        [mJSCallbackUID release];
    }
    if (mJSWebViewUID) {
        [mJSWebViewUID release];
    }
    mJSCallbackUID = [uid retain];
    mJSWebViewUID = [webViewUID retain];
    mJSTabIndex = rho_webview_active_tab();
}

-(void) setCallbackParam:(NSString*)param {
    if (mCallbackParam) {
        [mCallbackParam release];
    }
    mCallbackParam = [param retain];
}

-(void) callRubyCallback {
    RubyCallbackHelper_callCallback(mRubyCallbackURL, mRubyCallbackMethod, self, mCallbackParam);
}

-(void) setRubyFactory:(id<IMethodResult_RubyObjectFactory>)factory {
    mRubyFactory = factory;
}

-(void) enableObjectCreationModeWithRubyClassPath:(NSString*)classPath {
    mRubyModulePath = [classPath stringByReplacingOccurrencesOfString:@"." withString:@"::"];
}

-(void) enableObjectCreationModeWithJSClassPath:(NSString*)classPath {
    mRubyModulePath = classPath;//[classPath stringByReplacingOccurrencesOfString:@"." withString:@"::"];
}


-(void) callJSCallback:(NSString*)uid {
    if (mCallbackParam == nil) {
        mCallbackParam = @"";
    }
    NSString* jscode = [NSString stringWithFormat:@"Rho.callbackHandler(\"%@\",{%@},\"%@\")", mJSCallbackUID, [self toJSON], mCallbackParam];
    int tabIndex = mJSTabIndex;
    //if (mJSWebViewUID != nil) {
    //    tabIndex = [mJSWebViewUID intValue];
    //}
    //[[[Rhodes sharedInstance] mainView] executeJs:jscode tab:tabIndex];
    
    const char* js_code = [jscode UTF8String];
    if (callCustomJVMCallbackProvider([mJSWebViewUID UTF8String], js_code) != 1) {
        rho_webview_execute_js(js_code, tabIndex);
    }
}

-(void) callCallback {
    if ((mRubyCallbackURL != nil) || (mRubyCallbackMethod != 0)) {
        [self callRubyCallback];
    }
    else if (mJSCallbackUID != nil) {
        [self callJSCallback:mJSCallbackUID];
    }
}

-(BOOL) hasCallback {
    return ((mRubyCallbackURL != nil) || (mRubyCallbackMethod != 0) || (mJSCallbackUID != nil));
}

+ (NSObject*) getObjectiveCNULL {
    return [NSNull null];
}


+ (BOOL)isBoolInsideNumber:(NSNumber*)number {
    return [number objCType][0] == 'c';
}

+ (BOOL)isFloatInsideNumber:(NSNumber*)number {
    return ([number objCType][0] == 'f') || ([number objCType][0] == 'd');
}

+ (BOOL)isIntInsideNumber:(NSNumber*)number {
    return [number objCType][0] == 'i';
}




-(void) dealloc {
    //[mRubyCallbackURL release];
    //[mJSCallbackUID release];
    //[mCallbackParam release];
    //[mValue release];
    //[mRubyModulePath release];
    if (mRubyCallbackMethod != 0) {
        RubyCallbackHelper_releaseRubyValue(mRubyCallbackMethod);
    }
    [super dealloc];
}


@end


@implementation CMethodResult_SimpleHolder

-(id) init {
    self = [super init];
    resultObject = nil;
    return self;
}

+(CMethodResult_SimpleHolder*) makeEmptyHolder {
    CMethodResult_SimpleHolder* obj = [[CMethodResult_SimpleHolder alloc] init];
    return obj;
}

- (void) setResult:(NSObject*)value {
    resultObject = [value retain];
}

-(BOOL) hasCallback
{
    return NO;
}

- (NSObject*) getResult {
    return resultObject;
}

- (void)dealloc {
    [resultObject release];
    [super dealloc];
}

@end

