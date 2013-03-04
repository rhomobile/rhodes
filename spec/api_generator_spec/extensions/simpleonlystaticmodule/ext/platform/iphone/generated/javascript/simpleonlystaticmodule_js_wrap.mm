#import "ISimpleOnlyStaticModule.h"
//#import "api_generator/common/ruby_helpers.h"

#include "api_generator/js_helpers.h"

#import "api_generator/iphone/CMethodResult.h"
#import "api_generator/iphone/CJSConverter.h"

extern VALUE getRuby_SimpleOnlyStaticModule_Module();



id<ISimpleOnlyStaticModule> SimpleOnlyStaticModule_makeInstanceByJSObject(rho::String sid) {
    const char* szID = sid.c_str();
    id<ISimpleOnlyStaticModuleFactory> factory = [SimpleOnlyStaticModuleFactorySingleton getSimpleOnlyStaticModuleFactoryInstance];
    return [factory getSimpleOnlyStaticModuleByID:[NSString stringWithUTF8String:szID]];
}









@interface SimpleOnlyStaticModule_calcSumm_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<ISimpleOnlyStaticModuleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(SimpleOnlyStaticModule_calcSumm_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation SimpleOnlyStaticModule_calcSumm_caller_params

@synthesize params,item,methodResult;

+(SimpleOnlyStaticModule_calcSumm_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    SimpleOnlyStaticModule_calcSumm_caller_params* par = [[SimpleOnlyStaticModule_calcSumm_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface SimpleOnlyStaticModule_calcSumm_caller : NSObject {

}
+(SimpleOnlyStaticModule_calcSumm_caller*) getSharedInstance;
+(void) calcSumm:(SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params;
+(void) calcSumm_in_thread:(SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params;
+(void) calcSumm_in_UI_thread:(SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params;

@end

static SimpleOnlyStaticModule_calcSumm_caller* our_SimpleOnlyStaticModule_calcSumm_caller = nil;

@implementation SimpleOnlyStaticModule_calcSumm_caller

+(SimpleOnlyStaticModule_calcSumm_caller*) getSharedInstance {
    if (our_SimpleOnlyStaticModule_calcSumm_caller == nil) {
        our_SimpleOnlyStaticModule_calcSumm_caller = [[SimpleOnlyStaticModule_calcSumm_caller alloc] init];
    }
    return our_SimpleOnlyStaticModule_calcSumm_caller;
}

-(void) command_calcSumm:(SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<ISimpleOnlyStaticModuleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem calcSumm:(int)[((NSNumber*)[params objectAtIndex:0]) intValue] y:(int)[((NSNumber*)[params objectAtIndex:1]) intValue] methodResult:methodResult ];
}


+(void) calcSumm:(SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_calcSumm_caller getSharedInstance] command_calcSumm:caller_params];
}

+(void) calcSumm_in_thread:(SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_calcSumm_caller getSharedInstance] performSelectorInBackground:@selector(command_calcSumm:) withObject:caller_params];
}

+(void) calcSumm_in_UI_thread:(SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_calcSumm_caller getSharedInstance] performSelectorOnMainThread:@selector(command_calcSumm:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_SimpleOnlyStaticModule_calcSumm_Obj(rho::json::CJSONArray& argv, id<ISimpleOnlyStaticModuleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[2+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
    static RHO_API_PARAM rho_api_params[] = { {RHO_API_INTEGER, 0, "x", 0, 0 }, {RHO_API_INTEGER, 0, "y", 0, 0 } };

    
    BOOL is_factory_param[] = { NO, NO, NO };

    int i;

    // init
    for (i = 0; i < (2); i++) {
        params[i] = [CJSConverter getObjectiveCNULL];
    }

    // enumerate params
    for (int i = 0; i < (2); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = SimpleOnlyStaticModule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry rho_api_param:&(rho_api_params[i])] retain];
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(2)];
    for (i = 0 ; i < (2); i++) {
        [params_array addObject:params[i]];
    }

    
    // check callback
    if (argc >= (2+1)) {
        rho::json::CJSONEntry callback = argv.getItem(2);
        if (callback.isString()) {
            rho::json::CJSONEntry entry = argv.getItem(i);
            callbackURL = [((NSString*)[CJSConverter convertFromJS:&callback]) retain];
        }
    }
    // check callback param
    if (argc >= (2+2)) {
        rho::json::CJSONEntry callback_param = argv.getItem(2+1);
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
        
        [SimpleOnlyStaticModule_calcSumm_caller calcSumm_in_thread:[SimpleOnlyStaticModule_calcSumm_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [SimpleOnlyStaticModule_calcSumm_caller calcSumm:[SimpleOnlyStaticModule_calcSumm_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_s_SimpleOnlyStaticModule_calcSumm(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<ISimpleOnlyStaticModuleFactory> factory = [SimpleOnlyStaticModuleFactorySingleton getSimpleOnlyStaticModuleFactoryInstance];
    id<ISimpleOnlyStaticModuleSingleton> singleton = [factory getSimpleOnlyStaticModuleSingleton];
    return js_SimpleOnlyStaticModule_calcSumm_Obj(argv, singleton);

}







@interface SimpleOnlyStaticModule_joinStrings_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<ISimpleOnlyStaticModuleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(SimpleOnlyStaticModule_joinStrings_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation SimpleOnlyStaticModule_joinStrings_caller_params

@synthesize params,item,methodResult;

+(SimpleOnlyStaticModule_joinStrings_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    SimpleOnlyStaticModule_joinStrings_caller_params* par = [[SimpleOnlyStaticModule_joinStrings_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface SimpleOnlyStaticModule_joinStrings_caller : NSObject {

}
+(SimpleOnlyStaticModule_joinStrings_caller*) getSharedInstance;
+(void) joinStrings:(SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params;
+(void) joinStrings_in_thread:(SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params;
+(void) joinStrings_in_UI_thread:(SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params;

@end

static SimpleOnlyStaticModule_joinStrings_caller* our_SimpleOnlyStaticModule_joinStrings_caller = nil;

@implementation SimpleOnlyStaticModule_joinStrings_caller

+(SimpleOnlyStaticModule_joinStrings_caller*) getSharedInstance {
    if (our_SimpleOnlyStaticModule_joinStrings_caller == nil) {
        our_SimpleOnlyStaticModule_joinStrings_caller = [[SimpleOnlyStaticModule_joinStrings_caller alloc] init];
    }
    return our_SimpleOnlyStaticModule_joinStrings_caller;
}

-(void) command_joinStrings:(SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<ISimpleOnlyStaticModuleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem joinStrings:(NSArray*)[params objectAtIndex:0] methodResult:methodResult ];
}


+(void) joinStrings:(SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_joinStrings_caller getSharedInstance] command_joinStrings:caller_params];
}

+(void) joinStrings_in_thread:(SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_joinStrings_caller getSharedInstance] performSelectorInBackground:@selector(command_joinStrings:) withObject:caller_params];
}

+(void) joinStrings_in_UI_thread:(SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_joinStrings_caller getSharedInstance] performSelectorOnMainThread:@selector(command_joinStrings:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_SimpleOnlyStaticModule_joinStrings_Obj(rho::json::CJSONArray& argv, id<ISimpleOnlyStaticModuleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[1+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
    static RHO_API_PARAM rho_api_params_param0[] = { {RHO_API_STRING, 0, "array_param", 0, 0 } };
    static RHO_API_PARAM rho_api_params[] = { {RHO_API_ARRAY, 0, "strings", 1, rho_api_params_param0 } };

    
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
                params[i] = SimpleOnlyStaticModule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry rho_api_param:&(rho_api_params[i])] retain];
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
        
        [SimpleOnlyStaticModule_joinStrings_caller joinStrings_in_thread:[SimpleOnlyStaticModule_joinStrings_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [SimpleOnlyStaticModule_joinStrings_caller joinStrings:[SimpleOnlyStaticModule_joinStrings_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_s_SimpleOnlyStaticModule_joinStrings(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<ISimpleOnlyStaticModuleFactory> factory = [SimpleOnlyStaticModuleFactorySingleton getSimpleOnlyStaticModuleFactoryInstance];
    id<ISimpleOnlyStaticModuleSingleton> singleton = [factory getSimpleOnlyStaticModuleSingleton];
    return js_SimpleOnlyStaticModule_joinStrings_Obj(argv, singleton);

}







@interface SimpleOnlyStaticModule_getPlatform_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<ISimpleOnlyStaticModuleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(SimpleOnlyStaticModule_getPlatform_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation SimpleOnlyStaticModule_getPlatform_caller_params

@synthesize params,item,methodResult;

+(SimpleOnlyStaticModule_getPlatform_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    SimpleOnlyStaticModule_getPlatform_caller_params* par = [[SimpleOnlyStaticModule_getPlatform_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface SimpleOnlyStaticModule_getPlatform_caller : NSObject {

}
+(SimpleOnlyStaticModule_getPlatform_caller*) getSharedInstance;
+(void) getPlatform:(SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params;
+(void) getPlatform_in_thread:(SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params;
+(void) getPlatform_in_UI_thread:(SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params;

@end

static SimpleOnlyStaticModule_getPlatform_caller* our_SimpleOnlyStaticModule_getPlatform_caller = nil;

@implementation SimpleOnlyStaticModule_getPlatform_caller

+(SimpleOnlyStaticModule_getPlatform_caller*) getSharedInstance {
    if (our_SimpleOnlyStaticModule_getPlatform_caller == nil) {
        our_SimpleOnlyStaticModule_getPlatform_caller = [[SimpleOnlyStaticModule_getPlatform_caller alloc] init];
    }
    return our_SimpleOnlyStaticModule_getPlatform_caller;
}

-(void) command_getPlatform:(SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<ISimpleOnlyStaticModuleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getPlatform:methodResult ];
}


+(void) getPlatform:(SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_getPlatform_caller getSharedInstance] command_getPlatform:caller_params];
}

+(void) getPlatform_in_thread:(SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_getPlatform_caller getSharedInstance] performSelectorInBackground:@selector(command_getPlatform:) withObject:caller_params];
}

+(void) getPlatform_in_UI_thread:(SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_getPlatform_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getPlatform:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_SimpleOnlyStaticModule_getPlatform_Obj(rho::json::CJSONArray& argv, id<ISimpleOnlyStaticModuleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
    static RHO_API_PARAM rho_api_params[] = {  };

    
    BOOL is_factory_param[] = { NO };

    int i;

    // init
    for (i = 0; i < (0); i++) {
        params[i] = [CJSConverter getObjectiveCNULL];
    }

    // enumerate params
    for (int i = 0; i < (0); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = SimpleOnlyStaticModule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry rho_api_param:&(rho_api_params[i])] retain];
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(0)];
    for (i = 0 ; i < (0); i++) {
        [params_array addObject:params[i]];
    }

    
    // check callback
    if (argc >= (0+1)) {
        rho::json::CJSONEntry callback = argv.getItem(0);
        if (callback.isString()) {
            rho::json::CJSONEntry entry = argv.getItem(i);
            callbackURL = [((NSString*)[CJSConverter convertFromJS:&callback]) retain];
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        rho::json::CJSONEntry callback_param = argv.getItem(0+1);
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
        
        [SimpleOnlyStaticModule_getPlatform_caller getPlatform_in_thread:[SimpleOnlyStaticModule_getPlatform_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [SimpleOnlyStaticModule_getPlatform_caller getPlatform:[SimpleOnlyStaticModule_getPlatform_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_s_SimpleOnlyStaticModule_getPlatform(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<ISimpleOnlyStaticModuleFactory> factory = [SimpleOnlyStaticModuleFactorySingleton getSimpleOnlyStaticModuleFactoryInstance];
    id<ISimpleOnlyStaticModuleSingleton> singleton = [factory getSimpleOnlyStaticModuleSingleton];
    return js_SimpleOnlyStaticModule_getPlatform_Obj(argv, singleton);

}







@interface SimpleOnlyStaticModule_showAlertFromUIThread_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<ISimpleOnlyStaticModuleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation SimpleOnlyStaticModule_showAlertFromUIThread_caller_params

@synthesize params,item,methodResult;

+(SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    SimpleOnlyStaticModule_showAlertFromUIThread_caller_params* par = [[SimpleOnlyStaticModule_showAlertFromUIThread_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface SimpleOnlyStaticModule_showAlertFromUIThread_caller : NSObject {

}
+(SimpleOnlyStaticModule_showAlertFromUIThread_caller*) getSharedInstance;
+(void) showAlertFromUIThread:(SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params;
+(void) showAlertFromUIThread_in_thread:(SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params;
+(void) showAlertFromUIThread_in_UI_thread:(SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params;

@end

static SimpleOnlyStaticModule_showAlertFromUIThread_caller* our_SimpleOnlyStaticModule_showAlertFromUIThread_caller = nil;

@implementation SimpleOnlyStaticModule_showAlertFromUIThread_caller

+(SimpleOnlyStaticModule_showAlertFromUIThread_caller*) getSharedInstance {
    if (our_SimpleOnlyStaticModule_showAlertFromUIThread_caller == nil) {
        our_SimpleOnlyStaticModule_showAlertFromUIThread_caller = [[SimpleOnlyStaticModule_showAlertFromUIThread_caller alloc] init];
    }
    return our_SimpleOnlyStaticModule_showAlertFromUIThread_caller;
}

-(void) command_showAlertFromUIThread:(SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<ISimpleOnlyStaticModuleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem showAlertFromUIThread];
}


+(void) showAlertFromUIThread:(SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_showAlertFromUIThread_caller getSharedInstance] command_showAlertFromUIThread:caller_params];
}

+(void) showAlertFromUIThread_in_thread:(SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_showAlertFromUIThread_caller getSharedInstance] performSelectorInBackground:@selector(command_showAlertFromUIThread:) withObject:caller_params];
}

+(void) showAlertFromUIThread_in_UI_thread:(SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params {
    [[SimpleOnlyStaticModule_showAlertFromUIThread_caller getSharedInstance] performSelectorOnMainThread:@selector(command_showAlertFromUIThread:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_SimpleOnlyStaticModule_showAlertFromUIThread_Obj(rho::json::CJSONArray& argv, id<ISimpleOnlyStaticModuleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
    static RHO_API_PARAM rho_api_params[] = {  };

    
    BOOL is_factory_param[] = { NO };

    int i;

    // init
    for (i = 0; i < (0); i++) {
        params[i] = [CJSConverter getObjectiveCNULL];
    }

    // enumerate params
    for (int i = 0; i < (0); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = SimpleOnlyStaticModule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry rho_api_param:&(rho_api_params[i])] retain];
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(0)];
    for (i = 0 ; i < (0); i++) {
        [params_array addObject:params[i]];
    }

    
    // check callback
    if (argc >= (0+1)) {
        rho::json::CJSONEntry callback = argv.getItem(0);
        if (callback.isString()) {
            rho::json::CJSONEntry entry = argv.getItem(i);
            callbackURL = [((NSString*)[CJSConverter convertFromJS:&callback]) retain];
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        rho::json::CJSONEntry callback_param = argv.getItem(0+1);
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
        
        [SimpleOnlyStaticModule_showAlertFromUIThread_caller showAlertFromUIThread_in_UI_thread:[SimpleOnlyStaticModule_showAlertFromUIThread_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [SimpleOnlyStaticModule_showAlertFromUIThread_caller showAlertFromUIThread_in_UI_thread:[SimpleOnlyStaticModule_showAlertFromUIThread_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        method_return_result = NO;
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_s_SimpleOnlyStaticModule_showAlertFromUIThread(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<ISimpleOnlyStaticModuleFactory> factory = [SimpleOnlyStaticModuleFactorySingleton getSimpleOnlyStaticModuleFactoryInstance];
    id<ISimpleOnlyStaticModuleSingleton> singleton = [factory getSimpleOnlyStaticModuleSingleton];
    return js_SimpleOnlyStaticModule_showAlertFromUIThread_Obj(argv, singleton);

}









