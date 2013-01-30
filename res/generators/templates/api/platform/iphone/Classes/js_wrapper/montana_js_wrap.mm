#import "../api/I<%= $cur_module.name %>.h"
//#import "api_generator/common/ruby_helpers.h"

#include "api_generator/js_helpers.h"

#import "api_generator/iphone/CMethodResult.h"
#import "api_generator/iphone/CJSConverter.h"

extern VALUE getRuby_<%= $cur_module.name %>_Module();

<% $iphone_types = {}
$iphone_types["STRING"] = 'NSString*'
$iphone_types["ARRAY"] = 'NSArray*'
$iphone_types["HASH"] = 'NSDictionary*'
$iphone_types["SELF_INSTANCE"] = 'id<I'+$cur_module.name+'>' %>



id<I<%= $cur_module.name %>> <%= $cur_module.name %>_makeInstanceByJSObject(rho::String sid) {
    const char* szID = sid.c_str();
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    return [factory get<%= $cur_module.name %>ByID:[NSString stringWithUTF8String:szID]];
}





<% $cur_module.methods.each do |module_method|
     interface_name = "I#{$cur_module.name}"
     if module_method.access == ModuleMethod::ACCESS_STATIC
         interface_name = "I#{$cur_module.name}Singleton"
     end

%>

<% is_callback_possible = module_method.is_run_in_thread || module_method.is_run_in_ui_thread || (module_method.has_callback != ModuleMethod::CALLBACK_NONE) %>

@interface <%= $cur_module.name %>_<%= module_method.name %>_caller_params : NSObject

@property (assign) NSArray* params;
@property (assign) id<<%= interface_name %>> item;
@property (assign) CMethodResult* methodResult;

+(<%= $cur_module.name %>_<%= module_method.name %>_caller_params*) makeParams:(NSArray*)_params _item:(id<<%= interface_name %>>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation <%= $cur_module.name %>_<%= module_method.name %>_caller_params

@synthesize params,item,methodResult;

+(<%= $cur_module.name %>_<%= module_method.name %>_caller_params*) makeParams:(NSArray*)_params _item:(id<<%= interface_name %>>)_item _methodResult:(CMethodResult*)_methodResult {
    <%= $cur_module.name %>_<%= module_method.name %>_caller_params* par = [[<%= $cur_module.name %>_<%= module_method.name %>_caller_params alloc] init];
    par.params = _params;
    par.item = _item;
    par.methodResult = _methodResult;
    return par;
}

@end


@interface <%= $cur_module.name %>_<%= module_method.name %>_caller : NSObject {

}
+(<%= $cur_module.name %>_<%= module_method.name %>_caller*) getSharedInstance;
+(void) <%= module_method.name %>:(<%= $cur_module.name %>_<%= module_method.name %>_caller_params*)caller_params;
+(void) <%= module_method.name %>_in_thread:(<%= $cur_module.name %>_<%= module_method.name %>_caller_params*)caller_params;
+(void) <%= module_method.name %>_in_UI_thread:(<%= $cur_module.name %>_<%= module_method.name %>_caller_params*)caller_params;

@end

static <%= $cur_module.name %>_<%= module_method.name %>_caller* our_<%= $cur_module.name %>_<%= module_method.name %>_caller = nil;

@implementation <%= $cur_module.name %>_<%= module_method.name %>_caller

+(<%= $cur_module.name %>_<%= module_method.name %>_caller*) getSharedInstance {
    if (our_<%= $cur_module.name %>_<%= module_method.name %>_caller == nil) {
        our_<%= $cur_module.name %>_<%= module_method.name %>_caller = [[<%= $cur_module.name %>_<%= module_method.name %>_caller alloc] init];
    }
    return our_<%= $cur_module.name %>_<%= module_method.name %>_caller;
}

-(void) command_<%= module_method.name %>:(<%= $cur_module.name %>_<%= module_method.name %>_caller_params*)caller_params {
    NSArray* params = caller_params.params;
    id<<%= interface_name %>> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    <%
    method_line = "[objItem "+module_method.name
    if module_method.params.size > 0
        method_line = method_line + ":(#{$iphone_types[module_method.params[0].type]})params[0] "
        for i in 1..(module_method.params.size-1)
            method_line = method_line + "#{module_method.params[i].name}:(#{$iphone_types[module_method.params[i].type]})params[#{i}] "
        end
        method_line = method_line + "methodResult:methodResult "
    else
        method_line = method_line + ":methodResult "
    end
    method_line = method_line + "];"
    %>
    <%= method_line %>
}

+(void) <%= module_method.name %>:(<%= $cur_module.name %>_<%= module_method.name %>_caller_params*)caller_params {
    [[<%= $cur_module.name %>_<%= module_method.name %>_caller getSharedInstance] command_<%= module_method.name %>:caller_params];
}

+(void) <%= module_method.name %>_in_thread:(<%= $cur_module.name %>_<%= module_method.name %>_caller_params*)caller_params {
    [[<%= $cur_module.name %>_<%= module_method.name %>_caller getSharedInstance] performSelectorInBackground:@selector(command_<%= module_method.name %>:) withObject:caller_params];
}

+(void) <%= module_method.name %>_in_UI_thread:(<%= $cur_module.name %>_<%= module_method.name %>_caller_params*)caller_params {
    [[<%= $cur_module.name %>_<%= module_method.name %>_caller getSharedInstance] performSelectorOnMainThread:@selector(command_<%= module_method.name %>:) withObject:caller_params waitUntilDone:NO];
}


@end


<%= "rho::String js_"+$cur_module.name+"_"+module_method.name+"_Obj(rho::json::CJSONArray& argv, id<#{interface_name}>objItem) {" %>

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[<%= module_method.params.size %>+1];
    NSString* callbackURL = nil;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;
    int argc = argv.getSize();
    <%
     factory_params = "BOOL is_factory_param[] = { "
     module_method.params.each do |method_param|
        if method_param.type == MethodParam::TYPE_SELF
            factory_params = factory_params + "YES, "
        else
            factory_params = factory_params + "NO, "
        end
     end
     factory_params = factory_params + "NO };"
    %>
    <%= factory_params %>

    int i;

    // init
    for (i = 0; i < (<%= module_method.params.size %>); i++) {
        params[i] = [CJSConverter getObjectiveCNULL];
    }

    // enumerate params
    for (int i = 0; i < (<%= module_method.params.size %>); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = <%= $cur_module.name %>_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry] retain];
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(<%= module_method.params.size %>)];
    for (i = 0 ; i < (<%= module_method.params.size %>); i++) {
        [params_array addObject:params[i]];
    }

    <% if is_callback_possible %>
    // check callback
    if (argc >= (<%= module_method.params.size %>+1)) {
        rho::json::CJSONEntry callback = argv.getItem(<%= module_method.params.size %>);
        if (callback.isString()) {
            rho::json::CJSONEntry entry = argv.getItem(i);
            callbackURL = [((NSString*)[CJSConverter convertFromJS:&callback]) retain];
        }
    }
    // check callback param
    if (argc >= (<%= module_method.params.size %>+2)) {
        rho::json::CJSONEntry callback_param = argv.getItem(<%= module_method.params.size %>+1);
        if (callback_param.isString()) {
            callbackParam = [((NSString*)[CJSConverter convertFromJS:&callback_param]) retain];
        }
    }
    <% end %>

    if (callbackURL != nil) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        [methodResult setJSCallback:callbackURL];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        <% if module_method.is_run_in_ui_thread %>
        [<%= $cur_module.name %>_<%= module_method.name %>_caller <%= module_method.name %>_in_UI_thread:[<%= $cur_module.name %>_<%= module_method.name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        <% else %>
        [<%= $cur_module.name %>_<%= module_method.name %>_caller <%= module_method.name %>_in_thread:[<%= $cur_module.name %>_<%= module_method.name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        <% end %>
    }
    else {
        // we do not have callback
        <% if module_method.is_run_in_ui_thread %>
        [<%= $cur_module.name %>_<%= module_method.name %>_caller <%= module_method.name %>_in_UI_thread:[<%= $cur_module.name %>_<%= module_method.name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        method_return_result = NO;
        <% else
             if module_method.is_run_in_thread %>
        [<%= $cur_module.name %>_<%= module_method.name %>_caller <%= module_method.name %>_in_thread:[<%= $cur_module.name %>_<%= module_method.name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        method_return_result = NO;
             <% else %>
        [<%= $cur_module.name %>_<%= module_method.name %>_caller <%= module_method.name %>:[<%= $cur_module.name %>_<%= module_method.name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        <% end
        end%>
    }
    rho::String resValue = "";
    if ((callbackURL == nil) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    return resValue;
}


<%= api_generator_MakeJSMethodDecl($cur_module.name, module_method.name, module_method.access == ModuleMethod::ACCESS_STATIC)%> {
<% if module_method.access == ModuleMethod::ACCESS_STATIC %>
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];
    return <%= "js_"+$cur_module.name+"_"+module_method.name %>_Obj(argv, singleton);
<% else %>
    id<I<%= $cur_module.name %>> item = <%= $cur_module.name %>_makeInstanceByJSObject(strObjID);
    return <%= "js_"+$cur_module.name+"_"+module_method.name %>_Obj(argv, item);
<% end %>
}

<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE
%><%= api_generator_MakeJSMethodDecl($cur_module.name, "def_"+module_method.name, true)%> {
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];

    NSString* defID = [singleton getDefaultID];

    id<I<%= $cur_module.name %>> item = [factory get<%= $cur_module.name %>ByID:defID];
    return <%= "js_"+$cur_module.name+"_"+module_method.name %>_Obj(argv, item);
}

<% end %>

<% end %>


<% if $cur_module.is_template_default_instance %>
rho::String js_s_<%= $cur_module.name %>_getDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];

    NSString* defID = [singleton getDefaultID];

    rho::String res =  [defID UTF8String];

    return res;
}

rho::String js_s_<%= $cur_module.name %>_setDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID)
{
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];

    [singleton setDefaultID:[NSString stringWithUTF8String:(strObjID.c_str())]];

    return "";
}
<% end %>


