#import "IJavascriptVM.h"
//#import "api_generator/common/ruby_helpers.h"

#import "ruby/ext/rho/rhoruby.h"
#import "api_generator/iphone/CMethodResult.h"
#import "api_generator/iphone/CRubyConverter.h"

extern VALUE getRuby_JavascriptVM_Module();



@interface JavascriptVM_RubyValueFactory : NSObject<IMethodResult_RubyObjectFactory> {
}

- (VALUE) makeRubyValue:(NSObject*)obj;
+ (JavascriptVM_RubyValueFactory*) getSharedInstance;

@end

static JavascriptVM_RubyValueFactory* our_JavascriptVM_RubyValueFactory = nil;

@implementation JavascriptVM_RubyValueFactory

- (VALUE) makeRubyValue:(NSObject*)obj {
    VALUE v = rho_ruby_get_NIL();
    if ([obj isKindOfClass:[NSString class]]) {
        // single objects id
        NSString* strRes = (NSString*)obj;
        v = rho_ruby_create_object_with_id( getRuby_JavascriptVM_Module(), [strRes UTF8String] );
    }
    else if ([obj isKindOfClass:[NSArray class]]) {
        // list of IDs
        v = rho_ruby_create_array();
        NSArray* arrRes = (NSArray*)obj;
        int i;
        for (i = 0; i < [arrRes count]; i++) {
            NSString* strItem = (NSString*)[arrRes objectAtIndex:i];
            VALUE vItem = rho_ruby_create_object_with_id( getRuby_JavascriptVM_Module(), [strItem UTF8String] );
            rho_ruby_add_to_array(v, vItem);
        }
    }
    return v;
}

+ (JavascriptVM_RubyValueFactory*) getSharedInstance {
    if (our_JavascriptVM_RubyValueFactory == nil) {
        our_JavascriptVM_RubyValueFactory = [[JavascriptVM_RubyValueFactory alloc] init];
    }
    return our_JavascriptVM_RubyValueFactory;
}

@end


id<IJavascriptVM> JavascriptVM_makeInstanceByRubyObject(VALUE obj) {
    const char* szID = rho_ruby_get_object_id( obj );
    id<IJavascriptVMFactory> factory = [JavascriptVMFactorySingleton getJavascriptVMFactoryInstance];
    return [factory getJavascriptVMByID:[NSString stringWithUTF8String:szID]];
}









@interface rb_JavascriptVM_executeScript_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IJavascriptVMSingleton> item;
@property (assign) CMethodResult* methodResult;

+(rb_JavascriptVM_executeScript_caller_params*) makeParams:(NSArray*)_params _item:(id<IJavascriptVMSingleton>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_JavascriptVM_executeScript_caller_params

@synthesize params,item,methodResult;

+(rb_JavascriptVM_executeScript_caller_params*) makeParams:(NSArray*)_params _item:(id<IJavascriptVMSingleton>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_JavascriptVM_executeScript_caller_params* par = [[rb_JavascriptVM_executeScript_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface rb_JavascriptVM_executeScript_caller : NSObject {

}
+(rb_JavascriptVM_executeScript_caller*) getSharedInstance;
+(void) executeScript:(rb_JavascriptVM_executeScript_caller_params*)caller_params;
+(void) executeScript_in_thread:(rb_JavascriptVM_executeScript_caller_params*)caller_params;
+(void) executeScript_in_UI_thread:(rb_JavascriptVM_executeScript_caller_params*)caller_params;

@end

static rb_JavascriptVM_executeScript_caller* our_JavascriptVM_executeScript_caller = nil;

@implementation rb_JavascriptVM_executeScript_caller

+(rb_JavascriptVM_executeScript_caller*) getSharedInstance {
    if (our_JavascriptVM_executeScript_caller == nil) {
        our_JavascriptVM_executeScript_caller = [[rb_JavascriptVM_executeScript_caller alloc] init];
    }
    return our_JavascriptVM_executeScript_caller;
}

-(void) command_executeScript:(rb_JavascriptVM_executeScript_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IJavascriptVMSingleton> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    
    [objItem executeScript:(NSString*)[params objectAtIndex:0] methodResult:methodResult ];
}

+(void) executeScript:(rb_JavascriptVM_executeScript_caller_params*)caller_params {
    [[rb_JavascriptVM_executeScript_caller getSharedInstance] command_executeScript:caller_params];
}

+(void) executeScript_in_thread:(rb_JavascriptVM_executeScript_caller_params*)caller_params {
    [[rb_JavascriptVM_executeScript_caller getSharedInstance] performSelectorInBackground:@selector(command_executeScript:) withObject:caller_params];
}

+(void) executeScript_in_UI_thread:(rb_JavascriptVM_executeScript_caller_params*)caller_params {
    [[rb_JavascriptVM_executeScript_caller getSharedInstance] performSelectorOnMainThread:@selector(command_executeScript:) withObject:caller_params waitUntilDone:NO];
}


@end


VALUE rb_JavascriptVM_executeScript_Obj(int argc, VALUE *argv, id<IJavascriptVMSingleton>objItem) {

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
                params[i] = JavascriptVM_makeInstanceByRubyObject(argv[i]);
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
        
        [rb_JavascriptVM_executeScript_caller executeScript_in_thread:[rb_JavascriptVM_executeScript_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    else {
        // we do not have callback
        
        [rb_JavascriptVM_executeScript_caller executeScript:[rb_JavascriptVM_executeScript_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    return resValue;
}


VALUE rb_s_JavascriptVM_executeScript(int argc, VALUE *argv) {

    id<IJavascriptVMFactory> factory = [JavascriptVMFactorySingleton getJavascriptVMFactoryInstance];
    id<IJavascriptVMSingleton> singleton = [factory getJavascriptVMSingleton];
    return rb_JavascriptVM_executeScript_Obj(argc, argv, singleton);

}









