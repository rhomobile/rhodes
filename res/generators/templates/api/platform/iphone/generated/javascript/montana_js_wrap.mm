<%
# NB: each rb_"+$cur_module.name+"_"+module_method.native_name+"_Obj call
# creates CMethodResult inside and releases it after call
# in case if call is sheduled result should be retained
# so makeParams retains it and releases after a call 
%>
#import "I<%= $cur_module.name %>.h"
//#import "api_generator/common/ruby_helpers.h"

#include "api_generator/js_helpers.h"

#import "api_generator/iphone/CMethodResult.h"
#import "api_generator/iphone/CJSConverter.h"

extern VALUE getRuby_<%= $cur_module.name %>_Module();

<%
$iphone_types = {}
$iphone_types["BOOLEAN"] = 'BOOL'
$iphone_types["INTEGER"] = 'int'
$iphone_types["FLOAT"] = 'float'
$iphone_types["STRING"] = 'NSString*'
$iphone_types["ARRAY"] = 'NSArray*'
$iphone_types["HASH"] = 'NSDictionary*'
$iphone_types["SELF_INSTANCE"] = 'id<I'+$cur_module.name+'>'

$iphone_extract_param_prefix = {}
$iphone_extract_param_prefix["BOOLEAN"] = '[((NSNumber*)'
$iphone_extract_param_prefix["INTEGER"] = '[((NSNumber*)'
$iphone_extract_param_prefix["FLOAT"] = '[((NSNumber*)'
$iphone_extract_param_prefix["STRING"] = ''
$iphone_extract_param_prefix["ARRAY"] = ''
$iphone_extract_param_prefix["HASH"] = ''
$iphone_extract_param_prefix["SELF_INSTANCE"] = ''

$iphone_extract_param_suffix = {}
$iphone_extract_param_suffix["BOOLEAN"] = ') boolValue]'
$iphone_extract_param_suffix["INTEGER"] = ') intValue]'
$iphone_extract_param_suffix["FLOAT"] = ') floatValue]'
$iphone_extract_param_suffix["STRING"] = ''
$iphone_extract_param_suffix["ARRAY"] = ''
$iphone_extract_param_suffix["HASH"] = ''
$iphone_extract_param_suffix["SELF_INSTANCE"] = ''
%>

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

<% is_callback_possible = (module_method.run_in_thread != ModuleMethod::RUN_IN_THREAD_NONE) || (module_method.has_callback != ModuleMethod::CALLBACK_NONE) %>

@interface <%= $cur_module.name %>_<%= module_method.native_name %>_caller_params : NSObject

@property (nonatomic, copy) NSArray* params;
@property (assign) id<<%= interface_name %>> item;
@property (assign) CMethodResult* methodResult;

-(void)dealloc;

+(<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*) makeParams:(NSArray*)_params _item:(id<<%= interface_name %>>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation <%= $cur_module.name %>_<%= module_method.native_name %>_caller_params

@synthesize params,item,methodResult;

-(void)dealloc {
    [params release];
    [super dealloc];
}

+(<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*) makeParams:(NSArray*)_params _item:(id<<%= interface_name %>>)_item _methodResult:(CMethodResult*)_methodResult {
    <%= $cur_module.name %>_<%= module_method.native_name %>_caller_params* par = [[[<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params alloc] init] autorelease];
    par.params = _params;
    par.item = _item;
    par.methodResult = [_methodResult retain];
    return [par retain];
}

@end


@interface <%= $cur_module.name %>_<%= module_method.native_name %>_caller : NSObject {

}
+(<%= $cur_module.name %>_<%= module_method.native_name %>_caller*) getSharedInstance;
+(void) <%= module_method.native_name %>:(<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params;
+(void) <%= module_method.native_name %>_in_thread:(<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params;
+(void) <%= module_method.native_name %>_in_UI_thread:(<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params;

@end

static <%= $cur_module.name %>_<%= module_method.native_name %>_caller* our_<%= $cur_module.name %>_<%= module_method.native_name %>_caller = nil;

@implementation <%= $cur_module.name %>_<%= module_method.native_name %>_caller

+(<%= $cur_module.name %>_<%= module_method.native_name %>_caller*) getSharedInstance {
    if (our_<%= $cur_module.name %>_<%= module_method.native_name %>_caller == nil) {
        our_<%= $cur_module.name %>_<%= module_method.native_name %>_caller = [[<%= $cur_module.name %>_<%= module_method.native_name %>_caller alloc] init];
    }
    return our_<%= $cur_module.name %>_<%= module_method.native_name %>_caller;
}

-(void) command_<%= module_method.native_name %>:(<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params {
<% if module_method.params.size > 0 %>
    NSArray* params = caller_params.params;
<% end %>
    id<<%= interface_name %>> objItem = caller_params.item;
    CMethodResult* methodResult = caller_params.methodResult;

    <%
    method_line = "[objItem "+module_method.native_name
    if module_method.params.size > 0
        method_line = method_line + ":(#{$iphone_types[module_method.params[0].type]})#{$iphone_extract_param_prefix[module_method.params[0].type]}[params objectAtIndex:0]#{$iphone_extract_param_suffix[module_method.params[0].type]} "
        for i in 1..(module_method.params.size-1)
            method_line = method_line + "#{module_method.params[i].name}:(#{$iphone_types[module_method.params[i].type]})#{$iphone_extract_param_prefix[module_method.params[i].type]}[params objectAtIndex:#{i}]#{$iphone_extract_param_suffix[module_method.params[i].type]} "
        end
        #if (module_method.has_callback != ModuleMethod::CALLBACK_NONE) || (module_method.is_return_value  || module_method.is_factory_method)
            method_line = method_line + "methodResult:methodResult "
        #end
    else
        #if (module_method.has_callback != ModuleMethod::CALLBACK_NONE) || (module_method.is_return_value  || module_method.is_factory_method)
            method_line = method_line + ":methodResult "
        #end
    end
    method_line = method_line + "];"
    %>
    <%= method_line %>
    [caller_params.methodResult release];
    [caller_params release];
}


+(void) <%= module_method.native_name %>:(<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params {
    [[<%= $cur_module.name %>_<%= module_method.native_name %>_caller getSharedInstance] command_<%= module_method.native_name %>:caller_params];
}

+(void) <%= module_method.native_name %>_in_thread:(<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params {
    [[<%= $cur_module.name %>_<%= module_method.native_name %>_caller getSharedInstance] performSelectorInBackground:@selector(command_<%= module_method.native_name %>:) withObject:caller_params];
}

+(void) <%= module_method.native_name %>_in_UI_thread:(<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params {
    [[<%= $cur_module.name %>_<%= module_method.native_name %>_caller getSharedInstance] performSelectorOnMainThread:@selector(command_<%= module_method.native_name %>:) withObject:caller_params waitUntilDone:NO];
}


@end


<%= "rho::String js_"+$cur_module.name+"_"+module_method.native_name+"_Obj(rho::json::CJSONArray& argv, id<#{interface_name}>objItem, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam) {" %>

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[<%= module_method.params.size %>+1];
    BOOL method_return_result = YES;
    BOOL method_receive_callback = NO;
    int argc = argv.getSize();


    [methodResult setMethodSignature:@"<%= $cur_module.name+"::"+module_method.name %>"];

    <%
     lines = prepare_rho_api_params_structure_lines(module_method.params, 'rho_api_params')
     lines.each do |line|
    %>
    <%= line %><%
     end
    %>

    <%
     factory_params = "BOOL is_factory_param[] = { "
     module_method.params.each do |method_param|
        if method_param.type == RhogenCore::TYPE_CLASS
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

    <% for i in 0..(module_method.params.size-1)
          if module_method.params[i].default_value != nil
             if module_method.params[i].type == RhogenCore::TYPE_STRING
                line = 'params['+i.to_s+']= @"'+module_method.params[i].default_value.to_s+'";'
                %>
                <%= line %><%
             end
             if module_method.params[i].type == RhogenCore::TYPE_INT
                line = 'params['+i.to_s+']= [NSNumber numberWithInt:'+module_method.params[i].default_value.to_s+'];'
                %>
                <%= line %><%
             end
             if module_method.params[i].type == RhogenCore::TYPE_DOUBLE
                line = 'params['+i.to_s+']= [NSNumber numberWithFloat:'+module_method.params[i].default_value.to_s+'];'
                %>
                <%= line %><%
             end
             if module_method.params[i].type == RhogenCore::TYPE_BOOL
                line = ''
                if module_method.params[i].default_value.to_s.downcase != "false"
                     line = 'params['+i.to_s+']= [NSNumber numberWithBool:YES];'
                else
                     line = 'params['+i.to_s+']= [NSNumber numberWithBool:NO];'
                end
                %>
                <%= line %><%
             end
          end
       end
    %>

    // enumerate params
    for (int i = 0; i < (<%= module_method.params.size %>); i++) {
        if (argc > i) {
            // we have a [i] param !
            if (is_factory_param[i]) {
                params[i] = <%= $cur_module.name %>_makeInstanceByJSObject(argv.getItem(i).getString());
            }
            else {
                rho::json::CJSONEntry entry = argv.getItem(i);
                params[i] = [[CJSConverter convertFromJS:&entry rho_api_param:&(rho_api_params[i])] retain];
            }
            // TODO: Handle CMethodResultError
            if (params[i] == nil) {
                NSLog(<%= '@"' + $cur_module.name + '::' + module_method.native_name + ' parameter %d is nil!"' %>, i);
                rho::String resValue = rho::String("\"result\":null,\"error\":\"Method parameter is nil!\"");
                return resValue;
            }
        }
    }

    NSMutableArray* params_array = [NSMutableArray arrayWithCapacity:(<%= module_method.params.size %>)];
    for (i = 0 ; i < (<%= module_method.params.size %>); i++) {
        [params_array addObject:params[i]];
    }

    <% if is_callback_possible %>
        if (strCallbackID.size() > 0) {
            method_receive_callback = YES;
        }
    <% end %>

    <% if module_method.result != nil
        if RhogenCore::BASE_TYPES.include?(module_method.result.type)
            if module_method.result.type == RhogenCore::TYPE_ARRAY
                if !RhogenCore::BASE_TYPES.include?(module_method.result.item_type)
            %>[methodResult enableObjectCreationModeWithJSClassPath:@"<%= api_generator_getJSModuleName(module_method.result.item_type) %>"];<%
                end
            end
        else
            %>[methodResult enableObjectCreationModeWithJSClassPath:@"<%= api_generator_getJSModuleName(module_method.result.type) %>"];<%
        end
    end %>

    if (method_receive_callback) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        [methodResult setJSCallback:[NSString stringWithUTF8String:strCallbackID.c_str()] webViewUID:[NSString stringWithUTF8String:strJsVmID.c_str()]];
        [methodResult setCallbackParam:[NSString stringWithUTF8String:strCallbackParam.c_str()]];
        <% if module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI %>
        [<%= $cur_module.name %>_<%= module_method.native_name %>_caller <%= module_method.native_name %>_in_UI_thread:[<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        <% else %>
        [<%= $cur_module.name %>_<%= module_method.native_name %>_caller <%= module_method.native_name %>_in_thread:[<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        <% end %>

        // FIXME: callback should not be retained, it must be saved outside of this call
        [methodResult retain];
    }
    else {
        // we do not have callback
        <% if module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI %>
        [<%= $cur_module.name %>_<%= module_method.native_name %>_caller <%= module_method.native_name %>_in_UI_thread:[<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        method_return_result = NO;
        <% else
             if (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_MODULE) || (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_SEPARATED) %>
        [<%= $cur_module.name %>_<%= module_method.native_name %>_caller <%= module_method.native_name %>_in_thread:[<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        method_return_result = NO;
             <% else %>
        [<%= $cur_module.name %>_<%= module_method.native_name %>_caller <%= module_method.native_name %>:[<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        <% end
        end%>
    }
    rho::String resValue = rho::String("\"result\":null");
    if ((!method_receive_callback) && (method_return_result)) {
        resValue = [[methodResult toJSON] UTF8String];
    }
    [methodResult release];
    return resValue;
}


<%= api_generator_MakeJSMethodDecl($cur_module.name, module_method.native_name, module_method.access == ModuleMethod::ACCESS_STATIC)%> {
<% if module_method.access == ModuleMethod::ACCESS_STATIC %>
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];
    return <%= "js_"+$cur_module.name+"_"+module_method.native_name %>_Obj(argv, singleton, strCallbackID, strJsVmID, strCallbackParam);
<% else %>
    id<I<%= $cur_module.name %>> item = <%= $cur_module.name %>_makeInstanceByJSObject(strObjID);
    return <%= "js_"+$cur_module.name+"_"+module_method.native_name %>_Obj(argv, item, strCallbackID, strJsVmID, strCallbackParam);
<% end %>
}

<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE
%><%= api_generator_MakeJSMethodDecl($cur_module.name, "def_"+module_method.native_name, true)%> {
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];

    NSString* defID = [singleton getDefaultID];

    id<I<%= $cur_module.name %>> item = [factory get<%= $cur_module.name %>ByID:defID];
    return <%= "js_"+$cur_module.name+"_"+module_method.native_name %>_Obj(argv, item, strCallbackID, strJsVmID, strCallbackParam);
}

<% end %>

<% end %>


<% if $cur_module.is_template_default_instance %>
rho::String js_s_<%= $cur_module.name %>_getDefaultID(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];

    NSString* defID = [singleton getDefaultID];

    rho::String res =  [[CJSConverter convertToJS:defID level:0] UTF8String];

    return res;
}

rho::String js_s_<%= $cur_module.name %>_getDefault(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];

    NSString* defID = [singleton getDefaultID];

    rho::String res =  [[CJSConverter convertToJS:[CRhoAPIClassInstance rubyClassByName:@"<%= api_generator_getJSModuleName(api_generator_getRubyModuleFullName($cur_module))%>" instanceID:defID] level:0] UTF8String];

    return res;
}

rho::String js_s_<%= $cur_module.name %>_setDefaultID(const rho::String& strObjID, rho::json::CJSONArray& argv, const rho::String& strCallbackID, const rho::String& strJsVmID, const rho::String& strCallbackParam)
{
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];

    rho::json::CJSONEntry el = argv[0];

    if (el.isString()) {
        NSString* defID = [NSString stringWithUTF8String:(el.getString())];

        [singleton setDefaultID:defID];

        return [[CJSConverter convertToJS:nil level:0] UTF8String];
    }

    return "\"result\":null,\"error\":\"Method parameter should be defined as string!\"";
}
<% end %>


