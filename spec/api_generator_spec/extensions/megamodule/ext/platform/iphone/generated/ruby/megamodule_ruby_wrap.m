#import "IMegamodule.h"
//#import "api_generator/common/ruby_helpers.h"

#import "ruby/ext/rho/rhoruby.h"
#import "api_generator/iphone/CMethodResult.h"
#import "api_generator/iphone/CRubyConverter.h"

extern VALUE getRuby_Megamodule_Module();



@interface Megamodule_RubyValueFactory : NSObject<IMethodResult_RubyObjectFactory> {
}

- (VALUE) makeRubyValue:(NSObject*)obj;
+ (Megamodule_RubyValueFactory*) getSharedInstance;

@end

static Megamodule_RubyValueFactory* our_Megamodule_RubyValueFactory = nil;

@implementation Megamodule_RubyValueFactory

- (VALUE) makeRubyValue:(NSObject*)obj {
    VALUE v = rho_ruby_get_NIL();
    if ([obj isKindOfClass:[NSString class]]) {
        // single objects id
        NSString* strRes = (NSString*)obj;
        v = rho_ruby_create_object_with_id( getRuby_Megamodule_Module(), [strRes UTF8String] );
    }
    else if ([obj isKindOfClass:[NSArray class]]) {
        // list of IDs
        v = rho_ruby_create_array();
        NSArray* arrRes = (NSArray*)obj;
        int i;
        for (i = 0; i < [arrRes count]; i++) {
            NSString* strItem = (NSString*)[arrRes objectAtIndex:i];
            VALUE vItem = rho_ruby_create_object_with_id( getRuby_Megamodule_Module(), [strItem UTF8String] );
            rho_ruby_add_to_array(v, vItem);
        }
    }
    return v;
}

+ (Megamodule_RubyValueFactory*) getSharedInstance {
    if (our_Megamodule_RubyValueFactory == nil) {
        our_Megamodule_RubyValueFactory = [[Megamodule_RubyValueFactory alloc] init];
    }
    return our_Megamodule_RubyValueFactory;
}

@end


id<IMegamodule> Megamodule_makeInstanceByRubyObject(VALUE obj) {
    const char* szID = rho_ruby_get_object_id( obj );
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    return [factory getMegamoduleByID:[NSString stringWithUTF8String:szID]];
}









@interface rb_Megamodule_getStringProperty_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_getStringProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_getStringProperty_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_getStringProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_getStringProperty_caller_params* par = [[rb_Megamodule_getStringProperty_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_getStringProperty_caller : NSObject {

}
+(rb_Megamodule_getStringProperty_caller*) getSharedInstance;
+(void) getStringProperty:(rb_Megamodule_getStringProperty_caller_params*)caller_params;
+(void) getStringProperty_in_thread:(rb_Megamodule_getStringProperty_caller_params*)caller_params;
+(void) getStringProperty_in_UI_thread:(rb_Megamodule_getStringProperty_caller_params*)caller_params;

@end

static rb_Megamodule_getStringProperty_caller* our_Megamodule_getStringProperty_caller = nil;

@implementation rb_Megamodule_getStringProperty_caller

+(rb_Megamodule_getStringProperty_caller*) getSharedInstance {
    if (our_Megamodule_getStringProperty_caller == nil) {
        our_Megamodule_getStringProperty_caller = [[rb_Megamodule_getStringProperty_caller alloc] init];
    }
    return our_Megamodule_getStringProperty_caller;
}

-(void) command_getStringProperty:(rb_Megamodule_getStringProperty_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getStringProperty:methodResult ];
}

+(void) getStringProperty:(rb_Megamodule_getStringProperty_caller_params*)caller_params {
    [[rb_Megamodule_getStringProperty_caller getSharedInstance] command_getStringProperty:caller_params];
}

+(void) getStringProperty_in_thread:(rb_Megamodule_getStringProperty_caller_params*)caller_params {
    [[rb_Megamodule_getStringProperty_caller getSharedInstance] performSelectorInBackground:@selector(command_getStringProperty:) withObject:caller_params];
}

+(void) getStringProperty_in_UI_thread:(rb_Megamodule_getStringProperty_caller_params*)caller_params {
    [[rb_Megamodule_getStringProperty_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getStringProperty:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_getStringProperty_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_getStringProperty_caller getStringProperty_in_thread:[rb_Megamodule_getStringProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_getStringProperty_caller getStringProperty:[rb_Megamodule_getStringProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_getStringProperty(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_getStringProperty_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_getStringProperty(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_getStringProperty_Obj(argc, argv, item);
}







@interface rb_Megamodule_getIntegerProperty_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_getIntegerProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_getIntegerProperty_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_getIntegerProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_getIntegerProperty_caller_params* par = [[rb_Megamodule_getIntegerProperty_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_getIntegerProperty_caller : NSObject {

}
+(rb_Megamodule_getIntegerProperty_caller*) getSharedInstance;
+(void) getIntegerProperty:(rb_Megamodule_getIntegerProperty_caller_params*)caller_params;
+(void) getIntegerProperty_in_thread:(rb_Megamodule_getIntegerProperty_caller_params*)caller_params;
+(void) getIntegerProperty_in_UI_thread:(rb_Megamodule_getIntegerProperty_caller_params*)caller_params;

@end

static rb_Megamodule_getIntegerProperty_caller* our_Megamodule_getIntegerProperty_caller = nil;

@implementation rb_Megamodule_getIntegerProperty_caller

+(rb_Megamodule_getIntegerProperty_caller*) getSharedInstance {
    if (our_Megamodule_getIntegerProperty_caller == nil) {
        our_Megamodule_getIntegerProperty_caller = [[rb_Megamodule_getIntegerProperty_caller alloc] init];
    }
    return our_Megamodule_getIntegerProperty_caller;
}

-(void) command_getIntegerProperty:(rb_Megamodule_getIntegerProperty_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getIntegerProperty:methodResult ];
}

+(void) getIntegerProperty:(rb_Megamodule_getIntegerProperty_caller_params*)caller_params {
    [[rb_Megamodule_getIntegerProperty_caller getSharedInstance] command_getIntegerProperty:caller_params];
}

+(void) getIntegerProperty_in_thread:(rb_Megamodule_getIntegerProperty_caller_params*)caller_params {
    [[rb_Megamodule_getIntegerProperty_caller getSharedInstance] performSelectorInBackground:@selector(command_getIntegerProperty:) withObject:caller_params];
}

+(void) getIntegerProperty_in_UI_thread:(rb_Megamodule_getIntegerProperty_caller_params*)caller_params {
    [[rb_Megamodule_getIntegerProperty_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getIntegerProperty:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_getIntegerProperty_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_getIntegerProperty_caller getIntegerProperty_in_thread:[rb_Megamodule_getIntegerProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_getIntegerProperty_caller getIntegerProperty:[rb_Megamodule_getIntegerProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_getIntegerProperty(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_getIntegerProperty_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_getIntegerProperty(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_getIntegerProperty_Obj(argc, argv, item);
}







@interface rb_Megamodule_setIntegerProperty_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_setIntegerProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_setIntegerProperty_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_setIntegerProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_setIntegerProperty_caller_params* par = [[rb_Megamodule_setIntegerProperty_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_setIntegerProperty_caller : NSObject {

}
+(rb_Megamodule_setIntegerProperty_caller*) getSharedInstance;
+(void) setIntegerProperty:(rb_Megamodule_setIntegerProperty_caller_params*)caller_params;
+(void) setIntegerProperty_in_thread:(rb_Megamodule_setIntegerProperty_caller_params*)caller_params;
+(void) setIntegerProperty_in_UI_thread:(rb_Megamodule_setIntegerProperty_caller_params*)caller_params;

@end

static rb_Megamodule_setIntegerProperty_caller* our_Megamodule_setIntegerProperty_caller = nil;

@implementation rb_Megamodule_setIntegerProperty_caller

+(rb_Megamodule_setIntegerProperty_caller*) getSharedInstance {
    if (our_Megamodule_setIntegerProperty_caller == nil) {
        our_Megamodule_setIntegerProperty_caller = [[rb_Megamodule_setIntegerProperty_caller alloc] init];
    }
    return our_Megamodule_setIntegerProperty_caller;
}

-(void) command_setIntegerProperty:(rb_Megamodule_setIntegerProperty_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem setIntegerProperty:(int)[((NSNumber*)[params objectAtIndex:0]) intValue] ];
}

+(void) setIntegerProperty:(rb_Megamodule_setIntegerProperty_caller_params*)caller_params {
    [[rb_Megamodule_setIntegerProperty_caller getSharedInstance] command_setIntegerProperty:caller_params];
}

+(void) setIntegerProperty_in_thread:(rb_Megamodule_setIntegerProperty_caller_params*)caller_params {
    [[rb_Megamodule_setIntegerProperty_caller getSharedInstance] performSelectorInBackground:@selector(command_setIntegerProperty:) withObject:caller_params];
}

+(void) setIntegerProperty_in_UI_thread:(rb_Megamodule_setIntegerProperty_caller_params*)caller_params {
    [[rb_Megamodule_setIntegerProperty_caller getSharedInstance] performSelectorOnMainThread:@selector(command_setIntegerProperty:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_setIntegerProperty_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[1+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    
    BOOL is_factory_param[] = { NO, NO };

    int i;

    // init
    for (i = 0; i < (1); i++) {
        params[i] = [NSNull null];
    }

    
                params[0]= [NSNumber numberWithInt:12345];

    // enumerate params
    for (int i = 0; i < (1); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (1+2)) {
        VALUE callback_param = argv[1+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_setIntegerProperty_caller setIntegerProperty_in_thread:[rb_Megamodule_setIntegerProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_setIntegerProperty_caller setIntegerProperty:[rb_Megamodule_setIntegerProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_setIntegerProperty(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_setIntegerProperty_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_setIntegerProperty(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_setIntegerProperty_Obj(argc, argv, item);
}







@interface rb_Megamodule_typesTest_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_typesTest_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_typesTest_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_typesTest_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_typesTest_caller_params* par = [[rb_Megamodule_typesTest_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_typesTest_caller : NSObject {

}
+(rb_Megamodule_typesTest_caller*) getSharedInstance;
+(void) typesTest:(rb_Megamodule_typesTest_caller_params*)caller_params;
+(void) typesTest_in_thread:(rb_Megamodule_typesTest_caller_params*)caller_params;
+(void) typesTest_in_UI_thread:(rb_Megamodule_typesTest_caller_params*)caller_params;

@end

static rb_Megamodule_typesTest_caller* our_Megamodule_typesTest_caller = nil;

@implementation rb_Megamodule_typesTest_caller

+(rb_Megamodule_typesTest_caller*) getSharedInstance {
    if (our_Megamodule_typesTest_caller == nil) {
        our_Megamodule_typesTest_caller = [[rb_Megamodule_typesTest_caller alloc] init];
    }
    return our_Megamodule_typesTest_caller;
}

-(void) command_typesTest:(rb_Megamodule_typesTest_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem typesTest:(NSString*)[params objectAtIndex:0] paramBool:(BOOL)[((NSNumber*)[params objectAtIndex:1]) boolValue] paramInt:(int)[((NSNumber*)[params objectAtIndex:2]) intValue] paramFloat:(float)[((NSNumber*)[params objectAtIndex:3]) floatValue] paramArray:(NSArray*)[params objectAtIndex:4] paramHash:(NSDictionary*)[params objectAtIndex:5] methodResult:methodResult ];
}

+(void) typesTest:(rb_Megamodule_typesTest_caller_params*)caller_params {
    [[rb_Megamodule_typesTest_caller getSharedInstance] command_typesTest:caller_params];
}

+(void) typesTest_in_thread:(rb_Megamodule_typesTest_caller_params*)caller_params {
    [[rb_Megamodule_typesTest_caller getSharedInstance] performSelectorInBackground:@selector(command_typesTest:) withObject:caller_params];
}

+(void) typesTest_in_UI_thread:(rb_Megamodule_typesTest_caller_params*)caller_params {
    [[rb_Megamodule_typesTest_caller getSharedInstance] performSelectorOnMainThread:@selector(command_typesTest:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_typesTest_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[6+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    
    BOOL is_factory_param[] = { NO, NO, NO, NO, NO, NO, NO };

    int i;

    // init
    for (i = 0; i < (6); i++) {
        params[i] = [NSNull null];
    }

    

    // enumerate params
    for (int i = 0; i < (6); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
            }
            else {
                params[i] = [[CRubyConverter convertFromRuby:argv[i]] retain];
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(6)];
    for (i = 0 ; i < (6); i++) {
        [params_array addObject:params[i]];
    }

    
    // check callback
    if (argc >= (6+1)) {
        VALUE callback = argv[6];
        if (rho_ruby_is_string(callback)) {
            callbackURL = [((NSString*)[CRubyConverter convertFromRuby:callback]) retain];
        }
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (6+2)) {
        VALUE callback_param = argv[6+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_typesTest_caller typesTest_in_thread:[rb_Megamodule_typesTest_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_typesTest_caller typesTest:[rb_Megamodule_typesTest_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_typesTest(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_typesTest_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_typesTest(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_typesTest_Obj(argc, argv, item);
}







@interface rb_Megamodule_produceArray_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_produceArray_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_produceArray_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_produceArray_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_produceArray_caller_params* par = [[rb_Megamodule_produceArray_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_produceArray_caller : NSObject {

}
+(rb_Megamodule_produceArray_caller*) getSharedInstance;
+(void) produceArray:(rb_Megamodule_produceArray_caller_params*)caller_params;
+(void) produceArray_in_thread:(rb_Megamodule_produceArray_caller_params*)caller_params;
+(void) produceArray_in_UI_thread:(rb_Megamodule_produceArray_caller_params*)caller_params;

@end

static rb_Megamodule_produceArray_caller* our_Megamodule_produceArray_caller = nil;

@implementation rb_Megamodule_produceArray_caller

+(rb_Megamodule_produceArray_caller*) getSharedInstance {
    if (our_Megamodule_produceArray_caller == nil) {
        our_Megamodule_produceArray_caller = [[rb_Megamodule_produceArray_caller alloc] init];
    }
    return our_Megamodule_produceArray_caller;
}

-(void) command_produceArray:(rb_Megamodule_produceArray_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem produceArray:methodResult ];
}

+(void) produceArray:(rb_Megamodule_produceArray_caller_params*)caller_params {
    [[rb_Megamodule_produceArray_caller getSharedInstance] command_produceArray:caller_params];
}

+(void) produceArray_in_thread:(rb_Megamodule_produceArray_caller_params*)caller_params {
    [[rb_Megamodule_produceArray_caller getSharedInstance] performSelectorInBackground:@selector(command_produceArray:) withObject:caller_params];
}

+(void) produceArray_in_UI_thread:(rb_Megamodule_produceArray_caller_params*)caller_params {
    [[rb_Megamodule_produceArray_caller getSharedInstance] performSelectorOnMainThread:@selector(command_produceArray:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_produceArray_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_produceArray_caller produceArray_in_thread:[rb_Megamodule_produceArray_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_produceArray_caller produceArray:[rb_Megamodule_produceArray_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_produceArray(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_produceArray_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_produceArray(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_produceArray_Obj(argc, argv, item);
}







@interface rb_Megamodule_produceHash_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_produceHash_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_produceHash_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_produceHash_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_produceHash_caller_params* par = [[rb_Megamodule_produceHash_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_produceHash_caller : NSObject {

}
+(rb_Megamodule_produceHash_caller*) getSharedInstance;
+(void) produceHash:(rb_Megamodule_produceHash_caller_params*)caller_params;
+(void) produceHash_in_thread:(rb_Megamodule_produceHash_caller_params*)caller_params;
+(void) produceHash_in_UI_thread:(rb_Megamodule_produceHash_caller_params*)caller_params;

@end

static rb_Megamodule_produceHash_caller* our_Megamodule_produceHash_caller = nil;

@implementation rb_Megamodule_produceHash_caller

+(rb_Megamodule_produceHash_caller*) getSharedInstance {
    if (our_Megamodule_produceHash_caller == nil) {
        our_Megamodule_produceHash_caller = [[rb_Megamodule_produceHash_caller alloc] init];
    }
    return our_Megamodule_produceHash_caller;
}

-(void) command_produceHash:(rb_Megamodule_produceHash_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem produceHash:methodResult ];
}

+(void) produceHash:(rb_Megamodule_produceHash_caller_params*)caller_params {
    [[rb_Megamodule_produceHash_caller getSharedInstance] command_produceHash:caller_params];
}

+(void) produceHash_in_thread:(rb_Megamodule_produceHash_caller_params*)caller_params {
    [[rb_Megamodule_produceHash_caller getSharedInstance] performSelectorInBackground:@selector(command_produceHash:) withObject:caller_params];
}

+(void) produceHash_in_UI_thread:(rb_Megamodule_produceHash_caller_params*)caller_params {
    [[rb_Megamodule_produceHash_caller getSharedInstance] performSelectorOnMainThread:@selector(command_produceHash:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_produceHash_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_produceHash_caller produceHash_in_thread:[rb_Megamodule_produceHash_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_produceHash_caller produceHash:[rb_Megamodule_produceHash_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_produceHash(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_produceHash_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_produceHash(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_produceHash_Obj(argc, argv, item);
}







@interface rb_Megamodule_produceComplicatedResult_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_produceComplicatedResult_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_produceComplicatedResult_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_produceComplicatedResult_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_produceComplicatedResult_caller_params* par = [[rb_Megamodule_produceComplicatedResult_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_produceComplicatedResult_caller : NSObject {

}
+(rb_Megamodule_produceComplicatedResult_caller*) getSharedInstance;
+(void) produceComplicatedResult:(rb_Megamodule_produceComplicatedResult_caller_params*)caller_params;
+(void) produceComplicatedResult_in_thread:(rb_Megamodule_produceComplicatedResult_caller_params*)caller_params;
+(void) produceComplicatedResult_in_UI_thread:(rb_Megamodule_produceComplicatedResult_caller_params*)caller_params;

@end

static rb_Megamodule_produceComplicatedResult_caller* our_Megamodule_produceComplicatedResult_caller = nil;

@implementation rb_Megamodule_produceComplicatedResult_caller

+(rb_Megamodule_produceComplicatedResult_caller*) getSharedInstance {
    if (our_Megamodule_produceComplicatedResult_caller == nil) {
        our_Megamodule_produceComplicatedResult_caller = [[rb_Megamodule_produceComplicatedResult_caller alloc] init];
    }
    return our_Megamodule_produceComplicatedResult_caller;
}

-(void) command_produceComplicatedResult:(rb_Megamodule_produceComplicatedResult_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem produceComplicatedResult:methodResult ];
}

+(void) produceComplicatedResult:(rb_Megamodule_produceComplicatedResult_caller_params*)caller_params {
    [[rb_Megamodule_produceComplicatedResult_caller getSharedInstance] command_produceComplicatedResult:caller_params];
}

+(void) produceComplicatedResult_in_thread:(rb_Megamodule_produceComplicatedResult_caller_params*)caller_params {
    [[rb_Megamodule_produceComplicatedResult_caller getSharedInstance] performSelectorInBackground:@selector(command_produceComplicatedResult:) withObject:caller_params];
}

+(void) produceComplicatedResult_in_UI_thread:(rb_Megamodule_produceComplicatedResult_caller_params*)caller_params {
    [[rb_Megamodule_produceComplicatedResult_caller getSharedInstance] performSelectorOnMainThread:@selector(command_produceComplicatedResult:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_produceComplicatedResult_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_produceComplicatedResult_caller produceComplicatedResult_in_thread:[rb_Megamodule_produceComplicatedResult_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_produceComplicatedResult_caller produceComplicatedResult:[rb_Megamodule_produceComplicatedResult_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_produceComplicatedResult(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_produceComplicatedResult_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_produceComplicatedResult(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_produceComplicatedResult_Obj(argc, argv, item);
}







@interface rb_Megamodule_getObjectsCount_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamoduleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_getObjectsCount_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_getObjectsCount_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_getObjectsCount_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_getObjectsCount_caller_params* par = [[rb_Megamodule_getObjectsCount_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_getObjectsCount_caller : NSObject {

}
+(rb_Megamodule_getObjectsCount_caller*) getSharedInstance;
+(void) getObjectsCount:(rb_Megamodule_getObjectsCount_caller_params*)caller_params;
+(void) getObjectsCount_in_thread:(rb_Megamodule_getObjectsCount_caller_params*)caller_params;
+(void) getObjectsCount_in_UI_thread:(rb_Megamodule_getObjectsCount_caller_params*)caller_params;

@end

static rb_Megamodule_getObjectsCount_caller* our_Megamodule_getObjectsCount_caller = nil;

@implementation rb_Megamodule_getObjectsCount_caller

+(rb_Megamodule_getObjectsCount_caller*) getSharedInstance {
    if (our_Megamodule_getObjectsCount_caller == nil) {
        our_Megamodule_getObjectsCount_caller = [[rb_Megamodule_getObjectsCount_caller alloc] init];
    }
    return our_Megamodule_getObjectsCount_caller;
}

-(void) command_getObjectsCount:(rb_Megamodule_getObjectsCount_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamoduleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getObjectsCount:methodResult ];
}

+(void) getObjectsCount:(rb_Megamodule_getObjectsCount_caller_params*)caller_params {
    [[rb_Megamodule_getObjectsCount_caller getSharedInstance] command_getObjectsCount:caller_params];
}

+(void) getObjectsCount_in_thread:(rb_Megamodule_getObjectsCount_caller_params*)caller_params {
    [[rb_Megamodule_getObjectsCount_caller getSharedInstance] performSelectorInBackground:@selector(command_getObjectsCount:) withObject:caller_params];
}

+(void) getObjectsCount_in_UI_thread:(rb_Megamodule_getObjectsCount_caller_params*)caller_params {
    [[rb_Megamodule_getObjectsCount_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getObjectsCount:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_getObjectsCount_Obj(int argc, VALUE *argv, id<IMegamoduleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_getObjectsCount_caller getObjectsCount_in_thread:[rb_Megamodule_getObjectsCount_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_getObjectsCount_caller getObjectsCount:[rb_Megamodule_getObjectsCount_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_s_Megamodule_getObjectsCount(int argc, VALUE *argv) {

    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];
    return rb_Megamodule_getObjectsCount_Obj(argc, argv, singleton);

}







@interface rb_Megamodule_getObjectByIndex_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamoduleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_getObjectByIndex_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_getObjectByIndex_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_getObjectByIndex_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_getObjectByIndex_caller_params* par = [[rb_Megamodule_getObjectByIndex_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_getObjectByIndex_caller : NSObject {

}
+(rb_Megamodule_getObjectByIndex_caller*) getSharedInstance;
+(void) getObjectByIndex:(rb_Megamodule_getObjectByIndex_caller_params*)caller_params;
+(void) getObjectByIndex_in_thread:(rb_Megamodule_getObjectByIndex_caller_params*)caller_params;
+(void) getObjectByIndex_in_UI_thread:(rb_Megamodule_getObjectByIndex_caller_params*)caller_params;

@end

static rb_Megamodule_getObjectByIndex_caller* our_Megamodule_getObjectByIndex_caller = nil;

@implementation rb_Megamodule_getObjectByIndex_caller

+(rb_Megamodule_getObjectByIndex_caller*) getSharedInstance {
    if (our_Megamodule_getObjectByIndex_caller == nil) {
        our_Megamodule_getObjectByIndex_caller = [[rb_Megamodule_getObjectByIndex_caller alloc] init];
    }
    return our_Megamodule_getObjectByIndex_caller;
}

-(void) command_getObjectByIndex:(rb_Megamodule_getObjectByIndex_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamoduleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getObjectByIndex:(int)[((NSNumber*)[params objectAtIndex:0]) intValue] methodResult:methodResult ];
}

+(void) getObjectByIndex:(rb_Megamodule_getObjectByIndex_caller_params*)caller_params {
    [[rb_Megamodule_getObjectByIndex_caller getSharedInstance] command_getObjectByIndex:caller_params];
}

+(void) getObjectByIndex_in_thread:(rb_Megamodule_getObjectByIndex_caller_params*)caller_params {
    [[rb_Megamodule_getObjectByIndex_caller getSharedInstance] performSelectorInBackground:@selector(command_getObjectByIndex:) withObject:caller_params];
}

+(void) getObjectByIndex_in_UI_thread:(rb_Megamodule_getObjectByIndex_caller_params*)caller_params {
    [[rb_Megamodule_getObjectByIndex_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getObjectByIndex:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_getObjectByIndex_Obj(int argc, VALUE *argv, id<IMegamoduleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[1+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (1+2)) {
        VALUE callback_param = argv[1+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    [methodResult enableObjectCreationModeWithRubyClassPath:@"Rho.Examples.Megamodule"];


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_getObjectByIndex_caller getObjectByIndex_in_thread:[rb_Megamodule_getObjectByIndex_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_getObjectByIndex_caller getObjectByIndex:[rb_Megamodule_getObjectByIndex_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_s_Megamodule_getObjectByIndex(int argc, VALUE *argv) {

    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];
    return rb_Megamodule_getObjectByIndex_Obj(argc, argv, singleton);

}







@interface rb_Megamodule_showAlertFromUIThread_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_showAlertFromUIThread_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_showAlertFromUIThread_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_showAlertFromUIThread_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_showAlertFromUIThread_caller_params* par = [[rb_Megamodule_showAlertFromUIThread_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_showAlertFromUIThread_caller : NSObject {

}
+(rb_Megamodule_showAlertFromUIThread_caller*) getSharedInstance;
+(void) showAlertFromUIThread:(rb_Megamodule_showAlertFromUIThread_caller_params*)caller_params;
+(void) showAlertFromUIThread_in_thread:(rb_Megamodule_showAlertFromUIThread_caller_params*)caller_params;
+(void) showAlertFromUIThread_in_UI_thread:(rb_Megamodule_showAlertFromUIThread_caller_params*)caller_params;

@end

static rb_Megamodule_showAlertFromUIThread_caller* our_Megamodule_showAlertFromUIThread_caller = nil;

@implementation rb_Megamodule_showAlertFromUIThread_caller

+(rb_Megamodule_showAlertFromUIThread_caller*) getSharedInstance {
    if (our_Megamodule_showAlertFromUIThread_caller == nil) {
        our_Megamodule_showAlertFromUIThread_caller = [[rb_Megamodule_showAlertFromUIThread_caller alloc] init];
    }
    return our_Megamodule_showAlertFromUIThread_caller;
}

-(void) command_showAlertFromUIThread:(rb_Megamodule_showAlertFromUIThread_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem showAlertFromUIThread:(NSString*)[params objectAtIndex:0] ];
}

+(void) showAlertFromUIThread:(rb_Megamodule_showAlertFromUIThread_caller_params*)caller_params {
    [[rb_Megamodule_showAlertFromUIThread_caller getSharedInstance] command_showAlertFromUIThread:caller_params];
}

+(void) showAlertFromUIThread_in_thread:(rb_Megamodule_showAlertFromUIThread_caller_params*)caller_params {
    [[rb_Megamodule_showAlertFromUIThread_caller getSharedInstance] performSelectorInBackground:@selector(command_showAlertFromUIThread:) withObject:caller_params];
}

+(void) showAlertFromUIThread_in_UI_thread:(rb_Megamodule_showAlertFromUIThread_caller_params*)caller_params {
    [[rb_Megamodule_showAlertFromUIThread_caller getSharedInstance] performSelectorOnMainThread:@selector(command_showAlertFromUIThread:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_showAlertFromUIThread_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[1+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (1+2)) {
        VALUE callback_param = argv[1+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_showAlertFromUIThread_caller showAlertFromUIThread_in_UI_thread:[rb_Megamodule_showAlertFromUIThread_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_showAlertFromUIThread_caller showAlertFromUIThread_in_UI_thread:[rb_Megamodule_showAlertFromUIThread_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        method_return_result = NO;
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_showAlertFromUIThread(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_showAlertFromUIThread_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_showAlertFromUIThread(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_showAlertFromUIThread_Obj(argc, argv, item);
}







@interface rb_Megamodule_setPeriodicallyCallback_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_setPeriodicallyCallback_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_setPeriodicallyCallback_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_setPeriodicallyCallback_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_setPeriodicallyCallback_caller_params* par = [[rb_Megamodule_setPeriodicallyCallback_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_setPeriodicallyCallback_caller : NSObject {

}
+(rb_Megamodule_setPeriodicallyCallback_caller*) getSharedInstance;
+(void) setPeriodicallyCallback:(rb_Megamodule_setPeriodicallyCallback_caller_params*)caller_params;
+(void) setPeriodicallyCallback_in_thread:(rb_Megamodule_setPeriodicallyCallback_caller_params*)caller_params;
+(void) setPeriodicallyCallback_in_UI_thread:(rb_Megamodule_setPeriodicallyCallback_caller_params*)caller_params;

@end

static rb_Megamodule_setPeriodicallyCallback_caller* our_Megamodule_setPeriodicallyCallback_caller = nil;

@implementation rb_Megamodule_setPeriodicallyCallback_caller

+(rb_Megamodule_setPeriodicallyCallback_caller*) getSharedInstance {
    if (our_Megamodule_setPeriodicallyCallback_caller == nil) {
        our_Megamodule_setPeriodicallyCallback_caller = [[rb_Megamodule_setPeriodicallyCallback_caller alloc] init];
    }
    return our_Megamodule_setPeriodicallyCallback_caller;
}

-(void) command_setPeriodicallyCallback:(rb_Megamodule_setPeriodicallyCallback_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem setPeriodicallyCallback:(int)[((NSNumber*)[params objectAtIndex:0]) intValue] methodResult:methodResult ];
}

+(void) setPeriodicallyCallback:(rb_Megamodule_setPeriodicallyCallback_caller_params*)caller_params {
    [[rb_Megamodule_setPeriodicallyCallback_caller getSharedInstance] command_setPeriodicallyCallback:caller_params];
}

+(void) setPeriodicallyCallback_in_thread:(rb_Megamodule_setPeriodicallyCallback_caller_params*)caller_params {
    [[rb_Megamodule_setPeriodicallyCallback_caller getSharedInstance] performSelectorInBackground:@selector(command_setPeriodicallyCallback:) withObject:caller_params];
}

+(void) setPeriodicallyCallback_in_UI_thread:(rb_Megamodule_setPeriodicallyCallback_caller_params*)caller_params {
    [[rb_Megamodule_setPeriodicallyCallback_caller getSharedInstance] performSelectorOnMainThread:@selector(command_setPeriodicallyCallback:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_setPeriodicallyCallback_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[1+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (1+2)) {
        VALUE callback_param = argv[1+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_setPeriodicallyCallback_caller setPeriodicallyCallback_in_thread:[rb_Megamodule_setPeriodicallyCallback_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_setPeriodicallyCallback_caller setPeriodicallyCallback:[rb_Megamodule_setPeriodicallyCallback_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_setPeriodicallyCallback(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_setPeriodicallyCallback_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_setPeriodicallyCallback(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_setPeriodicallyCallback_Obj(argc, argv, item);
}







@interface rb_Megamodule_isPeriodicallyCallbackSetted_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_isPeriodicallyCallbackSetted_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_isPeriodicallyCallbackSetted_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_isPeriodicallyCallbackSetted_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_isPeriodicallyCallbackSetted_caller_params* par = [[rb_Megamodule_isPeriodicallyCallbackSetted_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_isPeriodicallyCallbackSetted_caller : NSObject {

}
+(rb_Megamodule_isPeriodicallyCallbackSetted_caller*) getSharedInstance;
+(void) isPeriodicallyCallbackSetted:(rb_Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params;
+(void) isPeriodicallyCallbackSetted_in_thread:(rb_Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params;
+(void) isPeriodicallyCallbackSetted_in_UI_thread:(rb_Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params;

@end

static rb_Megamodule_isPeriodicallyCallbackSetted_caller* our_Megamodule_isPeriodicallyCallbackSetted_caller = nil;

@implementation rb_Megamodule_isPeriodicallyCallbackSetted_caller

+(rb_Megamodule_isPeriodicallyCallbackSetted_caller*) getSharedInstance {
    if (our_Megamodule_isPeriodicallyCallbackSetted_caller == nil) {
        our_Megamodule_isPeriodicallyCallbackSetted_caller = [[rb_Megamodule_isPeriodicallyCallbackSetted_caller alloc] init];
    }
    return our_Megamodule_isPeriodicallyCallbackSetted_caller;
}

-(void) command_isPeriodicallyCallbackSetted:(rb_Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem isPeriodicallyCallbackSetted:methodResult ];
}

+(void) isPeriodicallyCallbackSetted:(rb_Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params {
    [[rb_Megamodule_isPeriodicallyCallbackSetted_caller getSharedInstance] command_isPeriodicallyCallbackSetted:caller_params];
}

+(void) isPeriodicallyCallbackSetted_in_thread:(rb_Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params {
    [[rb_Megamodule_isPeriodicallyCallbackSetted_caller getSharedInstance] performSelectorInBackground:@selector(command_isPeriodicallyCallbackSetted:) withObject:caller_params];
}

+(void) isPeriodicallyCallbackSetted_in_UI_thread:(rb_Megamodule_isPeriodicallyCallbackSetted_caller_params*)caller_params {
    [[rb_Megamodule_isPeriodicallyCallbackSetted_caller getSharedInstance] performSelectorOnMainThread:@selector(command_isPeriodicallyCallbackSetted:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_isPeriodicallyCallbackSetted_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_isPeriodicallyCallbackSetted_caller isPeriodicallyCallbackSetted_in_thread:[rb_Megamodule_isPeriodicallyCallbackSetted_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_isPeriodicallyCallbackSetted_caller isPeriodicallyCallbackSetted:[rb_Megamodule_isPeriodicallyCallbackSetted_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_isPeriodicallyCallbackSetted(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_isPeriodicallyCallbackSetted_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_isPeriodicallyCallbackSetted(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_isPeriodicallyCallbackSetted_Obj(argc, argv, item);
}







@interface rb_Megamodule_stopPeriodicallyCallback_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_stopPeriodicallyCallback_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_stopPeriodicallyCallback_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_stopPeriodicallyCallback_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_stopPeriodicallyCallback_caller_params* par = [[rb_Megamodule_stopPeriodicallyCallback_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_stopPeriodicallyCallback_caller : NSObject {

}
+(rb_Megamodule_stopPeriodicallyCallback_caller*) getSharedInstance;
+(void) stopPeriodicallyCallback:(rb_Megamodule_stopPeriodicallyCallback_caller_params*)caller_params;
+(void) stopPeriodicallyCallback_in_thread:(rb_Megamodule_stopPeriodicallyCallback_caller_params*)caller_params;
+(void) stopPeriodicallyCallback_in_UI_thread:(rb_Megamodule_stopPeriodicallyCallback_caller_params*)caller_params;

@end

static rb_Megamodule_stopPeriodicallyCallback_caller* our_Megamodule_stopPeriodicallyCallback_caller = nil;

@implementation rb_Megamodule_stopPeriodicallyCallback_caller

+(rb_Megamodule_stopPeriodicallyCallback_caller*) getSharedInstance {
    if (our_Megamodule_stopPeriodicallyCallback_caller == nil) {
        our_Megamodule_stopPeriodicallyCallback_caller = [[rb_Megamodule_stopPeriodicallyCallback_caller alloc] init];
    }
    return our_Megamodule_stopPeriodicallyCallback_caller;
}

-(void) command_stopPeriodicallyCallback:(rb_Megamodule_stopPeriodicallyCallback_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem stopPeriodicallyCallback];
}

+(void) stopPeriodicallyCallback:(rb_Megamodule_stopPeriodicallyCallback_caller_params*)caller_params {
    [[rb_Megamodule_stopPeriodicallyCallback_caller getSharedInstance] command_stopPeriodicallyCallback:caller_params];
}

+(void) stopPeriodicallyCallback_in_thread:(rb_Megamodule_stopPeriodicallyCallback_caller_params*)caller_params {
    [[rb_Megamodule_stopPeriodicallyCallback_caller getSharedInstance] performSelectorInBackground:@selector(command_stopPeriodicallyCallback:) withObject:caller_params];
}

+(void) stopPeriodicallyCallback_in_UI_thread:(rb_Megamodule_stopPeriodicallyCallback_caller_params*)caller_params {
    [[rb_Megamodule_stopPeriodicallyCallback_caller getSharedInstance] performSelectorOnMainThread:@selector(command_stopPeriodicallyCallback:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_stopPeriodicallyCallback_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_stopPeriodicallyCallback_caller stopPeriodicallyCallback_in_thread:[rb_Megamodule_stopPeriodicallyCallback_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_stopPeriodicallyCallback_caller stopPeriodicallyCallback:[rb_Megamodule_stopPeriodicallyCallback_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_stopPeriodicallyCallback(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_stopPeriodicallyCallback_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_stopPeriodicallyCallback(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_stopPeriodicallyCallback_Obj(argc, argv, item);
}







@interface rb_Megamodule_complicatedTypesTest1_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_complicatedTypesTest1_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_complicatedTypesTest1_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_complicatedTypesTest1_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_complicatedTypesTest1_caller_params* par = [[rb_Megamodule_complicatedTypesTest1_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_complicatedTypesTest1_caller : NSObject {

}
+(rb_Megamodule_complicatedTypesTest1_caller*) getSharedInstance;
+(void) complicatedTypesTest1:(rb_Megamodule_complicatedTypesTest1_caller_params*)caller_params;
+(void) complicatedTypesTest1_in_thread:(rb_Megamodule_complicatedTypesTest1_caller_params*)caller_params;
+(void) complicatedTypesTest1_in_UI_thread:(rb_Megamodule_complicatedTypesTest1_caller_params*)caller_params;

@end

static rb_Megamodule_complicatedTypesTest1_caller* our_Megamodule_complicatedTypesTest1_caller = nil;

@implementation rb_Megamodule_complicatedTypesTest1_caller

+(rb_Megamodule_complicatedTypesTest1_caller*) getSharedInstance {
    if (our_Megamodule_complicatedTypesTest1_caller == nil) {
        our_Megamodule_complicatedTypesTest1_caller = [[rb_Megamodule_complicatedTypesTest1_caller alloc] init];
    }
    return our_Megamodule_complicatedTypesTest1_caller;
}

-(void) command_complicatedTypesTest1:(rb_Megamodule_complicatedTypesTest1_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem complicatedTypesTest1:(NSArray*)[params objectAtIndex:0] methodResult:methodResult ];
}

+(void) complicatedTypesTest1:(rb_Megamodule_complicatedTypesTest1_caller_params*)caller_params {
    [[rb_Megamodule_complicatedTypesTest1_caller getSharedInstance] command_complicatedTypesTest1:caller_params];
}

+(void) complicatedTypesTest1_in_thread:(rb_Megamodule_complicatedTypesTest1_caller_params*)caller_params {
    [[rb_Megamodule_complicatedTypesTest1_caller getSharedInstance] performSelectorInBackground:@selector(command_complicatedTypesTest1:) withObject:caller_params];
}

+(void) complicatedTypesTest1_in_UI_thread:(rb_Megamodule_complicatedTypesTest1_caller_params*)caller_params {
    [[rb_Megamodule_complicatedTypesTest1_caller getSharedInstance] performSelectorOnMainThread:@selector(command_complicatedTypesTest1:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_complicatedTypesTest1_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[1+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (1+2)) {
        VALUE callback_param = argv[1+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_complicatedTypesTest1_caller complicatedTypesTest1_in_thread:[rb_Megamodule_complicatedTypesTest1_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_complicatedTypesTest1_caller complicatedTypesTest1:[rb_Megamodule_complicatedTypesTest1_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_complicatedTypesTest1(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_complicatedTypesTest1_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_complicatedTypesTest1(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_complicatedTypesTest1_Obj(argc, argv, item);
}







@interface rb_Megamodule_getProperty_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_getProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_getProperty_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_getProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_getProperty_caller_params* par = [[rb_Megamodule_getProperty_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_getProperty_caller : NSObject {

}
+(rb_Megamodule_getProperty_caller*) getSharedInstance;
+(void) getProperty:(rb_Megamodule_getProperty_caller_params*)caller_params;
+(void) getProperty_in_thread:(rb_Megamodule_getProperty_caller_params*)caller_params;
+(void) getProperty_in_UI_thread:(rb_Megamodule_getProperty_caller_params*)caller_params;

@end

static rb_Megamodule_getProperty_caller* our_Megamodule_getProperty_caller = nil;

@implementation rb_Megamodule_getProperty_caller

+(rb_Megamodule_getProperty_caller*) getSharedInstance {
    if (our_Megamodule_getProperty_caller == nil) {
        our_Megamodule_getProperty_caller = [[rb_Megamodule_getProperty_caller alloc] init];
    }
    return our_Megamodule_getProperty_caller;
}

-(void) command_getProperty:(rb_Megamodule_getProperty_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getProperty:(NSString*)[params objectAtIndex:0] methodResult:methodResult ];
}

+(void) getProperty:(rb_Megamodule_getProperty_caller_params*)caller_params {
    [[rb_Megamodule_getProperty_caller getSharedInstance] command_getProperty:caller_params];
}

+(void) getProperty_in_thread:(rb_Megamodule_getProperty_caller_params*)caller_params {
    [[rb_Megamodule_getProperty_caller getSharedInstance] performSelectorInBackground:@selector(command_getProperty:) withObject:caller_params];
}

+(void) getProperty_in_UI_thread:(rb_Megamodule_getProperty_caller_params*)caller_params {
    [[rb_Megamodule_getProperty_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getProperty:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_getProperty_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[1+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (1+2)) {
        VALUE callback_param = argv[1+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_getProperty_caller getProperty_in_thread:[rb_Megamodule_getProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_getProperty_caller getProperty:[rb_Megamodule_getProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_getProperty(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_getProperty_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_getProperty(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_getProperty_Obj(argc, argv, item);
}







@interface rb_Megamodule_getProperties_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_getProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_getProperties_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_getProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_getProperties_caller_params* par = [[rb_Megamodule_getProperties_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_getProperties_caller : NSObject {

}
+(rb_Megamodule_getProperties_caller*) getSharedInstance;
+(void) getProperties:(rb_Megamodule_getProperties_caller_params*)caller_params;
+(void) getProperties_in_thread:(rb_Megamodule_getProperties_caller_params*)caller_params;
+(void) getProperties_in_UI_thread:(rb_Megamodule_getProperties_caller_params*)caller_params;

@end

static rb_Megamodule_getProperties_caller* our_Megamodule_getProperties_caller = nil;

@implementation rb_Megamodule_getProperties_caller

+(rb_Megamodule_getProperties_caller*) getSharedInstance {
    if (our_Megamodule_getProperties_caller == nil) {
        our_Megamodule_getProperties_caller = [[rb_Megamodule_getProperties_caller alloc] init];
    }
    return our_Megamodule_getProperties_caller;
}

-(void) command_getProperties:(rb_Megamodule_getProperties_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getProperties:(NSArray*)[params objectAtIndex:0] methodResult:methodResult ];
}

+(void) getProperties:(rb_Megamodule_getProperties_caller_params*)caller_params {
    [[rb_Megamodule_getProperties_caller getSharedInstance] command_getProperties:caller_params];
}

+(void) getProperties_in_thread:(rb_Megamodule_getProperties_caller_params*)caller_params {
    [[rb_Megamodule_getProperties_caller getSharedInstance] performSelectorInBackground:@selector(command_getProperties:) withObject:caller_params];
}

+(void) getProperties_in_UI_thread:(rb_Megamodule_getProperties_caller_params*)caller_params {
    [[rb_Megamodule_getProperties_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getProperties:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_getProperties_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[1+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (1+2)) {
        VALUE callback_param = argv[1+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_getProperties_caller getProperties_in_thread:[rb_Megamodule_getProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_getProperties_caller getProperties:[rb_Megamodule_getProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_getProperties(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_getProperties_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_getProperties(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_getProperties_Obj(argc, argv, item);
}







@interface rb_Megamodule_getAllProperties_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_getAllProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_getAllProperties_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_getAllProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_getAllProperties_caller_params* par = [[rb_Megamodule_getAllProperties_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_getAllProperties_caller : NSObject {

}
+(rb_Megamodule_getAllProperties_caller*) getSharedInstance;
+(void) getAllProperties:(rb_Megamodule_getAllProperties_caller_params*)caller_params;
+(void) getAllProperties_in_thread:(rb_Megamodule_getAllProperties_caller_params*)caller_params;
+(void) getAllProperties_in_UI_thread:(rb_Megamodule_getAllProperties_caller_params*)caller_params;

@end

static rb_Megamodule_getAllProperties_caller* our_Megamodule_getAllProperties_caller = nil;

@implementation rb_Megamodule_getAllProperties_caller

+(rb_Megamodule_getAllProperties_caller*) getSharedInstance {
    if (our_Megamodule_getAllProperties_caller == nil) {
        our_Megamodule_getAllProperties_caller = [[rb_Megamodule_getAllProperties_caller alloc] init];
    }
    return our_Megamodule_getAllProperties_caller;
}

-(void) command_getAllProperties:(rb_Megamodule_getAllProperties_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem getAllProperties:methodResult ];
}

+(void) getAllProperties:(rb_Megamodule_getAllProperties_caller_params*)caller_params {
    [[rb_Megamodule_getAllProperties_caller getSharedInstance] command_getAllProperties:caller_params];
}

+(void) getAllProperties_in_thread:(rb_Megamodule_getAllProperties_caller_params*)caller_params {
    [[rb_Megamodule_getAllProperties_caller getSharedInstance] performSelectorInBackground:@selector(command_getAllProperties:) withObject:caller_params];
}

+(void) getAllProperties_in_UI_thread:(rb_Megamodule_getAllProperties_caller_params*)caller_params {
    [[rb_Megamodule_getAllProperties_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getAllProperties:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_getAllProperties_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_getAllProperties_caller getAllProperties_in_thread:[rb_Megamodule_getAllProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_getAllProperties_caller getAllProperties:[rb_Megamodule_getAllProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_getAllProperties(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_getAllProperties_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_getAllProperties(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_getAllProperties_Obj(argc, argv, item);
}







@interface rb_Megamodule_setProperty_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_setProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_setProperty_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_setProperty_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_setProperty_caller_params* par = [[rb_Megamodule_setProperty_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_setProperty_caller : NSObject {

}
+(rb_Megamodule_setProperty_caller*) getSharedInstance;
+(void) setProperty:(rb_Megamodule_setProperty_caller_params*)caller_params;
+(void) setProperty_in_thread:(rb_Megamodule_setProperty_caller_params*)caller_params;
+(void) setProperty_in_UI_thread:(rb_Megamodule_setProperty_caller_params*)caller_params;

@end

static rb_Megamodule_setProperty_caller* our_Megamodule_setProperty_caller = nil;

@implementation rb_Megamodule_setProperty_caller

+(rb_Megamodule_setProperty_caller*) getSharedInstance {
    if (our_Megamodule_setProperty_caller == nil) {
        our_Megamodule_setProperty_caller = [[rb_Megamodule_setProperty_caller alloc] init];
    }
    return our_Megamodule_setProperty_caller;
}

-(void) command_setProperty:(rb_Megamodule_setProperty_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem setProperty:(NSString*)[params objectAtIndex:0] propertyValue:(NSString*)[params objectAtIndex:1] ];
}

+(void) setProperty:(rb_Megamodule_setProperty_caller_params*)caller_params {
    [[rb_Megamodule_setProperty_caller getSharedInstance] command_setProperty:caller_params];
}

+(void) setProperty_in_thread:(rb_Megamodule_setProperty_caller_params*)caller_params {
    [[rb_Megamodule_setProperty_caller getSharedInstance] performSelectorInBackground:@selector(command_setProperty:) withObject:caller_params];
}

+(void) setProperty_in_UI_thread:(rb_Megamodule_setProperty_caller_params*)caller_params {
    [[rb_Megamodule_setProperty_caller getSharedInstance] performSelectorOnMainThread:@selector(command_setProperty:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_setProperty_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[2+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (2+2)) {
        VALUE callback_param = argv[2+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_setProperty_caller setProperty_in_thread:[rb_Megamodule_setProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_setProperty_caller setProperty:[rb_Megamodule_setProperty_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_setProperty(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_setProperty_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_setProperty(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_setProperty_Obj(argc, argv, item);
}







@interface rb_Megamodule_setProperties_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_setProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_setProperties_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_setProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_setProperties_caller_params* par = [[rb_Megamodule_setProperties_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_setProperties_caller : NSObject {

}
+(rb_Megamodule_setProperties_caller*) getSharedInstance;
+(void) setProperties:(rb_Megamodule_setProperties_caller_params*)caller_params;
+(void) setProperties_in_thread:(rb_Megamodule_setProperties_caller_params*)caller_params;
+(void) setProperties_in_UI_thread:(rb_Megamodule_setProperties_caller_params*)caller_params;

@end

static rb_Megamodule_setProperties_caller* our_Megamodule_setProperties_caller = nil;

@implementation rb_Megamodule_setProperties_caller

+(rb_Megamodule_setProperties_caller*) getSharedInstance {
    if (our_Megamodule_setProperties_caller == nil) {
        our_Megamodule_setProperties_caller = [[rb_Megamodule_setProperties_caller alloc] init];
    }
    return our_Megamodule_setProperties_caller;
}

-(void) command_setProperties:(rb_Megamodule_setProperties_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem setProperties:(NSDictionary*)[params objectAtIndex:0] ];
}

+(void) setProperties:(rb_Megamodule_setProperties_caller_params*)caller_params {
    [[rb_Megamodule_setProperties_caller getSharedInstance] command_setProperties:caller_params];
}

+(void) setProperties_in_thread:(rb_Megamodule_setProperties_caller_params*)caller_params {
    [[rb_Megamodule_setProperties_caller getSharedInstance] performSelectorInBackground:@selector(command_setProperties:) withObject:caller_params];
}

+(void) setProperties_in_UI_thread:(rb_Megamodule_setProperties_caller_params*)caller_params {
    [[rb_Megamodule_setProperties_caller getSharedInstance] performSelectorOnMainThread:@selector(command_setProperties:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_setProperties_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[1+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (1+2)) {
        VALUE callback_param = argv[1+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_setProperties_caller setProperties_in_thread:[rb_Megamodule_setProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_setProperties_caller setProperties:[rb_Megamodule_setProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_setProperties(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_setProperties_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_setProperties(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_setProperties_Obj(argc, argv, item);
}







@interface rb_Megamodule_clearAllProperties_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamodule> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_clearAllProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_clearAllProperties_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_clearAllProperties_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamodule>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_clearAllProperties_caller_params* par = [[rb_Megamodule_clearAllProperties_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_clearAllProperties_caller : NSObject {

}
+(rb_Megamodule_clearAllProperties_caller*) getSharedInstance;
+(void) clearAllProperties:(rb_Megamodule_clearAllProperties_caller_params*)caller_params;
+(void) clearAllProperties_in_thread:(rb_Megamodule_clearAllProperties_caller_params*)caller_params;
+(void) clearAllProperties_in_UI_thread:(rb_Megamodule_clearAllProperties_caller_params*)caller_params;

@end

static rb_Megamodule_clearAllProperties_caller* our_Megamodule_clearAllProperties_caller = nil;

@implementation rb_Megamodule_clearAllProperties_caller

+(rb_Megamodule_clearAllProperties_caller*) getSharedInstance {
    if (our_Megamodule_clearAllProperties_caller == nil) {
        our_Megamodule_clearAllProperties_caller = [[rb_Megamodule_clearAllProperties_caller alloc] init];
    }
    return our_Megamodule_clearAllProperties_caller;
}

-(void) command_clearAllProperties:(rb_Megamodule_clearAllProperties_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamodule> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem clearAllProperties];
}

+(void) clearAllProperties:(rb_Megamodule_clearAllProperties_caller_params*)caller_params {
    [[rb_Megamodule_clearAllProperties_caller getSharedInstance] command_clearAllProperties:caller_params];
}

+(void) clearAllProperties_in_thread:(rb_Megamodule_clearAllProperties_caller_params*)caller_params {
    [[rb_Megamodule_clearAllProperties_caller getSharedInstance] performSelectorInBackground:@selector(command_clearAllProperties:) withObject:caller_params];
}

+(void) clearAllProperties_in_UI_thread:(rb_Megamodule_clearAllProperties_caller_params*)caller_params {
    [[rb_Megamodule_clearAllProperties_caller getSharedInstance] performSelectorOnMainThread:@selector(command_clearAllProperties:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_clearAllProperties_Obj(int argc, VALUE *argv, id<IMegamodule>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_clearAllProperties_caller clearAllProperties_in_thread:[rb_Megamodule_clearAllProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_clearAllProperties_caller clearAllProperties:[rb_Megamodule_clearAllProperties_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_Megamodule_clearAllProperties(int argc, VALUE *argv, VALUE obj) {

    id<IMegamodule> item = Megamodule_makeInstanceByRubyObject(obj);
    return rb_Megamodule_clearAllProperties_Obj(argc, argv, item);

}

VALUE rb_s_Megamodule_def_clearAllProperties(int argc, VALUE *argv) {
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    id<IMegamodule> item = [factory getMegamoduleByID:defID];
    return rb_Megamodule_clearAllProperties_Obj(argc, argv, item);
}







@interface rb_Megamodule_enumerate_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IMegamoduleSingleton> item;
@property (assign) CMethodResult* methodResult;

+(rb_Megamodule_enumerate_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_Megamodule_enumerate_caller_params

@synthesize params,item,methodResult;

+(rb_Megamodule_enumerate_caller_params*) makeParams:(NSArray*)_params _item:(id<IMegamoduleSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_Megamodule_enumerate_caller_params* par = [[rb_Megamodule_enumerate_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_Megamodule_enumerate_caller : NSObject {

}
+(rb_Megamodule_enumerate_caller*) getSharedInstance;
+(void) enumerate:(rb_Megamodule_enumerate_caller_params*)caller_params;
+(void) enumerate_in_thread:(rb_Megamodule_enumerate_caller_params*)caller_params;
+(void) enumerate_in_UI_thread:(rb_Megamodule_enumerate_caller_params*)caller_params;

@end

static rb_Megamodule_enumerate_caller* our_Megamodule_enumerate_caller = nil;

@implementation rb_Megamodule_enumerate_caller

+(rb_Megamodule_enumerate_caller*) getSharedInstance {
    if (our_Megamodule_enumerate_caller == nil) {
        our_Megamodule_enumerate_caller = [[rb_Megamodule_enumerate_caller alloc] init];
    }
    return our_Megamodule_enumerate_caller;
}

-(void) command_enumerate:(rb_Megamodule_enumerate_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IMegamoduleSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem enumerate:methodResult ];
}

+(void) enumerate:(rb_Megamodule_enumerate_caller_params*)caller_params {
    [[rb_Megamodule_enumerate_caller getSharedInstance] command_enumerate:caller_params];
}

+(void) enumerate_in_thread:(rb_Megamodule_enumerate_caller_params*)caller_params {
    [[rb_Megamodule_enumerate_caller getSharedInstance] performSelectorInBackground:@selector(command_enumerate:) withObject:caller_params];
}

+(void) enumerate_in_UI_thread:(rb_Megamodule_enumerate_caller_params*)caller_params {
    [[rb_Megamodule_enumerate_caller getSharedInstance] performSelectorOnMainThread:@selector(command_enumerate:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_Megamodule_enumerate_Obj(int argc, VALUE *argv, id<IMegamoduleSingleton>objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[0+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
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
                params[i] = Megamodule_makeInstanceByRubyObject(argv[i]);
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
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (0+2)) {
        VALUE callback_param = argv[0+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    

    
    //[methodResult setRubyFactory:[Megamodule_RubyValueFactory getSharedInstance]];
    //[methodResult enableObjectCreationModeWithRubyClassPath:@"Rho.Examples.Megamodule"];
    
    [methodResult enableObjectCreationModeWithRubyClassPath:@"Rho.Examples.Megamodule"];


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        
        [rb_Megamodule_enumerate_caller enumerate_in_thread:[rb_Megamodule_enumerate_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_Megamodule_enumerate_caller enumerate:[rb_Megamodule_enumerate_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_s_Megamodule_enumerate(int argc, VALUE *argv) {

    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];
    return rb_Megamodule_enumerate_Obj(argc, argv, singleton);

}







VALUE rb_Megamodule_s_default(VALUE klass)
{
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    NSString* defID = [singleton getDefaultID];

    return rho_ruby_create_object_with_id( klass, [defID UTF8String] );
}

VALUE rb_Megamodule_s_setDefault(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    id<IMegamoduleFactory> factory = [MegamoduleFactorySingleton getMegamoduleFactoryInstance];
    id<IMegamoduleSingleton> singleton = [factory getMegamoduleSingleton];

    [singleton setDefaultID:[NSString stringWithUTF8String:szID]];

    return rho_ruby_get_NIL();
}



