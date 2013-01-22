#import "../api/I<%= $name %>.h"
//#import "api_generator/common/ruby_helpers.h"

#import "ruby/ext/rho/rhoruby.h"
#import "api_generator/common/iphone/CMethodResult.h"
#import "api_generator/common/iphone/CRubyConverter.h"



<% if $xml_class_attributes_singletone_id %>
VALUE rb_<%= namefixed.downcase %>_s_enumerate(VALUE klass)
{
    
    id<I<%= $name %>Factory> factory = [<%= $name %>FactorySingleton get<%= $name %>FactoryInstance];
    id<I<%= $name %>Singleton> singleton = [factory get<%= $name %>Singleton];
    
    CMethodResult* mr = [[CMethodResult alloc] init];
    
    [singleton enumerate:mr];

    
    NSObject* methodResultValue = [mr getResult];
    
    VALUE v = rho_ruby_get_NIL();
    
    if ([methodResultValue isKindOfClass:[NSString class]]) {
        // single objects id
        NSString* strRes = (NSString*)methodResultValue;
        v = rho_create_object_with_id( klass, [strRes UTF8String] );
    }
    else if ([methodResultValue isKindOfClass:[NSArray class]]) {
        // list of IDs
        v = rho_ruby_create_array();
        NSArray* arrRes = (NSArray*)methodResultValue;
        int i;
        for (i = 0; i < [arrRes count]; i++) {
            NSString* strItem = (NSString*)[arrRes objectAtIndex:i];
            VALUE vItem = rho_create_object_with_id( klass, [strItem UTF8String] );
            rho_ruby_add_to_array(v, vItem);
        }
    }
    [mr release];
    
    return v;

}
<% end %>


<% if $xml_class_attributes_default_instance %>
VALUE rb_<%= namefixed.downcase %>_s_default(VALUE klass)
{
    id<I<%= $name %>Factory> factory = [<%= $name %>FactorySingleton get<%= $name %>FactoryInstance];
    id<I<%= $name %>Singleton> singleton = [factory get<%= $name %>Singleton];

    NSString* defID = [singleton getDefaultID];
    
    return rho_create_object_with_id( klass, [defID UTF8String] );
}

VALUE rb_<%= namefixed.downcase %>_s_set_default(VALUE klass, VALUE valObj)
{
    const char* szID = rho_get_object_id( valObj );
    id<I<%= $name %>Factory> factory = [<%= $name %>FactorySingleton get<%= $name %>FactoryInstance];
    id<I<%= $name %>Singleton> singleton = [factory get<%= $name %>Singleton];

    [singleton setDefaultID:[NSString stringWithUTF8String:szID]];
    
    return rho_ruby_get_NIL();
}
<% end %>



<% if $xml_class_attributes_propertybag %>

// minimum is 2 - because eny method can has callback and callback param !
#define <%= namefixed.downcase %>_getprops_MAX_PARAMS 3

// if this param is YES and no callback - call method and wait in this thread
#define <%= namefixed.downcase %>_getprops_CallInUIThread NO

// if this param is YES and no callback - call method and wait in this thread
#define <%= namefixed.downcase %>_getprops_CallInThread NO

@interface <%= namefixed.downcase %>_getprops_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<I<%= $name %>> item;
@property (assign) CMethodResult* methodResult;

+(<%= namefixed.downcase %>_getprops_caller_params*) makeParams:(NSArray*)_params _item:(id<I<%= $name %>>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation <%= namefixed.downcase %>_getprops_caller_params

@synthesize params,item,methodResult;

+(<%= namefixed.downcase %>_getprops_caller_params*) makeParams:(NSArray*)_params _item:(id<I<%= $name %>>)_item _methodResult:(CMethodResult*)_methodResult {
    <%= namefixed.downcase %>_getprops_caller_params* par = [[<%= namefixed.downcase %>_getprops_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface <%= namefixed.downcase %>_getprops_caller : NSObject {
    
}
+(<%= namefixed.downcase %>_getprops_caller*) getSharedInstance;
+(void) getProps:(<%= namefixed.downcase %>_getprops_caller_params*)caller_params;
+(void) getProps_in_thread:(<%= namefixed.downcase %>_getprops_caller_params*)caller_params;
+(void) getProps_in_UI_thread:(<%= namefixed.downcase %>_getprops_caller_params*)caller_params;

@end

static <%= namefixed.downcase %>_getprops_caller* our_<%= namefixed.downcase %>_getprops_caller = nil;

@implementation <%= namefixed.downcase %>_getprops_caller

+(<%= namefixed.downcase %>_getprops_caller*) getSharedInstance {
    if (our_<%= namefixed.downcase %>_getprops_caller == nil) {
        our_<%= namefixed.downcase %>_getprops_caller = [[<%= namefixed.downcase %>_getprops_caller alloc] init];
    }
    return our_<%= namefixed.downcase %>_getprops_caller;
}

-(void) command_getProps:(<%= namefixed.downcase %>_getprops_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<I<%= $name %>> objItem = caller_params.item;
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

+(void) getProps:(<%= namefixed.downcase %>_getprops_caller_params*)caller_params {
    [[<%= namefixed.downcase %>_getprops_caller getSharedInstance] command_getProps:caller_params];
}

+(void) getProps_in_thread:(<%= namefixed.downcase %>_getprops_caller_params*)caller_params {
    [[<%= namefixed.downcase %>_getprops_caller getSharedInstance] performSelectorInBackground:@selector(command_getProps:) withObject:caller_params];
}

+(void) getProps_in_UI_thread:(<%= namefixed.downcase %>_getprops_caller_params*)caller_params {
    [[<%= namefixed.downcase %>_getprops_caller getSharedInstance] performSelectorOnMainThread:@selector(command_getProps:) withObject:caller_params waitUntilDone:NO];
}


@end

static VALUE <%= namefixed.downcase %>_getprops(int argc, VALUE *argv, id<I<%= $name %>> objItem) {

    CMethodResult* methodResult = [[CMethodResult alloc] init];
    
    NSObject* params[<%= namefixed.downcase %>_getprops_MAX_PARAMS-2];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;

    int i;
    
    // init
    for (i = 0; i < (<%= namefixed.downcase %>_getprops_MAX_PARAMS-2); i++) {
        params[i] = [NSNull null];
    }
    
    // enumerate params
    for (int i = 0; i < (<%= namefixed.downcase %>_getprops_MAX_PARAMS -2); i++) {
        if (argc > i) {
            // we have a [i] param !
            params[i] = [[CRubyConverter convertFromRuby:argv[i]] retain];
        }
    }
    
    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(<%= namefixed.downcase %>_getprops_MAX_PARAMS-2)];
    for (i = 0 ; i < (<%= namefixed.downcase %>_getprops_MAX_PARAMS-2); i++) {
        [params_array addObject:params[i]];
    }
    
    // check callback
    if (argc >= (<%= namefixed.downcase %>_getprops_MAX_PARAMS-1)) {
        VALUE callback = argv[<%= namefixed.downcase %>_getprops_MAX_PARAMS-2];
        if (rho_ruby_is_string(callback)) {
            callbackURL = [((NSString*)[CRubyConverter convertFromRuby:callback]) retain];
        }
    }
    // check callback param
    if (argc >= (<%= namefixed.downcase %>_getprops_MAX_PARAMS)) {
        VALUE callback_param = argv[<%= namefixed.downcase %>_getprops_MAX_PARAMS-1];
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
        if (<%= namefixed.downcase %>_getprops_CallInUIThread) {
            // call in UI thread and do not wait
            [<%= namefixed.downcase %>_getprops_caller getProps_in_UI_thread:[<%= namefixed.downcase %>_getprops_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        }
        else {
            // if we have callback we must call method in separated thread !
            // call in separated thread and do not wait
            
            [<%= namefixed.downcase %>_getprops_caller getProps_in_thread:[<%= namefixed.downcase %>_getprops_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        }
    }
    else {
        // we do not have callback
        if (<%= namefixed.downcase %>_getprops_CallInUIThread) {
            // call in UI thread, wait and return result from this method
            //TODO:
        }
        else if (<%= namefixed.downcase %>_getprops_CallInThread) {
            // call in separated thread (maked for this call), wait and return result from this method
            //TODO:
        }
        else {
            // just call now and return result from this method
            [<%= namefixed.downcase %>_getprops_caller getProps:[<%= namefixed.downcase %>_getprops_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        }
    }
    VALUE resValue = rho_ruby_get_NIL();
    if (callbackURL == nil) {
        resValue = [methodResult toRuby];
        //[methodResult release];
    }
    // free resources
    //for (i = 0; i < (<%= namefixed.downcase %>_getprops_MAX_PARAMS-2); i++) {
    //    [params[i] release];
    //}
    return resValue;
}



VALUE rb_<%= namefixed.downcase %>_s_getprops(int argc, VALUE *argv)
{
    id<I<%= $name %>Factory> factory = [<%= $name %>FactorySingleton get<%= $name %>FactoryInstance];
    id<I<%= $name %>Singleton> singleton = [factory get<%= $name %>Singleton];
    
    NSString* defID = [singleton getDefaultID];
    
    id<I<%= $name %>> item = [factory get<%= $name %>ByID:defID];
    return <%= namefixed.downcase %>_getprops(argc, argv, item);
}

VALUE rb_<%= namefixed.downcase %>_getprops(int argc, VALUE *argv, VALUE valObj)
{
    const char* szID = rho_get_object_id( valObj );
    id<I<%= $name %>Factory> factory = [<%= $name %>FactorySingleton get<%= $name %>FactoryInstance];
    id<I<%= $name %>Singleton> singleton = [factory get<%= $name %>Singleton];
    
    NSString* defID = [NSString stringWithUTF8String:szID];
    
    id<I<%= $name %>> item = [factory get<%= $name %>ByID:defID];
    return <%= namefixed.downcase %>_getprops(argc, argv, item);
}
<% end %>