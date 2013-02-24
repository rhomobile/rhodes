#import "IMegamodule.h"
//#import "api_generator/common/ruby_helpers.h"

#include "api_generator/js_helpers.h"

#import "api_generator/iphone/CMethodResult.h"
#import "api_generator/iphone/CJSConverter.h"

extern VALUE getRuby_Megamodule_Module();



id<IMegamodule> Megamodule_makeInstanceByJSObject(rho::String sid) {
    const char* szID = sid.c_str();
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    return [factory getMegamoduleByID:[NSString stringWithUTF8String:szID]];
}









@interface Megamodule_getStringProperty_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_getStringProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_getStringProperty_caller_params

@synthesize params,item,methodResult;

+(Megamodule_getStringProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_getStringProperty_caller_params* par = [[Megamodule_getStringProperty_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_getStringProperty_caller : NSObject {

}
+(Megamodule_getStringProperty_caller*) getSharedInstance;
+(void) getStringProperty:(Megamodule_getStringProperty_caller_params*)caller_params;
+(void) getStringProperty_in_thread:(Megamodule_getStringProperty_caller_params*)caller_params;
+(void) getStringProperty_in_UI_thread:(Megamodule_getStringProperty_caller_params*)caller_params;

@end

static Megamodule_getStringProperty_caller* our_Megamodule_getStringProperty_caller = nil;

@implementation Megamodule_getStringProperty_caller

+(Megamodule_getStringProperty_caller*) getSharedInstance {
    if (our_Megamodule_getStringProperty_caller == nil) {
        our_Megamodule_getStringProperty_caller = [[Megamodule_getStringProperty_caller alloc] init];
    }
    return our_Megamodule_getStringProperty_caller;
}

-(void) command_getStringProperty:(Megamodule_getStringProperty_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getStringProperty:methodResult ];
}


+(void) getStringProperty:(Megamodule_getStringProperty_caller_params*)caller_params {
    [[Megamodule_getStringProperty_caller getSharedInstance] command_getStringProperty:caller_params];
}

+(void) getStringProperty_in_thread:(Megamodule_getStringProperty_caller_params*)caller_params {
    [[Megamodule_getStringProperty_caller getSharedInstance] performSelectorInBackground:@selector(command_getStringProperty:) withObject:caller_params];
}

+(void) getStringProperty_in_UI_thread:(Megamodule_getStringProperty_caller_params*)caller_params {
    [[Megamodule_getStringProperty_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getStringProperty:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_getStringProperty_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_getStringProperty_caller getStringProperty_in_thread:[Megamodule_getStringProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_getStringProperty_caller getStringProperty:[Megamodule_getStringProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_getStringProperty(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_getStringProperty_Obj(argv, item);

}

rho::String js_s_Megamodule_def_getStringProperty(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_getStringProperty_Obj(argv, item);
}







@interface Megamodule_getIntegerProperty_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_getIntegerProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_getIntegerProperty_caller_params

@synthesize params,item,methodResult;

+(Megamodule_getIntegerProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_getIntegerProperty_caller_params* par = [[Megamodule_getIntegerProperty_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_getIntegerProperty_caller : NSObject {

}
+(Megamodule_getIntegerProperty_caller*) getSharedInstance;
+(void) getIntegerProperty:(Megamodule_getIntegerProperty_caller_params*)caller_params;
+(void) getIntegerProperty_in_thread:(Megamodule_getIntegerProperty_caller_params*)caller_params;
+(void) getIntegerProperty_in_UI_thread:(Megamodule_getIntegerProperty_caller_params*)caller_params;

@end

static Megamodule_getIntegerProperty_caller* our_Megamodule_getIntegerProperty_caller = nil;

@implementation Megamodule_getIntegerProperty_caller

+(Megamodule_getIntegerProperty_caller*) getSharedInstance {
    if (our_Megamodule_getIntegerProperty_caller == nil) {
        our_Megamodule_getIntegerProperty_caller = [[Megamodule_getIntegerProperty_caller alloc] init];
    }
    return our_Megamodule_getIntegerProperty_caller;
}

-(void) command_getIntegerProperty:(Megamodule_getIntegerProperty_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getIntegerProperty:methodResult ];
}


+(void) getIntegerProperty:(Megamodule_getIntegerProperty_caller_params*)caller_params {
    [[Megamodule_getIntegerProperty_caller getSharedInstance] command_getIntegerProperty:caller_params];
}

+(void) getIntegerProperty_in_thread:(Megamodule_getIntegerProperty_caller_params*)caller_params {
    [[Megamodule_getIntegerProperty_caller getSharedInstance] performSelectorInBackground:@selector(command_getIntegerProperty:) withObject:caller_params];
}

+(void) getIntegerProperty_in_UI_thread:(Megamodule_getIntegerProperty_caller_params*)caller_params {
    [[Megamodule_getIntegerProperty_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getIntegerProperty:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_getIntegerProperty_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_getIntegerProperty_caller getIntegerProperty_in_thread:[Megamodule_getIntegerProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_getIntegerProperty_caller getIntegerProperty:[Megamodule_getIntegerProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_getIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_getIntegerProperty_Obj(argv, item);

}

rho::String js_s_Megamodule_def_getIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_getIntegerProperty_Obj(argv, item);
}







@interface Megamodule_setIntegerProperty_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_setIntegerProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_setIntegerProperty_caller_params

@synthesize params,item,methodResult;

+(Megamodule_setIntegerProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_setIntegerProperty_caller_params* par = [[Megamodule_setIntegerProperty_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_setIntegerProperty_caller : NSObject {

}
+(Megamodule_setIntegerProperty_caller*) getSharedInstance;
+(void) setIntegerProperty:(Megamodule_setIntegerProperty_caller_params*)caller_params;
+(void) setIntegerProperty_in_thread:(Megamodule_setIntegerProperty_caller_params*)caller_params;
+(void) setIntegerProperty_in_UI_thread:(Megamodule_setIntegerProperty_caller_params*)caller_params;

@end

static Megamodule_setIntegerProperty_caller* our_Megamodule_setIntegerProperty_caller = nil;

@implementation Megamodule_setIntegerProperty_caller

+(Megamodule_setIntegerProperty_caller*) getSharedInstance {
    if (our_Megamodule_setIntegerProperty_caller == nil) {
        our_Megamodule_setIntegerProperty_caller = [[Megamodule_setIntegerProperty_caller alloc] init];
    }
    return our_Megamodule_setIntegerProperty_caller;
}

-(void) command_setIntegerProperty:(Megamodule_setIntegerProperty_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem setIntegerProperty:(int)[((NSNumber*)[params objectAtIndex:0]) intValue] ];
}


+(void) setIntegerProperty:(Megamodule_setIntegerProperty_caller_params*)caller_params {
    [[Megamodule_setIntegerProperty_caller getSharedInstance] command_setIntegerProperty:caller_params];
}

+(void) setIntegerProperty_in_thread:(Megamodule_setIntegerProperty_caller_params*)caller_params {
    [[Megamodule_setIntegerProperty_caller getSharedInstance] performSelectorInBackground:@selector(command_setIntegerProperty:) withObject:caller_params];
}

+(void) setIntegerProperty_in_UI_thread:(Megamodule_setIntegerProperty_caller_params*)caller_params {
    [[Megamodule_setIntegerProperty_caller getSharedInstance] performSelectorOnMainThread:@selector(command_setIntegerProperty:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_setIntegerProperty_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
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
        
        [Megamodule_setIntegerProperty_caller setIntegerProperty_in_thread:[Megamodule_setIntegerProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_setIntegerProperty_caller setIntegerProperty:[Megamodule_setIntegerProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_setIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_setIntegerProperty_Obj(argv, item);

}

rho::String js_s_Megamodule_def_setIntegerProperty(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_setIntegerProperty_Obj(argv, item);
}







@interface Megamodule_typesTest_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_typesTest_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_typesTest_caller_params

@synthesize params,item,methodResult;

+(Megamodule_typesTest_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_typesTest_caller_params* par = [[Megamodule_typesTest_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_typesTest_caller : NSObject {

}
+(Megamodule_typesTest_caller*) getSharedInstance;
+(void) typesTest:(Megamodule_typesTest_caller_params*)caller_params;
+(void) typesTest_in_thread:(Megamodule_typesTest_caller_params*)caller_params;
+(void) typesTest_in_UI_thread:(Megamodule_typesTest_caller_params*)caller_params;

@end

static Megamodule_typesTest_caller* our_Megamodule_typesTest_caller = nil;

@implementation Megamodule_typesTest_caller

+(Megamodule_typesTest_caller*) getSharedInstance {
    if (our_Megamodule_typesTest_caller == nil) {
        our_Megamodule_typesTest_caller = [[Megamodule_typesTest_caller alloc] init];
    }
    return our_Megamodule_typesTest_caller;
}

-(void) command_typesTest:(Megamodule_typesTest_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem typesTest:(NSString*)[params objectAtIndex:0] paramBool:(BOOL)[((NSNumber*)[params objectAtIndex:1]) boolValue] paramInt:(int)[((NSNumber*)[params objectAtIndex:2]) intValue] paramFloat:(float)[((NSNumber*)[params objectAtIndex:3]) floatValue] paramArray:(NSArray*)[params objectAtIndex:4] paramHash:(NSDictionary*)[params objectAtIndex:5] methodResult:methodResult ];
}


+(void) typesTest:(Megamodule_typesTest_caller_params*)caller_params {
    [[Megamodule_typesTest_caller getSharedInstance] command_typesTest:caller_params];
}

+(void) typesTest_in_thread:(Megamodule_typesTest_caller_params*)caller_params {
    [[Megamodule_typesTest_caller getSharedInstance] performSelectorInBackground:@selector(command_typesTest:) withObject:caller_params];
}

+(void) typesTest_in_UI_thread:(Megamodule_typesTest_caller_params*)caller_params {
    [[Megamodule_typesTest_caller getSharedInstance] performSelectorOnMainThread:@selector(command_typesTest:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_typesTest_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[6+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
    BOOL is_factory_param[] = { NO, NO, NO, NO, NO, NO, NO };

    int i;

    // init
    for (i = 0; i < (6); i++) {
        params[i] = [CJSConverter getObjectiveCNULL];
    }

    // enumerate params
    for (int i = 0; i < (6); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(6)];
    for (i = 0 ; i < (6); i++) {
        [params_array addObject:params[i]];
    }

    
    // check callback
    if (argc >= (6+1)) {
        rho::json::CJSONEntry callback = argv.getItem(6);
        if (callback.isString()) {
            rho::json::CJSONEntry entry = argv.getItem(i);
            callbackURL = [((NSString*)[CJSConverter convertFromJS:&callback]) retain];
        }
    }
    // check callback param
    if (argc >= (6+2)) {
        rho::json::CJSONEntry callback_param = argv.getItem(6+1);
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
        
        [Megamodule_typesTest_caller typesTest_in_thread:[Megamodule_typesTest_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_typesTest_caller typesTest:[Megamodule_typesTest_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_typesTest(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_typesTest_Obj(argv, item);

}

rho::String js_s_Megamodule_def_typesTest(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_typesTest_Obj(argv, item);
}







@interface Megamodule_produceArray_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_produceArray_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_produceArray_caller_params

@synthesize params,item,methodResult;

+(Megamodule_produceArray_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_produceArray_caller_params* par = [[Megamodule_produceArray_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_produceArray_caller : NSObject {

}
+(Megamodule_produceArray_caller*) getSharedInstance;
+(void) produceArray:(Megamodule_produceArray_caller_params*)caller_params;
+(void) produceArray_in_thread:(Megamodule_produceArray_caller_params*)caller_params;
+(void) produceArray_in_UI_thread:(Megamodule_produceArray_caller_params*)caller_params;

@end

static Megamodule_produceArray_caller* our_Megamodule_produceArray_caller = nil;

@implementation Megamodule_produceArray_caller

+(Megamodule_produceArray_caller*) getSharedInstance {
    if (our_Megamodule_produceArray_caller == nil) {
        our_Megamodule_produceArray_caller = [[Megamodule_produceArray_caller alloc] init];
    }
    return our_Megamodule_produceArray_caller;
}

-(void) command_produceArray:(Megamodule_produceArray_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem produceArray:methodResult ];
}


+(void) produceArray:(Megamodule_produceArray_caller_params*)caller_params {
    [[Megamodule_produceArray_caller getSharedInstance] command_produceArray:caller_params];
}

+(void) produceArray_in_thread:(Megamodule_produceArray_caller_params*)caller_params {
    [[Megamodule_produceArray_caller getSharedInstance] performSelectorInBackground:@selector(command_produceArray:) withObject:caller_params];
}

+(void) produceArray_in_UI_thread:(Megamodule_produceArray_caller_params*)caller_params {
    [[Megamodule_produceArray_caller getSharedInstance] performSelectorOnMainThread:@selector(command_produceArray:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_produceArray_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_produceArray_caller produceArray_in_thread:[Megamodule_produceArray_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_produceArray_caller produceArray:[Megamodule_produceArray_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_produceArray(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_produceArray_Obj(argv, item);

}

rho::String js_s_Megamodule_def_produceArray(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_produceArray_Obj(argv, item);
}







@interface Megamodule_produceHash_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_produceHash_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_produceHash_caller_params

@synthesize params,item,methodResult;

+(Megamodule_produceHash_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_produceHash_caller_params* par = [[Megamodule_produceHash_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_produceHash_caller : NSObject {

}
+(Megamodule_produceHash_caller*) getSharedInstance;
+(void) produceHash:(Megamodule_produceHash_caller_params*)caller_params;
+(void) produceHash_in_thread:(Megamodule_produceHash_caller_params*)caller_params;
+(void) produceHash_in_UI_thread:(Megamodule_produceHash_caller_params*)caller_params;

@end

static Megamodule_produceHash_caller* our_Megamodule_produceHash_caller = nil;

@implementation Megamodule_produceHash_caller

+(Megamodule_produceHash_caller*) getSharedInstance {
    if (our_Megamodule_produceHash_caller == nil) {
        our_Megamodule_produceHash_caller = [[Megamodule_produceHash_caller alloc] init];
    }
    return our_Megamodule_produceHash_caller;
}

-(void) command_produceHash:(Megamodule_produceHash_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem produceHash:methodResult ];
}


+(void) produceHash:(Megamodule_produceHash_caller_params*)caller_params {
    [[Megamodule_produceHash_caller getSharedInstance] command_produceHash:caller_params];
}

+(void) produceHash_in_thread:(Megamodule_produceHash_caller_params*)caller_params {
    [[Megamodule_produceHash_caller getSharedInstance] performSelectorInBackground:@selector(command_produceHash:) withObject:caller_params];
}

+(void) produceHash_in_UI_thread:(Megamodule_produceHash_caller_params*)caller_params {
    [[Megamodule_produceHash_caller getSharedInstance] performSelectorOnMainThread:@selector(command_produceHash:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_produceHash_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_produceHash_caller produceHash_in_thread:[Megamodule_produceHash_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_produceHash_caller produceHash:[Megamodule_produceHash_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_produceHash(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_produceHash_Obj(argv, item);

}

rho::String js_s_Megamodule_def_produceHash(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_produceHash_Obj(argv, item);
}







@interface Megamodule_produceComplicatedResult_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_produceComplicatedResult_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_produceComplicatedResult_caller_params

@synthesize params,item,methodResult;

+(Megamodule_produceComplicatedResult_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_produceComplicatedResult_caller_params* par = [[Megamodule_produceComplicatedResult_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_produceComplicatedResult_caller : NSObject {

}
+(Megamodule_produceComplicatedResult_caller*) getSharedInstance;
+(void) produceComplicatedResult:(Megamodule_produceComplicatedResult_caller_params*)caller_params;
+(void) produceComplicatedResult_in_thread:(Megamodule_produceComplicatedResult_caller_params*)caller_params;
+(void) produceComplicatedResult_in_UI_thread:(Megamodule_produceComplicatedResult_caller_params*)caller_params;

@end

static Megamodule_produceComplicatedResult_caller* our_Megamodule_produceComplicatedResult_caller = nil;

@implementation Megamodule_produceComplicatedResult_caller

+(Megamodule_produceComplicatedResult_caller*) getSharedInstance {
    if (our_Megamodule_produceComplicatedResult_caller == nil) {
        our_Megamodule_produceComplicatedResult_caller = [[Megamodule_produceComplicatedResult_caller alloc] init];
    }
    return our_Megamodule_produceComplicatedResult_caller;
}

-(void) command_produceComplicatedResult:(Megamodule_produceComplicatedResult_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem produceComplicatedResult:methodResult ];
}


+(void) produceComplicatedResult:(Megamodule_produceComplicatedResult_caller_params*)caller_params {
    [[Megamodule_produceComplicatedResult_caller getSharedInstance] command_produceComplicatedResult:caller_params];
}

+(void) produceComplicatedResult_in_thread:(Megamodule_produceComplicatedResult_caller_params*)caller_params {
    [[Megamodule_produceComplicatedResult_caller getSharedInstance] performSelectorInBackground:@selector(command_produceComplicatedResult:) withObject:caller_params];
}

+(void) produceComplicatedResult_in_UI_thread:(Megamodule_produceComplicatedResult_caller_params*)caller_params {
    [[Megamodule_produceComplicatedResult_caller getSharedInstance] performSelectorOnMainThread:@selector(command_produceComplicatedResult:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_produceComplicatedResult_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_produceComplicatedResult_caller produceComplicatedResult_in_thread:[Megamodule_produceComplicatedResult_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_produceComplicatedResult_caller produceComplicatedResult:[Megamodule_produceComplicatedResult_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_produceComplicatedResult(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_produceComplicatedResult_Obj(argv, item);

}

rho::String js_s_Megamodule_def_produceComplicatedResult(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_produceComplicatedResult_Obj(argv, item);
}







@interface Megamodule_getObjectsCount_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamoduleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_getObjectsCount_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_getObjectsCount_caller_params

@synthesize params,item,methodResult;

+(Megamodule_getObjectsCount_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_getObjectsCount_caller_params* par = [[Megamodule_getObjectsCount_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_getObjectsCount_caller : NSObject {

}
+(Megamodule_getObjectsCount_caller*) getSharedInstance;
+(void) getObjectsCount:(Megamodule_getObjectsCount_caller_params*)caller_params;
+(void) getObjectsCount_in_thread:(Megamodule_getObjectsCount_caller_params*)caller_params;
+(void) getObjectsCount_in_UI_thread:(Megamodule_getObjectsCount_caller_params*)caller_params;

@end

static Megamodule_getObjectsCount_caller* our_Megamodule_getObjectsCount_caller = nil;

@implementation Megamodule_getObjectsCount_caller

+(Megamodule_getObjectsCount_caller*) getSharedInstance {
    if (our_Megamodule_getObjectsCount_caller == nil) {
        our_Megamodule_getObjectsCount_caller = [[Megamodule_getObjectsCount_caller alloc] init];
    }
    return our_Megamodule_getObjectsCount_caller;
}

-(void) command_getObjectsCount:(Megamodule_getObjectsCount_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamoduleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getObjectsCount:methodResult ];
}


+(void) getObjectsCount:(Megamodule_getObjectsCount_caller_params*)caller_params {
    [[Megamodule_getObjectsCount_caller getSharedInstance] command_getObjectsCount:caller_params];
}

+(void) getObjectsCount_in_thread:(Megamodule_getObjectsCount_caller_params*)caller_params {
    [[Megamodule_getObjectsCount_caller getSharedInstance] performSelectorInBackground:@selector(command_getObjectsCount:) withObject:caller_params];
}

+(void) getObjectsCount_in_UI_thread:(Megamodule_getObjectsCount_caller_params*)caller_params {
    [[Megamodule_getObjectsCount_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getObjectsCount:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_getObjectsCount_Obj(rho::json::CJSONArray& argv, id<IMegamoduleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_getObjectsCount_caller getObjectsCount_in_thread:[Megamodule_getObjectsCount_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_getObjectsCount_caller getObjectsCount:[Megamodule_getObjectsCount_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_s_Megamodule_getObjectsCount(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];
    return js_Megamodule_getObjectsCount_Obj(argv, singleton);

}







@interface Megamodule_getObjectByIndex_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamoduleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_getObjectByIndex_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_getObjectByIndex_caller_params

@synthesize params,item,methodResult;

+(Megamodule_getObjectByIndex_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_getObjectByIndex_caller_params* par = [[Megamodule_getObjectByIndex_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_getObjectByIndex_caller : NSObject {

}
+(Megamodule_getObjectByIndex_caller*) getSharedInstance;
+(void) getObjectByIndex:(Megamodule_getObjectByIndex_caller_params*)caller_params;
+(void) getObjectByIndex_in_thread:(Megamodule_getObjectByIndex_caller_params*)caller_params;
+(void) getObjectByIndex_in_UI_thread:(Megamodule_getObjectByIndex_caller_params*)caller_params;

@end

static Megamodule_getObjectByIndex_caller* our_Megamodule_getObjectByIndex_caller = nil;

@implementation Megamodule_getObjectByIndex_caller

+(Megamodule_getObjectByIndex_caller*) getSharedInstance {
    if (our_Megamodule_getObjectByIndex_caller == nil) {
        our_Megamodule_getObjectByIndex_caller = [[Megamodule_getObjectByIndex_caller alloc] init];
    }
    return our_Megamodule_getObjectByIndex_caller;
}

-(void) command_getObjectByIndex:(Megamodule_getObjectByIndex_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamoduleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getObjectByIndex:(int)[((NSNumber*)[params objectAtIndex:0]) intValue] methodResult:methodResult ];
}


+(void) getObjectByIndex:(Megamodule_getObjectByIndex_caller_params*)caller_params {
    [[Megamodule_getObjectByIndex_caller getSharedInstance] command_getObjectByIndex:caller_params];
}

+(void) getObjectByIndex_in_thread:(Megamodule_getObjectByIndex_caller_params*)caller_params {
    [[Megamodule_getObjectByIndex_caller getSharedInstance] performSelectorInBackground:@selector(command_getObjectByIndex:) withObject:caller_params];
}

+(void) getObjectByIndex_in_UI_thread:(Megamodule_getObjectByIndex_caller_params*)caller_params {
    [[Megamodule_getObjectByIndex_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getObjectByIndex:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_getObjectByIndex_Obj(rho::json::CJSONArray& argv, id<IMegamoduleSingleton>objItem) {

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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
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
        
        [Megamodule_getObjectByIndex_caller getObjectByIndex_in_thread:[Megamodule_getObjectByIndex_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_getObjectByIndex_caller getObjectByIndex:[Megamodule_getObjectByIndex_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_s_Megamodule_getObjectByIndex(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];
    return js_Megamodule_getObjectByIndex_Obj(argv, singleton);

}







@interface Megamodule_showAlertFromUIThread_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_showAlertFromUIThread_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_showAlertFromUIThread_caller_params

@synthesize params,item,methodResult;

+(Megamodule_showAlertFromUIThread_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_showAlertFromUIThread_caller_params* par = [[Megamodule_showAlertFromUIThread_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_showAlertFromUIThread_caller : NSObject {

}
+(Megamodule_showAlertFromUIThread_caller*) getSharedInstance;
+(void) showAlertFromUIThread:(Megamodule_showAlertFromUIThread_caller_params*)caller_params;
+(void) showAlertFromUIThread_in_thread:(Megamodule_showAlertFromUIThread_caller_params*)caller_params;
+(void) showAlertFromUIThread_in_UI_thread:(Megamodule_showAlertFromUIThread_caller_params*)caller_params;

@end

static Megamodule_showAlertFromUIThread_caller* our_Megamodule_showAlertFromUIThread_caller = nil;

@implementation Megamodule_showAlertFromUIThread_caller

+(Megamodule_showAlertFromUIThread_caller*) getSharedInstance {
    if (our_Megamodule_showAlertFromUIThread_caller == nil) {
        our_Megamodule_showAlertFromUIThread_caller = [[Megamodule_showAlertFromUIThread_caller alloc] init];
    }
    return our_Megamodule_showAlertFromUIThread_caller;
}

-(void) command_showAlertFromUIThread:(Megamodule_showAlertFromUIThread_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem showAlertFromUIThread:(NSString*)[params objectAtIndex:0] ];
}


+(void) showAlertFromUIThread:(Megamodule_showAlertFromUIThread_caller_params*)caller_params {
    [[Megamodule_showAlertFromUIThread_caller getSharedInstance] command_showAlertFromUIThread:caller_params];
}

+(void) showAlertFromUIThread_in_thread:(Megamodule_showAlertFromUIThread_caller_params*)caller_params {
    [[Megamodule_showAlertFromUIThread_caller getSharedInstance] performSelectorInBackground:@selector(command_showAlertFromUIThread:) withObject:caller_params];
}

+(void) showAlertFromUIThread_in_UI_thread:(Megamodule_showAlertFromUIThread_caller_params*)caller_params {
    [[Megamodule_showAlertFromUIThread_caller getSharedInstance] performSelectorOnMainThread:@selector(command_showAlertFromUIThread:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_showAlertFromUIThread_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
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
        
        [Megamodule_showAlertFromUIThread_caller showAlertFromUIThread_in_UI_thread:[Megamodule_showAlertFromUIThread_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_showAlertFromUIThread_caller showAlertFromUIThread_in_UI_thread:[Megamodule_showAlertFromUIThread_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        method_return_result = NO;
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_showAlertFromUIThread(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_showAlertFromUIThread_Obj(argv, item);

}

rho::String js_s_Megamodule_def_showAlertFromUIThread(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_showAlertFromUIThread_Obj(argv, item);
}







@interface Megamodule_setPeriodicallyCallback_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_setPeriodicallyCallback_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_setPeriodicallyCallback_caller_params

@synthesize params,item,methodResult;

+(Megamodule_setPeriodicallyCallback_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_setPeriodicallyCallback_caller_params* par = [[Megamodule_setPeriodicallyCallback_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_setPeriodicallyCallback_caller : NSObject {

}
+(Megamodule_setPeriodicallyCallback_caller*) getSharedInstance;
+(void) setPeriodicallyCallback:(Megamodule_setPeriodicallyCallback_caller_params*)caller_params;
+(void) setPeriodicallyCallback_in_thread:(Megamodule_setPeriodicallyCallback_caller_params*)caller_params;
+(void) setPeriodicallyCallback_in_UI_thread:(Megamodule_setPeriodicallyCallback_caller_params*)caller_params;

@end

static Megamodule_setPeriodicallyCallback_caller* our_Megamodule_setPeriodicallyCallback_caller = nil;

@implementation Megamodule_setPeriodicallyCallback_caller

+(Megamodule_setPeriodicallyCallback_caller*) getSharedInstance {
    if (our_Megamodule_setPeriodicallyCallback_caller == nil) {
        our_Megamodule_setPeriodicallyCallback_caller = [[Megamodule_setPeriodicallyCallback_caller alloc] init];
    }
    return our_Megamodule_setPeriodicallyCallback_caller;
}

-(void) command_setPeriodicallyCallback:(Megamodule_setPeriodicallyCallback_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem setPeriodicallyCallback:(int)[((NSNumber*)[params objectAtIndex:0]) intValue] methodResult:methodResult ];
}


+(void) setPeriodicallyCallback:(Megamodule_setPeriodicallyCallback_caller_params*)caller_params {
    [[Megamodule_setPeriodicallyCallback_caller getSharedInstance] command_setPeriodicallyCallback:caller_params];
}

+(void) setPeriodicallyCallback_in_thread:(Megamodule_setPeriodicallyCallback_caller_params*)caller_params {
    [[Megamodule_setPeriodicallyCallback_caller getSharedInstance] performSelectorInBackground:@selector(command_setPeriodicallyCallback:) withObject:caller_params];
}

+(void) setPeriodicallyCallback_in_UI_thread:(Megamodule_setPeriodicallyCallback_caller_params*)caller_params {
    [[Megamodule_setPeriodicallyCallback_caller getSharedInstance] performSelectorOnMainThread:@selector(command_setPeriodicallyCallback:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_setPeriodicallyCallback_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
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
        
        [Megamodule_setPeriodicallyCallback_caller setPeriodicallyCallback_in_thread:[Megamodule_setPeriodicallyCallback_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_setPeriodicallyCallback_caller setPeriodicallyCallback:[Megamodule_setPeriodicallyCallback_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_setPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_setPeriodicallyCallback_Obj(argv, item);

}

rho::String js_s_Megamodule_def_setPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_setPeriodicallyCallback_Obj(argv, item);
}







@interface Megamodule_isPeriodicallyCallbackSetted_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_isPeriodicallyCallbackSetted_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_isPeriodicallyCallbackSetted_caller_params

@synthesize params,item,methodResult;

+(Megamodule_isPeriodicallyCallbackSetted_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_isPeriodicallyCallbackSetted_caller_params* par = [[Megamodule_isPeriodicallyCallbackSetted_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_isPeriodicallyCallbackSetted_caller : NSObject {

}
+(Megamodule_isPeriodicallyCallbackSetted_caller*) getSharedInstance;
+(void) isPeriodicallyCallbackSetted:(Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params;
+(void) isPeriodicallyCallbackSetted_in_thread:(Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params;
+(void) isPeriodicallyCallbackSetted_in_UI_thread:(Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params;

@end

static Megamodule_isPeriodicallyCallbackSetted_caller* our_Megamodule_isPeriodicallyCallbackSetted_caller = nil;

@implementation Megamodule_isPeriodicallyCallbackSetted_caller

+(Megamodule_isPeriodicallyCallbackSetted_caller*) getSharedInstance {
    if (our_Megamodule_isPeriodicallyCallbackSetted_caller == nil) {
        our_Megamodule_isPeriodicallyCallbackSetted_caller = [[Megamodule_isPeriodicallyCallbackSetted_caller alloc] init];
    }
    return our_Megamodule_isPeriodicallyCallbackSetted_caller;
}

-(void) command_isPeriodicallyCallbackSetted:(Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem isPeriodicallyCallbackSetted:methodResult ];
}


+(void) isPeriodicallyCallbackSetted:(Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params {
    [[Megamodule_isPeriodicallyCallbackSetted_caller getSharedInstance] command_isPeriodicallyCallbackSetted:caller_params];
}

+(void) isPeriodicallyCallbackSetted_in_thread:(Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params {
    [[Megamodule_isPeriodicallyCallbackSetted_caller getSharedInstance] performSelectorInBackground:@selector(command_isPeriodicallyCallbackSetted:) withObject:caller_params];
}

+(void) isPeriodicallyCallbackSetted_in_UI_thread:(Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params {
    [[Megamodule_isPeriodicallyCallbackSetted_caller getSharedInstance] performSelectorOnMainThread:@selector(command_isPeriodicallyCallbackSetted:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_isPeriodicallyCallbackSetted_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_isPeriodicallyCallbackSetted_caller isPeriodicallyCallbackSetted_in_thread:[Megamodule_isPeriodicallyCallbackSetted_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_isPeriodicallyCallbackSetted_caller isPeriodicallyCallbackSetted:[Megamodule_isPeriodicallyCallbackSetted_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_isPeriodicallyCallbackSetted(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_isPeriodicallyCallbackSetted_Obj(argv, item);

}

rho::String js_s_Megamodule_def_isPeriodicallyCallbackSetted(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_isPeriodicallyCallbackSetted_Obj(argv, item);
}







@interface Megamodule_stopPeriodicallyCallback_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_stopPeriodicallyCallback_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_stopPeriodicallyCallback_caller_params

@synthesize params,item,methodResult;

+(Megamodule_stopPeriodicallyCallback_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_stopPeriodicallyCallback_caller_params* par = [[Megamodule_stopPeriodicallyCallback_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_stopPeriodicallyCallback_caller : NSObject {

}
+(Megamodule_stopPeriodicallyCallback_caller*) getSharedInstance;
+(void) stopPeriodicallyCallback:(Megamodule_stopPeriodicallyCallback_caller_params*)caller_params;
+(void) stopPeriodicallyCallback_in_thread:(Megamodule_stopPeriodicallyCallback_caller_params*)caller_params;
+(void) stopPeriodicallyCallback_in_UI_thread:(Megamodule_stopPeriodicallyCallback_caller_params*)caller_params;

@end

static Megamodule_stopPeriodicallyCallback_caller* our_Megamodule_stopPeriodicallyCallback_caller = nil;

@implementation Megamodule_stopPeriodicallyCallback_caller

+(Megamodule_stopPeriodicallyCallback_caller*) getSharedInstance {
    if (our_Megamodule_stopPeriodicallyCallback_caller == nil) {
        our_Megamodule_stopPeriodicallyCallback_caller = [[Megamodule_stopPeriodicallyCallback_caller alloc] init];
    }
    return our_Megamodule_stopPeriodicallyCallback_caller;
}

-(void) command_stopPeriodicallyCallback:(Megamodule_stopPeriodicallyCallback_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem stopPeriodicallyCallback];
}


+(void) stopPeriodicallyCallback:(Megamodule_stopPeriodicallyCallback_caller_params*)caller_params {
    [[Megamodule_stopPeriodicallyCallback_caller getSharedInstance] command_stopPeriodicallyCallback:caller_params];
}

+(void) stopPeriodicallyCallback_in_thread:(Megamodule_stopPeriodicallyCallback_caller_params*)caller_params {
    [[Megamodule_stopPeriodicallyCallback_caller getSharedInstance] performSelectorInBackground:@selector(command_stopPeriodicallyCallback:) withObject:caller_params];
}

+(void) stopPeriodicallyCallback_in_UI_thread:(Megamodule_stopPeriodicallyCallback_caller_params*)caller_params {
    [[Megamodule_stopPeriodicallyCallback_caller getSharedInstance] performSelectorOnMainThread:@selector(command_stopPeriodicallyCallback:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_stopPeriodicallyCallback_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_stopPeriodicallyCallback_caller stopPeriodicallyCallback_in_thread:[Megamodule_stopPeriodicallyCallback_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_stopPeriodicallyCallback_caller stopPeriodicallyCallback:[Megamodule_stopPeriodicallyCallback_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_stopPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_stopPeriodicallyCallback_Obj(argv, item);

}

rho::String js_s_Megamodule_def_stopPeriodicallyCallback(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_stopPeriodicallyCallback_Obj(argv, item);
}







@interface Megamodule_complicatedTypesTest1_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_complicatedTypesTest1_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_complicatedTypesTest1_caller_params

@synthesize params,item,methodResult;

+(Megamodule_complicatedTypesTest1_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_complicatedTypesTest1_caller_params* par = [[Megamodule_complicatedTypesTest1_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_complicatedTypesTest1_caller : NSObject {

}
+(Megamodule_complicatedTypesTest1_caller*) getSharedInstance;
+(void) complicatedTypesTest1:(Megamodule_complicatedTypesTest1_caller_params*)caller_params;
+(void) complicatedTypesTest1_in_thread:(Megamodule_complicatedTypesTest1_caller_params*)caller_params;
+(void) complicatedTypesTest1_in_UI_thread:(Megamodule_complicatedTypesTest1_caller_params*)caller_params;

@end

static Megamodule_complicatedTypesTest1_caller* our_Megamodule_complicatedTypesTest1_caller = nil;

@implementation Megamodule_complicatedTypesTest1_caller

+(Megamodule_complicatedTypesTest1_caller*) getSharedInstance {
    if (our_Megamodule_complicatedTypesTest1_caller == nil) {
        our_Megamodule_complicatedTypesTest1_caller = [[Megamodule_complicatedTypesTest1_caller alloc] init];
    }
    return our_Megamodule_complicatedTypesTest1_caller;
}

-(void) command_complicatedTypesTest1:(Megamodule_complicatedTypesTest1_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem complicatedTypesTest1:(NSArray*)[params objectAtIndex:0] methodResult:methodResult ];
}


+(void) complicatedTypesTest1:(Megamodule_complicatedTypesTest1_caller_params*)caller_params {
    [[Megamodule_complicatedTypesTest1_caller getSharedInstance] command_complicatedTypesTest1:caller_params];
}

+(void) complicatedTypesTest1_in_thread:(Megamodule_complicatedTypesTest1_caller_params*)caller_params {
    [[Megamodule_complicatedTypesTest1_caller getSharedInstance] performSelectorInBackground:@selector(command_complicatedTypesTest1:) withObject:caller_params];
}

+(void) complicatedTypesTest1_in_UI_thread:(Megamodule_complicatedTypesTest1_caller_params*)caller_params {
    [[Megamodule_complicatedTypesTest1_caller getSharedInstance] performSelectorOnMainThread:@selector(command_complicatedTypesTest1:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_complicatedTypesTest1_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
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
        
        [Megamodule_complicatedTypesTest1_caller complicatedTypesTest1_in_thread:[Megamodule_complicatedTypesTest1_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_complicatedTypesTest1_caller complicatedTypesTest1:[Megamodule_complicatedTypesTest1_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_complicatedTypesTest1(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_complicatedTypesTest1_Obj(argv, item);

}

rho::String js_s_Megamodule_def_complicatedTypesTest1(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_complicatedTypesTest1_Obj(argv, item);
}







@interface Megamodule_getProperty_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_getProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_getProperty_caller_params

@synthesize params,item,methodResult;

+(Megamodule_getProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_getProperty_caller_params* par = [[Megamodule_getProperty_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_getProperty_caller : NSObject {

}
+(Megamodule_getProperty_caller*) getSharedInstance;
+(void) getProperty:(Megamodule_getProperty_caller_params*)caller_params;
+(void) getProperty_in_thread:(Megamodule_getProperty_caller_params*)caller_params;
+(void) getProperty_in_UI_thread:(Megamodule_getProperty_caller_params*)caller_params;

@end

static Megamodule_getProperty_caller* our_Megamodule_getProperty_caller = nil;

@implementation Megamodule_getProperty_caller

+(Megamodule_getProperty_caller*) getSharedInstance {
    if (our_Megamodule_getProperty_caller == nil) {
        our_Megamodule_getProperty_caller = [[Megamodule_getProperty_caller alloc] init];
    }
    return our_Megamodule_getProperty_caller;
}

-(void) command_getProperty:(Megamodule_getProperty_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getProperty:(NSString*)[params objectAtIndex:0] methodResult:methodResult ];
}


+(void) getProperty:(Megamodule_getProperty_caller_params*)caller_params {
    [[Megamodule_getProperty_caller getSharedInstance] command_getProperty:caller_params];
}

+(void) getProperty_in_thread:(Megamodule_getProperty_caller_params*)caller_params {
    [[Megamodule_getProperty_caller getSharedInstance] performSelectorInBackground:@selector(command_getProperty:) withObject:caller_params];
}

+(void) getProperty_in_UI_thread:(Megamodule_getProperty_caller_params*)caller_params {
    [[Megamodule_getProperty_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getProperty:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_getProperty_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
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
        
        [Megamodule_getProperty_caller getProperty_in_thread:[Megamodule_getProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_getProperty_caller getProperty:[Megamodule_getProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_getProperty_Obj(argv, item);

}

rho::String js_s_Megamodule_def_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_getProperty_Obj(argv, item);
}







@interface Megamodule_getProperties_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_getProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_getProperties_caller_params

@synthesize params,item,methodResult;

+(Megamodule_getProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_getProperties_caller_params* par = [[Megamodule_getProperties_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_getProperties_caller : NSObject {

}
+(Megamodule_getProperties_caller*) getSharedInstance;
+(void) getProperties:(Megamodule_getProperties_caller_params*)caller_params;
+(void) getProperties_in_thread:(Megamodule_getProperties_caller_params*)caller_params;
+(void) getProperties_in_UI_thread:(Megamodule_getProperties_caller_params*)caller_params;

@end

static Megamodule_getProperties_caller* our_Megamodule_getProperties_caller = nil;

@implementation Megamodule_getProperties_caller

+(Megamodule_getProperties_caller*) getSharedInstance {
    if (our_Megamodule_getProperties_caller == nil) {
        our_Megamodule_getProperties_caller = [[Megamodule_getProperties_caller alloc] init];
    }
    return our_Megamodule_getProperties_caller;
}

-(void) command_getProperties:(Megamodule_getProperties_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getProperties:(NSArray*)[params objectAtIndex:0] methodResult:methodResult ];
}


+(void) getProperties:(Megamodule_getProperties_caller_params*)caller_params {
    [[Megamodule_getProperties_caller getSharedInstance] command_getProperties:caller_params];
}

+(void) getProperties_in_thread:(Megamodule_getProperties_caller_params*)caller_params {
    [[Megamodule_getProperties_caller getSharedInstance] performSelectorInBackground:@selector(command_getProperties:) withObject:caller_params];
}

+(void) getProperties_in_UI_thread:(Megamodule_getProperties_caller_params*)caller_params {
    [[Megamodule_getProperties_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getProperties:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_getProperties_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
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
        
        [Megamodule_getProperties_caller getProperties_in_thread:[Megamodule_getProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_getProperties_caller getProperties:[Megamodule_getProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_getProperties_Obj(argv, item);

}

rho::String js_s_Megamodule_def_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_getProperties_Obj(argv, item);
}







@interface Megamodule_getAllProperties_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_getAllProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_getAllProperties_caller_params

@synthesize params,item,methodResult;

+(Megamodule_getAllProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_getAllProperties_caller_params* par = [[Megamodule_getAllProperties_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_getAllProperties_caller : NSObject {

}
+(Megamodule_getAllProperties_caller*) getSharedInstance;
+(void) getAllProperties:(Megamodule_getAllProperties_caller_params*)caller_params;
+(void) getAllProperties_in_thread:(Megamodule_getAllProperties_caller_params*)caller_params;
+(void) getAllProperties_in_UI_thread:(Megamodule_getAllProperties_caller_params*)caller_params;

@end

static Megamodule_getAllProperties_caller* our_Megamodule_getAllProperties_caller = nil;

@implementation Megamodule_getAllProperties_caller

+(Megamodule_getAllProperties_caller*) getSharedInstance {
    if (our_Megamodule_getAllProperties_caller == nil) {
        our_Megamodule_getAllProperties_caller = [[Megamodule_getAllProperties_caller alloc] init];
    }
    return our_Megamodule_getAllProperties_caller;
}

-(void) command_getAllProperties:(Megamodule_getAllProperties_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getAllProperties:methodResult ];
}


+(void) getAllProperties:(Megamodule_getAllProperties_caller_params*)caller_params {
    [[Megamodule_getAllProperties_caller getSharedInstance] command_getAllProperties:caller_params];
}

+(void) getAllProperties_in_thread:(Megamodule_getAllProperties_caller_params*)caller_params {
    [[Megamodule_getAllProperties_caller getSharedInstance] performSelectorInBackground:@selector(command_getAllProperties:) withObject:caller_params];
}

+(void) getAllProperties_in_UI_thread:(Megamodule_getAllProperties_caller_params*)caller_params {
    [[Megamodule_getAllProperties_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getAllProperties:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_getAllProperties_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_getAllProperties_caller getAllProperties_in_thread:[Megamodule_getAllProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_getAllProperties_caller getAllProperties:[Megamodule_getAllProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_getAllProperties_Obj(argv, item);

}

rho::String js_s_Megamodule_def_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_getAllProperties_Obj(argv, item);
}







@interface Megamodule_setProperty_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_setProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_setProperty_caller_params

@synthesize params,item,methodResult;

+(Megamodule_setProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_setProperty_caller_params* par = [[Megamodule_setProperty_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_setProperty_caller : NSObject {

}
+(Megamodule_setProperty_caller*) getSharedInstance;
+(void) setProperty:(Megamodule_setProperty_caller_params*)caller_params;
+(void) setProperty_in_thread:(Megamodule_setProperty_caller_params*)caller_params;
+(void) setProperty_in_UI_thread:(Megamodule_setProperty_caller_params*)caller_params;

@end

static Megamodule_setProperty_caller* our_Megamodule_setProperty_caller = nil;

@implementation Megamodule_setProperty_caller

+(Megamodule_setProperty_caller*) getSharedInstance {
    if (our_Megamodule_setProperty_caller == nil) {
        our_Megamodule_setProperty_caller = [[Megamodule_setProperty_caller alloc] init];
    }
    return our_Megamodule_setProperty_caller;
}

-(void) command_setProperty:(Megamodule_setProperty_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem setProperty:(NSString*)[params objectAtIndex:0] propertyValue:(NSString*)[params objectAtIndex:1] ];
}


+(void) setProperty:(Megamodule_setProperty_caller_params*)caller_params {
    [[Megamodule_setProperty_caller getSharedInstance] command_setProperty:caller_params];
}

+(void) setProperty_in_thread:(Megamodule_setProperty_caller_params*)caller_params {
    [[Megamodule_setProperty_caller getSharedInstance] performSelectorInBackground:@selector(command_setProperty:) withObject:caller_params];
}

+(void) setProperty_in_UI_thread:(Megamodule_setProperty_caller_params*)caller_params {
    [[Megamodule_setProperty_caller getSharedInstance] performSelectorOnMainThread:@selector(command_setProperty:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_setProperty_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[2+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_setProperty_caller setProperty_in_thread:[Megamodule_setProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_setProperty_caller setProperty:[Megamodule_setProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_setProperty_Obj(argv, item);

}

rho::String js_s_Megamodule_def_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_setProperty_Obj(argv, item);
}







@interface Megamodule_setProperties_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_setProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_setProperties_caller_params

@synthesize params,item,methodResult;

+(Megamodule_setProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_setProperties_caller_params* par = [[Megamodule_setProperties_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_setProperties_caller : NSObject {

}
+(Megamodule_setProperties_caller*) getSharedInstance;
+(void) setProperties:(Megamodule_setProperties_caller_params*)caller_params;
+(void) setProperties_in_thread:(Megamodule_setProperties_caller_params*)caller_params;
+(void) setProperties_in_UI_thread:(Megamodule_setProperties_caller_params*)caller_params;

@end

static Megamodule_setProperties_caller* our_Megamodule_setProperties_caller = nil;

@implementation Megamodule_setProperties_caller

+(Megamodule_setProperties_caller*) getSharedInstance {
    if (our_Megamodule_setProperties_caller == nil) {
        our_Megamodule_setProperties_caller = [[Megamodule_setProperties_caller alloc] init];
    }
    return our_Megamodule_setProperties_caller;
}

-(void) command_setProperties:(Megamodule_setProperties_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem setProperties:(NSDictionary*)[params objectAtIndex:0] ];
}


+(void) setProperties:(Megamodule_setProperties_caller_params*)caller_params {
    [[Megamodule_setProperties_caller getSharedInstance] command_setProperties:caller_params];
}

+(void) setProperties_in_thread:(Megamodule_setProperties_caller_params*)caller_params {
    [[Megamodule_setProperties_caller getSharedInstance] performSelectorInBackground:@selector(command_setProperties:) withObject:caller_params];
}

+(void) setProperties_in_UI_thread:(Megamodule_setProperties_caller_params*)caller_params {
    [[Megamodule_setProperties_caller getSharedInstance] performSelectorOnMainThread:@selector(command_setProperties:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_setProperties_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
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
        
        [Megamodule_setProperties_caller setProperties_in_thread:[Megamodule_setProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_setProperties_caller setProperties:[Megamodule_setProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_setProperties_Obj(argv, item);

}

rho::String js_s_Megamodule_def_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_setProperties_Obj(argv, item);
}







@interface Megamodule_clearAllProperties_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_clearAllProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_clearAllProperties_caller_params

@synthesize params,item,methodResult;

+(Megamodule_clearAllProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_clearAllProperties_caller_params* par = [[Megamodule_clearAllProperties_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_clearAllProperties_caller : NSObject {

}
+(Megamodule_clearAllProperties_caller*) getSharedInstance;
+(void) clearAllProperties:(Megamodule_clearAllProperties_caller_params*)caller_params;
+(void) clearAllProperties_in_thread:(Megamodule_clearAllProperties_caller_params*)caller_params;
+(void) clearAllProperties_in_UI_thread:(Megamodule_clearAllProperties_caller_params*)caller_params;

@end

static Megamodule_clearAllProperties_caller* our_Megamodule_clearAllProperties_caller = nil;

@implementation Megamodule_clearAllProperties_caller

+(Megamodule_clearAllProperties_caller*) getSharedInstance {
    if (our_Megamodule_clearAllProperties_caller == nil) {
        our_Megamodule_clearAllProperties_caller = [[Megamodule_clearAllProperties_caller alloc] init];
    }
    return our_Megamodule_clearAllProperties_caller;
}

-(void) command_clearAllProperties:(Megamodule_clearAllProperties_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem clearAllProperties];
}


+(void) clearAllProperties:(Megamodule_clearAllProperties_caller_params*)caller_params {
    [[Megamodule_clearAllProperties_caller getSharedInstance] command_clearAllProperties:caller_params];
}

+(void) clearAllProperties_in_thread:(Megamodule_clearAllProperties_caller_params*)caller_params {
    [[Megamodule_clearAllProperties_caller getSharedInstance] performSelectorInBackground:@selector(command_clearAllProperties:) withObject:caller_params];
}

+(void) clearAllProperties_in_UI_thread:(Megamodule_clearAllProperties_caller_params*)caller_params {
    [[Megamodule_clearAllProperties_caller getSharedInstance] performSelectorOnMainThread:@selector(command_clearAllProperties:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_clearAllProperties_Obj(rho::json::CJSONArray& argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_clearAllProperties_caller clearAllProperties_in_thread:[Megamodule_clearAllProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_clearAllProperties_caller clearAllProperties:[Megamodule_clearAllProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_Megamodule_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamodule> item = Megamodule_makeInstanceByJSObject(strObjID);
    return js_Megamodule_clearAllProperties_Obj(argv, item);

}

rho::String js_s_Megamodule_def_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return js_Megamodule_clearAllProperties_Obj(argv, item);
}







@interface Megamodule_enumerate_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamoduleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(Megamodule_enumerate_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation Megamodule_enumerate_caller_params

@synthesize params,item,methodResult;

+(Megamodule_enumerate_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    Megamodule_enumerate_caller_params* par = [[Megamodule_enumerate_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface Megamodule_enumerate_caller : NSObject {

}
+(Megamodule_enumerate_caller*) getSharedInstance;
+(void) enumerate:(Megamodule_enumerate_caller_params*)caller_params;
+(void) enumerate_in_thread:(Megamodule_enumerate_caller_params*)caller_params;
+(void) enumerate_in_UI_thread:(Megamodule_enumerate_caller_params*)caller_params;

@end

static Megamodule_enumerate_caller* our_Megamodule_enumerate_caller = nil;

@implementation Megamodule_enumerate_caller

+(Megamodule_enumerate_caller*) getSharedInstance {
    if (our_Megamodule_enumerate_caller == nil) {
        our_Megamodule_enumerate_caller = [[Megamodule_enumerate_caller alloc] init];
    }
    return our_Megamodule_enumerate_caller;
}

-(void) command_enumerate:(Megamodule_enumerate_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamoduleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem enumerate:methodResult ];
}


+(void) enumerate:(Megamodule_enumerate_caller_params*)caller_params {
    [[Megamodule_enumerate_caller getSharedInstance] command_enumerate:caller_params];
}

+(void) enumerate_in_thread:(Megamodule_enumerate_caller_params*)caller_params {
    [[Megamodule_enumerate_caller getSharedInstance] performSelectorInBackground:@selector(command_enumerate:) withObject:caller_params];
}

+(void) enumerate_in_UI_thread:(Megamodule_enumerate_caller_params*)caller_params {
    [[Megamodule_enumerate_caller getSharedInstance] performSelectorOnMainThread:@selector(command_enumerate:) withObject:caller_params waitUntilDone:NO];
}


@end


rho::String js_Megamodule_enumerate_Obj(rho::json::CJSONArray& argv, id<IMegamoduleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    
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
                params[i] = Megamodule_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
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
        
        [Megamodule_enumerate_caller enumerate_in_thread:[Megamodule_enumerate_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [Megamodule_enumerate_caller enumerate:[Megamodule_enumerate_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


rho::String js_s_Megamodule_enumerate(rho::json::CJSONArray& argv, const rho::String& strObjID) {

    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];
    return js_Megamodule_enumerate_Obj(argv, singleton);

}







rho::String js_s_Megamodule_getDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    rho::String res =  [defID UTF8String];

    return res;
}

rho::String js_s_Megamodule_setDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    [singleton setDefaultID:[NSString stringWithUTF8String:(strObjID.c_str())]];

    return "";
}



