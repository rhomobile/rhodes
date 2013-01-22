#import "../api/IBarcode1.h"
//#import "../../../common/ruby_helpers.h"

#import "ruby/ext/rho/rhoruby.h"
#import "api_generator/iphone/CMethodResult.h"
#import "api_generator/iphone/CRubyConverter.h"

VALUE rb_barcode1_s_enumerate(VALUE klass)
{
    
    id<IBarcode1Factory> factory = [Barcode1FactorySingleton getBarcode1FactoryInstance];
    id<IBarcode1Singleton> singleton = [factory getBarcode1Singleton];
    
    CMethodResult* mr = [[CMethodResult alloc] init];
    
    [singleton enumerate:mr];

    
    NSObject* methodResultValue = [mr getResult];
    
    VALUE v = rho_ruby_get_NIL();
    
    if ([methodResultValue isKindOfClass:[NSString class]]) {
        // single objects id
        NSString* strRes = (NSString*)methodResultValue;
        v = rho_ruby_create_object_with_id( klass, [strRes UTF8String] );
    }
    else if ([methodResultValue isKindOfClass:[NSArray class]]) {
        // list of IDs
        v = rho_ruby_create_array();
        NSArray* arrRes = (NSArray*)methodResultValue;
        int i;
        for (i = 0; i < [arrRes count]; i++) {
            NSString* strItem = (NSString*)[arrRes objectAtIndex:i];
            VALUE vItem = rho_ruby_create_object_with_id( klass, [strItem UTF8String] );
            rho_ruby_add_to_array(v, vItem);
        }
    }
    [mr release];
    
    return v;
}

VALUE rb_barcode1_s_default(VALUE klass)
{
    id<IBarcode1Factory> factory = [Barcode1FactorySingleton getBarcode1FactoryInstance];
    id<IBarcode1Singleton> singleton = [factory getBarcode1Singleton];

    NSString* defID = [singleton getDefaultID];
    
    return rho_ruby_create_object_with_id( klass, [defID UTF8String] );
}

VALUE rb_barcode1_s_set_default(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    id<IBarcode1Factory> factory = [Barcode1FactorySingleton getBarcode1FactoryInstance];
    id<IBarcode1Singleton> singleton = [factory getBarcode1Singleton];

    [singleton setDefaultID:[NSString stringWithUTF8String:szID]];
    
    return rho_ruby_get_NIL();
}


VALUE rb_barcode1_s_setprops(int argc, VALUE *argv) {
    return rho_ruby_get_NIL();
}

VALUE rb_barcode1_setprops(int argc, VALUE *argv, VALUE obj) {
    return rho_ruby_get_NIL();
}



// minimum is 2 - because eny method can has callback and callback param !
#define barcode1_getprops_MAX_PARAMS 3

// if this param is YES and no callback - call method and wait in this thread
#define barcode1_getprops_CallInUIThread NO

// if this param is YES and no callback - call method and wait in this thread
#define barcode1_getprops_CallInThread NO

@interface barcode1_getprops_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<IBarcode1> item;
@property (assign) CMethodResult* methodResult;

+(barcode1_getprops_caller_params*) makeParams:(NSArray*)_params _item:(id<IBarcode1>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation barcode1_getprops_caller_params

@synthesize params,item,methodResult;

+(barcode1_getprops_caller_params*) makeParams:(NSArray*)_params _item:(id<IBarcode1>)_item _methodResult:(CMethodResult*)_methodResult {
    barcode1_getprops_caller_params* par = [[barcode1_getprops_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface barcode1_getprops_caller : NSObject {
    
}
+(barcode1_getprops_caller*) getSharedInstance;
+(void) getProps:(barcode1_getprops_caller_params*)caller_params;
+(void) getProps_in_thread:(barcode1_getprops_caller_params*)caller_params;
+(void) getProps_in_UI_thread:(barcode1_getprops_caller_params*)caller_params;

@end

static barcode1_getprops_caller* our_barcode1_getprops_caller = nil;

@implementation barcode1_getprops_caller

+(barcode1_getprops_caller*) getSharedInstance {
    if (our_barcode1_getprops_caller == nil) {
        our_barcode1_getprops_caller = [[barcode1_getprops_caller alloc] init];
    }
    return our_barcode1_getprops_caller;
}

-(void) command_getProps:(barcode1_getprops_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<IBarcode1> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    if (params[0] == [NSNull null]) {
        [objItem getProps:methodResult];
    }
    else if ([params[0] isKindOfClass:[NSString class]]) {
        [objItem getPropsWithString:((NSString*)params[0]) methodResult:methodResult];
    }
    else if ([params[0] isKindOfClass:[NSArray class]]) {
        [objItem getPropsWithArray:((NSArray*)params[0]) methodResult:methodResult];
    }
}

+(void) getProps:(barcode1_getprops_caller_params*)caller_params {
    [[barcode1_getprops_caller getSharedInstance] command_getProps:caller_params];
}

+(void) getProps_in_thread:(barcode1_getprops_caller_params*)caller_params {
    [[barcode1_getprops_caller getSharedInstance] performSelectorInBackground:@selector(command_getProps:) withObject:caller_params];
}

+(void) getProps_in_UI_thread:(barcode1_getprops_caller_params*)caller_params {
    [[barcode1_getprops_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getProps:) withObject:caller_params waitUntilDone:NO];
}


@end

static VALUE barcode1_getprops(int argc, VALUE *argv, id<IBarcode1> objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];
    
    NSObject* params[barcode1_getprops_MAX_PARAMS-2];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;

    int i;
    
    // init
    for (i = 0; i < (barcode1_getprops_MAX_PARAMS-2); i++) {
        params[i] = [NSNull null];
    }
    
    // enumerate params
    for (int i = 0; i < (barcode1_getprops_MAX_PARAMS -2); i++) {
        if (argc > i) {
            // we have a [i] param !
            params[i] = [[CRubyConverter convertFromRuby:argv[i]] retain];
        }
    }
    
    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(barcode1_getprops_MAX_PARAMS-2)];
    for (i = 0 ; i < (barcode1_getprops_MAX_PARAMS-2); i++) {
        [params_array addObject:params[i]];
    }
    
    // check callback
    if (argc >= (barcode1_getprops_MAX_PARAMS-1)) {
        VALUE callback = argv[barcode1_getprops_MAX_PARAMS-2];
        if (rho_ruby_is_string(callback)) {
            callbackURL = [((NSString*)[CRubyConverter convertFromRuby:callback]) retain];
        }
    }
    // check callback param
    if (argc >= (barcode1_getprops_MAX_PARAMS)) {
        VALUE callback_param = argv[barcode1_getprops_MAX_PARAMS-1];
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
        if (barcode1_getprops_CallInUIThread) {
            // call in UI thread and do not wait
            [barcode1_getprops_caller getProps_in_UI_thread:[barcode1_getprops_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        }
        else {
            // if we have callback we must call method in separated thread !
            // call in separated thread and do not wait
            
            [barcode1_getprops_caller getProps_in_thread:[barcode1_getprops_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        }
    }
    else {
        // we do not have callback
        if (barcode1_getprops_CallInUIThread) {
            // call in UI thread, wait and return result from this method
            //TODO:
        }
        else if (barcode1_getprops_CallInThread) {
            // call in separated thread (maked for this call), wait and return result from this method
            //TODO:
        }
        else {
            // just call now and return result from this method
            [barcode1_getprops_caller getProps:[barcode1_getprops_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        }
    }
    VALUE resValue = rho_ruby_get_NIL();
    if (callbackURL == nil) {
        resValue = [methodResult toRuby];
        //[methodResult release];
    }
    // free resources
    //for (i = 0; i < (barcode1_getprops_MAX_PARAMS-2); i++) {
    //    [params[i] release];
    //}
    return resValue;
}



VALUE rb_barcode1_s_getprops(int argc, VALUE *argv)
{
    id<IBarcode1Factory> factory = [Barcode1FactorySingleton getBarcode1FactoryInstance];
    id<IBarcode1Singleton> singleton = [factory getBarcode1Singleton];
    
    NSString* defID = [singleton getDefaultID];
    
    id<IBarcode1> item = [factory getBarcode1ByID:defID];
    return barcode1_getprops(argc, argv, item);
}

VALUE rb_barcode1_getprops(int argc, VALUE *argv, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    id<IBarcode1Factory> factory = [Barcode1FactorySingleton getBarcode1FactoryInstance];
    id<IBarcode1Singleton> singleton = [factory getBarcode1Singleton];
    
    NSString* defID = [NSString stringWithUTF8String:szID];
    
    id<IBarcode1> item = [factory getBarcode1ByID:defID];
    return barcode1_getprops(argc, argv, item);
}
