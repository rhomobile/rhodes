#import "IJavascriptVM.h"
//#import "api_generator/common/ruby_helpers.h"

#include "api_generator/js_helpers.h"

#import "api_generator/iphone/CMethodResult.h"
#import "api_generator/iphone/CJSConverter.h"

extern VALUE getRuby_JavascriptVM_Module();



id<IJavascriptVM> JavascriptVM_makeInstanceByJSObject(rho::String sid) {
    const char* szID = sid.c_str();
    id<IJavascriptVMFactory> factory = [JavascriptVMFactorySingleton getJavascriptVMFactoryInstance];
    return [factory getJavascriptVMByID:[NSString stringWithUTF8String:szID]];
}









@interface JavascriptVM_executeScript_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IJavascriptVMSingleton> item;
@property (assign) CMethodResult* methodResult;

+(JavascriptVM_executeScript_caller_params*) makeParams:(NSArray*)_params _item:(id<IJavascriptVMSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation JavascriptVM_executeScript_caller_params

@synthesize params,item,methodResult;

+(JavascriptVM_executeScript_caller_params*) makeParams:(NSArray*)_params _item:(id<IJavascriptVMSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    JavascriptVM_executeScript_caller_params* par = [[JavascriptVM_executeScript_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface JavascriptVM_executeScript_caller : NSObject {

}
+(JavascriptVM_executeScript_caller*) getSharedInstance;
+(void) executeScript:(JavascriptVM_executeScript_caller_params*)caller_params;
+(void) executeScript_in_thread:(JavascriptVM_executeScript_caller_params*)caller_params;
+(void) executeScript_in_UI_thread:(JavascriptVM_executeScript_caller_params*)caller_params;

@end

static JavascriptVM_executeScript_caller* our_JavascriptVM_executeScript_caller = nil;

@implementation JavascriptVM_executeScript_caller

+(JavascriptVM_executeScript_caller*) getSharedInstance {
    if (our_JavascriptVM_executeScript_caller == nil) {
        our_JavascriptVM_executeScript_caller = [[JavascriptVM_executeScript_caller alloc] init];
    }
    return our_JavascriptVM_executeScript_caller;
}

-(void) command_executeScript:(JavascriptVM_executeScript_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IJavascriptVMSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem executeScript:(NSString*)[params objectAtIndex:0] methodResult:methodResult ];
}


+(void) executeScript:(JavascriptVM_executeScript_caller_params*)caller_params {
    [[JavascriptVM_executeScript_caller getSharedInstance] command_executeScript:caller_params];
}

+(void) executeScript_in_thread:(JavascriptVM_executeScript_caller_params*)caller_params {
    [[JavascriptVM_executeScript_caller getSharedInstance] performSelectorInBackground:@selector(command_executeScript:) withObject:caller_params];
}

+(void) executeScript_in_UI_thread:(JavascriptVM_executeScript_caller_params*)caller_params {
    [[JavascriptVM_executeScript_caller getSharedInstance] performSelectorOnMainThread:@selector(command_executeScript:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_JavascriptVM_executeScript_Obj(rho::json::CJSONArray& argv, id<IJavascriptVMSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[1+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
    BOOL is_factory_param[] = { NO, NO };

    int i;

    // init
    for (i = 0; i < (1); i++) {
        params[i] = [CJSConverter getObjectiveCNULL];
    }

    // enumerate params
    for (int i = 0; i < (1); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = JavascriptVM_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(1)];
    for (i = 0 ; i < (1); i++) {
        [params_array addObject:params[i]];
    }

    
    // check callback
    if (argc >= (1+1)) {
        rho::json::CJSONEntry callback = argv.getItem(1);
        if (callback.isString()) {
            rho::json::CJSONEntry entry = argv.getItem(i);
            callbackURL = [((NSString*)[CJSConverter convertFromJS:&callback]) retain];
        }
    }
    // check callback param
    if (argc >= (1+2)) {
        rho::json::CJSONEntry callback_param = argv.getItem(1+1);
        if (callback_param.isString()) {
            callbackParam = [((NSString*)[CJSConverter convertFromJS:&callback_param]) retain];
        }
    }
    

    if (callbackURL != nil) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        [methodResult setJSCallback:callbackURL];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [JavascriptVM_executeScript_caller executeScript_in_thread:[JavascriptVM_executeScript_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [JavascriptVM_executeScript_caller executeScript:[JavascriptVM_executeScript_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_s_JavascriptVM_executeScript(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IJavascriptVMFactory> factory = [JavascriptVMFactorySingleton getJavascriptVMFactoryInstance];
    id<IJavascriptVMSingleton> singleton = [factory getJavascriptVMSingleton];
    return js_JavascriptVM_executeScript_Obj(argv, singleton);

}









