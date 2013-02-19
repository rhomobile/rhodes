#import "ISimpleOnlyStaticModule.h"
//#import "api_generator/common/ruby_helpers.h"

#import "ruby/ext/rho/rhoruby.h"
#import "api_generator/iphone/CMethodResult.h"
#import "api_generator/iphone/CRubyConverter.h"

extern VALUE getRuby_SimpleOnlyStaticModule_Module();



@interface SimpleOnlyStaticModule_RubyValueFactory : NSObject<IMethodResult_RubyObjectFactory> {
}

- (VALUE) makeRubyValue:(NSObject*)obj;
+ (SimpleOnlyStaticModule_RubyValueFactory*) getSharedInstance;

@end

static SimpleOnlyStaticModule_RubyValueFactory* our_SimpleOnlyStaticModule_RubyValueFactory = nil;

@implementation SimpleOnlyStaticModule_RubyValueFactory

- (VALUE) makeRubyValue:(NSObject*)obj {
    VALUE v = rho_ruby_get_NIL();
    if ([obj isKindOfClass:[NSString class]]) {
        // single objects id
        NSString* strRes = (NSString*)obj;
        v = rho_ruby_create_object_with_id( getRuby_SimpleOnlyStaticModule_Module(), [strRes UTF8String] );
    }
    else if ([obj isKindOfClass:[NSArray class]]) {
        // list of IDs
        v = rho_ruby_create_array();
        NSArray* arrRes = (NSArray*)obj;
        int i;
        for (i = 0; i < [arrRes count]; i++) {
            NSString* strItem = (NSString*)[arrRes objectAtIndex:i];
            VALUE vItem = rho_ruby_create_object_with_id( getRuby_SimpleOnlyStaticModule_Module(), [strItem UTF8String] );
            rho_ruby_add_to_array(v, vItem);
        }
    }
    return v;
}

+ (SimpleOnlyStaticModule_RubyValueFactory*) getSharedInstance {
    if (our_SimpleOnlyStaticModule_RubyValueFactory == nil) {
        our_SimpleOnlyStaticModule_RubyValueFactory = [[SimpleOnlyStaticModule_RubyValueFactory alloc] init];
    }
    return our_SimpleOnlyStaticModule_RubyValueFactory;
}

@end


id<ISimpleOnlyStaticModule> SimpleOnlyStaticModule_makeInstanceByRubyObject(VALUE obj) {
    const char* szID = rho_ruby_get_object_id( obj );
    id<ISimpleOnlyStaticModuleFactory> factory = [SimpleOnlyStaticModuleFactorySingleton getSimpleOnlyStaticModuleFactoryInstance];
    return [factory getSimpleOnlyStaticModuleByID:[NSString stringWithUTF8String:szID]];
}









@interface rb_SimpleOnlyStaticModule_calcSumm_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<ISimpleOnlyStaticModuleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(rb_SimpleOnlyStaticModule_calcSumm_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_SimpleOnlyStaticModule_calcSumm_caller_params

@synthesize params,item,methodResult;

+(rb_SimpleOnlyStaticModule_calcSumm_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_SimpleOnlyStaticModule_calcSumm_caller_params* par = [[rb_SimpleOnlyStaticModule_calcSumm_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_SimpleOnlyStaticModule_calcSumm_caller : NSObject {

}
+(rb_SimpleOnlyStaticModule_calcSumm_caller*) getSharedInstance;
+(void) calcSumm:(rb_SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params;
+(void) calcSumm_in_thread:(rb_SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params;
+(void) calcSumm_in_UI_thread:(rb_SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params;

@end

static rb_SimpleOnlyStaticModule_calcSumm_caller* our_SimpleOnlyStaticModule_calcSumm_caller = nil;

@implementation rb_SimpleOnlyStaticModule_calcSumm_caller

+(rb_SimpleOnlyStaticModule_calcSumm_caller*) getSharedInstance {
    if (our_SimpleOnlyStaticModule_calcSumm_caller == nil) {
        our_SimpleOnlyStaticModule_calcSumm_caller = [[rb_SimpleOnlyStaticModule_calcSumm_caller alloc] init];
    }
    return our_SimpleOnlyStaticModule_calcSumm_caller;
}

-(void) command_calcSumm:(rb_SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<ISimpleOnlyStaticModuleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem calcSumm:(int)[((NSNumber*)[params objectAtIndex:0]) intValue] y:(int)[((NSNumber*)[params objectAtIndex:1]) intValue] methodResult:methodResult ];
}

+(void) calcSumm:(rb_SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_calcSumm_caller getSharedInstance] command_calcSumm:caller_params];
}

+(void) calcSumm_in_thread:(rb_SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_calcSumm_caller getSharedInstance] performSelectorInBackground:@selector(command_calcSumm:) withObject:caller_params];
}

+(void) calcSumm_in_UI_thread:(rb_SimpleOnlyStaticModule_calcSumm_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_calcSumm_caller getSharedInstance] performSelectorOnMainThread:@selector(command_calcSumm:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_SimpleOnlyStaticModule_calcSumm_Obj(int argc, VALUE *argv, id<ISimpleOnlyStaticModuleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[2+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    
    BOOL is_factory_param[] = { NO, NO, NO };

    int i;

    // init
    for (i = 0; i < (2); i++) {
        params[i] = [NSNull null];
    }

    // enumerate params
    for (int i = 0; i < (2); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = SimpleOnlyStaticModule_makeInstanceByRubyObject(argv[i]);
            }
            else {
                params[i] = [[CRubyConverter convertFromRuby:argv[i]] retain];
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(2)];
    for (i = 0 ; i < (2); i++) {
        [params_array addObject:params[i]];
    }

    
    // check callback
    if (argc >= (2+1)) {
        VALUE callback = argv[2];
        if (rho_ruby_is_string(callback)) {
            callbackURL = [((NSString*)[CRubyConverter convertFromRuby:callback]) retain];
        }
    }
    // check callback param
    if (argc >= (2+2)) {
        VALUE callback_param = argv[2+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if (callbackURL != nil) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        [methodResult setRubyCallback:callbackURL];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_SimpleOnlyStaticModule_calcSumm_caller calcSumm_in_thread:[rb_SimpleOnlyStaticModule_calcSumm_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_SimpleOnlyStaticModule_calcSumm_caller calcSumm:[rb_SimpleOnlyStaticModule_calcSumm_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_s_SimpleOnlyStaticModule_calcSumm(int argc, VALUE *argv) {

    id<ISimpleOnlyStaticModuleFactory> factory = [SimpleOnlyStaticModuleFactorySingleton getSimpleOnlyStaticModuleFactoryInstance];
    id<ISimpleOnlyStaticModuleSingleton> singleton = [factory getSimpleOnlyStaticModuleSingleton];
    return rb_SimpleOnlyStaticModule_calcSumm_Obj(argc, argv, singleton);

}







@interface rb_SimpleOnlyStaticModule_joinStrings_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<ISimpleOnlyStaticModuleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(rb_SimpleOnlyStaticModule_joinStrings_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_SimpleOnlyStaticModule_joinStrings_caller_params

@synthesize params,item,methodResult;

+(rb_SimpleOnlyStaticModule_joinStrings_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_SimpleOnlyStaticModule_joinStrings_caller_params* par = [[rb_SimpleOnlyStaticModule_joinStrings_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_SimpleOnlyStaticModule_joinStrings_caller : NSObject {

}
+(rb_SimpleOnlyStaticModule_joinStrings_caller*) getSharedInstance;
+(void) joinStrings:(rb_SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params;
+(void) joinStrings_in_thread:(rb_SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params;
+(void) joinStrings_in_UI_thread:(rb_SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params;

@end

static rb_SimpleOnlyStaticModule_joinStrings_caller* our_SimpleOnlyStaticModule_joinStrings_caller = nil;

@implementation rb_SimpleOnlyStaticModule_joinStrings_caller

+(rb_SimpleOnlyStaticModule_joinStrings_caller*) getSharedInstance {
    if (our_SimpleOnlyStaticModule_joinStrings_caller == nil) {
        our_SimpleOnlyStaticModule_joinStrings_caller = [[rb_SimpleOnlyStaticModule_joinStrings_caller alloc] init];
    }
    return our_SimpleOnlyStaticModule_joinStrings_caller;
}

-(void) command_joinStrings:(rb_SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<ISimpleOnlyStaticModuleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem joinStrings:(NSArray*)[params objectAtIndex:0] methodResult:methodResult ];
}

+(void) joinStrings:(rb_SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_joinStrings_caller getSharedInstance] command_joinStrings:caller_params];
}

+(void) joinStrings_in_thread:(rb_SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_joinStrings_caller getSharedInstance] performSelectorInBackground:@selector(command_joinStrings:) withObject:caller_params];
}

+(void) joinStrings_in_UI_thread:(rb_SimpleOnlyStaticModule_joinStrings_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_joinStrings_caller getSharedInstance] performSelectorOnMainThread:@selector(command_joinStrings:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_SimpleOnlyStaticModule_joinStrings_Obj(int argc, VALUE *argv, id<ISimpleOnlyStaticModuleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[1+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    
    BOOL is_factory_param[] = { NO, NO };

    int i;

    // init
    for (i = 0; i < (1); i++) {
        params[i] = [NSNull null];
    }

    // enumerate params
    for (int i = 0; i < (1); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = SimpleOnlyStaticModule_makeInstanceByRubyObject(argv[i]);
            }
            else {
                params[i] = [[CRubyConverter convertFromRuby:argv[i]] retain];
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(1)];
    for (i = 0 ; i < (1); i++) {
        [params_array addObject:params[i]];
    }

    
    // check callback
    if (argc >= (1+1)) {
        VALUE callback = argv[1];
        if (rho_ruby_is_string(callback)) {
            callbackURL = [((NSString*)[CRubyConverter convertFromRuby:callback]) retain];
        }
    }
    // check callback param
    if (argc >= (1+2)) {
        VALUE callback_param = argv[1+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if (callbackURL != nil) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        [methodResult setRubyCallback:callbackURL];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_SimpleOnlyStaticModule_joinStrings_caller joinStrings_in_thread:[rb_SimpleOnlyStaticModule_joinStrings_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_SimpleOnlyStaticModule_joinStrings_caller joinStrings:[rb_SimpleOnlyStaticModule_joinStrings_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_s_SimpleOnlyStaticModule_joinStrings(int argc, VALUE *argv) {

    id<ISimpleOnlyStaticModuleFactory> factory = [SimpleOnlyStaticModuleFactorySingleton getSimpleOnlyStaticModuleFactoryInstance];
    id<ISimpleOnlyStaticModuleSingleton> singleton = [factory getSimpleOnlyStaticModuleSingleton];
    return rb_SimpleOnlyStaticModule_joinStrings_Obj(argc, argv, singleton);

}







@interface rb_SimpleOnlyStaticModule_getPlatform_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<ISimpleOnlyStaticModuleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(rb_SimpleOnlyStaticModule_getPlatform_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_SimpleOnlyStaticModule_getPlatform_caller_params

@synthesize params,item,methodResult;

+(rb_SimpleOnlyStaticModule_getPlatform_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_SimpleOnlyStaticModule_getPlatform_caller_params* par = [[rb_SimpleOnlyStaticModule_getPlatform_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_SimpleOnlyStaticModule_getPlatform_caller : NSObject {

}
+(rb_SimpleOnlyStaticModule_getPlatform_caller*) getSharedInstance;
+(void) getPlatform:(rb_SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params;
+(void) getPlatform_in_thread:(rb_SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params;
+(void) getPlatform_in_UI_thread:(rb_SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params;

@end

static rb_SimpleOnlyStaticModule_getPlatform_caller* our_SimpleOnlyStaticModule_getPlatform_caller = nil;

@implementation rb_SimpleOnlyStaticModule_getPlatform_caller

+(rb_SimpleOnlyStaticModule_getPlatform_caller*) getSharedInstance {
    if (our_SimpleOnlyStaticModule_getPlatform_caller == nil) {
        our_SimpleOnlyStaticModule_getPlatform_caller = [[rb_SimpleOnlyStaticModule_getPlatform_caller alloc] init];
    }
    return our_SimpleOnlyStaticModule_getPlatform_caller;
}

-(void) command_getPlatform:(rb_SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<ISimpleOnlyStaticModuleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getPlatform:methodResult ];
}

+(void) getPlatform:(rb_SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_getPlatform_caller getSharedInstance] command_getPlatform:caller_params];
}

+(void) getPlatform_in_thread:(rb_SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_getPlatform_caller getSharedInstance] performSelectorInBackground:@selector(command_getPlatform:) withObject:caller_params];
}

+(void) getPlatform_in_UI_thread:(rb_SimpleOnlyStaticModule_getPlatform_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_getPlatform_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getPlatform:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_SimpleOnlyStaticModule_getPlatform_Obj(int argc, VALUE *argv, id<ISimpleOnlyStaticModuleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    
    BOOL is_factory_param[] = { NO };

    int i;

    // init
    for (i = 0; i < (0); i++) {
        params[i] = [NSNull null];
    }

    // enumerate params
    for (int i = 0; i < (0); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = SimpleOnlyStaticModule_makeInstanceByRubyObject(argv[i]);
            }
            else {
                params[i] = [[CRubyConverter convertFromRuby:argv[i]] retain];
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(0)];
    for (i = 0 ; i < (0); i++) {
        [params_array addObject:params[i]];
    }

    
    // check callback
    if (argc >= (0+1)) {
        VALUE callback = argv[0];
        if (rho_ruby_is_string(callback)) {
            callbackURL = [((NSString*)[CRubyConverter convertFromRuby:callback]) retain];
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if (callbackURL != nil) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        [methodResult setRubyCallback:callbackURL];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_SimpleOnlyStaticModule_getPlatform_caller getPlatform_in_thread:[rb_SimpleOnlyStaticModule_getPlatform_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_SimpleOnlyStaticModule_getPlatform_caller getPlatform:[rb_SimpleOnlyStaticModule_getPlatform_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_s_SimpleOnlyStaticModule_getPlatform(int argc, VALUE *argv) {

    id<ISimpleOnlyStaticModuleFactory> factory = [SimpleOnlyStaticModuleFactorySingleton getSimpleOnlyStaticModuleFactoryInstance];
    id<ISimpleOnlyStaticModuleSingleton> singleton = [factory getSimpleOnlyStaticModuleSingleton];
    return rb_SimpleOnlyStaticModule_getPlatform_Obj(argc, argv, singleton);

}







@interface rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<ISimpleOnlyStaticModuleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params

@synthesize params,item,methodResult;

+(rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*) makeParams:(NSArray*)_params _item:(id<ISimpleOnlyStaticModuleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params* par = [[rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller : NSObject {

}
+(rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller*) getSharedInstance;
+(void) showAlertFromUIThread:(rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params;
+(void) showAlertFromUIThread_in_thread:(rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params;
+(void) showAlertFromUIThread_in_UI_thread:(rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params;

@end

static rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller* our_SimpleOnlyStaticModule_showAlertFromUIThread_caller = nil;

@implementation rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller

+(rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller*) getSharedInstance {
    if (our_SimpleOnlyStaticModule_showAlertFromUIThread_caller == nil) {
        our_SimpleOnlyStaticModule_showAlertFromUIThread_caller = [[rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller alloc] init];
    }
    return our_SimpleOnlyStaticModule_showAlertFromUIThread_caller;
}

-(void) command_showAlertFromUIThread:(rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<ISimpleOnlyStaticModuleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem showAlertFromUIThread];
}

+(void) showAlertFromUIThread:(rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller getSharedInstance] command_showAlertFromUIThread:caller_params];
}

+(void) showAlertFromUIThread_in_thread:(rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller getSharedInstance] performSelectorInBackground:@selector(command_showAlertFromUIThread:) withObject:caller_params];
}

+(void) showAlertFromUIThread_in_UI_thread:(rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params*)caller_params {
    [[rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller getSharedInstance] performSelectorOnMainThread:@selector(command_showAlertFromUIThread:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_SimpleOnlyStaticModule_showAlertFromUIThread_Obj(int argc, VALUE *argv, id<ISimpleOnlyStaticModuleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    
    BOOL is_factory_param[] = { NO };

    int i;

    // init
    for (i = 0; i < (0); i++) {
        params[i] = [NSNull null];
    }

    // enumerate params
    for (int i = 0; i < (0); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = SimpleOnlyStaticModule_makeInstanceByRubyObject(argv[i]);
            }
            else {
                params[i] = [[CRubyConverter convertFromRuby:argv[i]] retain];
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(0)];
    for (i = 0 ; i < (0); i++) {
        [params_array addObject:params[i]];
    }

    
    // check callback
    if (argc >= (0+1)) {
        VALUE callback = argv[0];
        if (rho_ruby_is_string(callback)) {
            callbackURL = [((NSString*)[CRubyConverter convertFromRuby:callback]) retain];
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if (callbackURL != nil) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        [methodResult setRubyCallback:callbackURL];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller showAlertFromUIThread_in_UI_thread:[rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller showAlertFromUIThread_in_UI_thread:[rb_SimpleOnlyStaticModule_showAlertFromUIThread_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        method_return_result = NO;
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_s_SimpleOnlyStaticModule_showAlertFromUIThread(int argc, VALUE *argv) {

    id<ISimpleOnlyStaticModuleFactory> factory = [SimpleOnlyStaticModuleFactorySingleton getSimpleOnlyStaticModuleFactoryInstance];
    id<ISimpleOnlyStaticModuleSingleton> singleton = [factory getSimpleOnlyStaticModuleSingleton];
    return rb_SimpleOnlyStaticModule_showAlertFromUIThread_Obj(argc, argv, singleton);

}









