<%
# NB: each rb_"+$cur_module.name+"_"+module_method.native_name+"_Obj call
# creates CMethodResult inside and releases it after call
# in case if call is sheduled result should be retained
# so makeParams retains it and releases after a call 
%>
#import "I<%= $cur_module.name %>.h"
//#import "api_generator/common/ruby_helpers.h"

#import "ruby/ext/rho/rhoruby.h"
#import "api_generator/iphone/CMethodResult.h"
#import "api_generator/iphone/CRubyConverter.h"

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

@interface <%= $cur_module.name %>_RubyValueFactory : NSObject<IMethodResult_RubyObjectFactory> {
}

- (VALUE) makeRubyValue:(NSObject*)obj;
+ (<%= $cur_module.name %>_RubyValueFactory*) getSharedInstance;

@end

static <%= $cur_module.name %>_RubyValueFactory* our_<%= $cur_module.name %>_RubyValueFactory = nil;

@implementation <%= $cur_module.name %>_RubyValueFactory

- (VALUE) makeRubyValue:(NSObject*)obj {
    VALUE v = rho_ruby_get_NIL();
    if ([obj isKindOfClass:[NSString class]]) {
        // single objects id
        NSString* strRes = (NSString*)obj;
        v = rho_ruby_create_object_with_id( getRuby_<%= $cur_module.name %>_Module(), [strRes UTF8String] );
    }
    else if ([obj isKindOfClass:[NSArray class]]) {
        // list of IDs
        v = rho_ruby_create_array();
        NSArray* arrRes = (NSArray*)obj;
        int i;
        for (i = 0; i < [arrRes count]; i++) {
            NSString* strItem = (NSString*)[arrRes objectAtIndex:i];
            VALUE vItem = rho_ruby_create_object_with_id( getRuby_<%= $cur_module.name %>_Module(), [strItem UTF8String] );
            rho_ruby_add_to_array(v, vItem);
        }
    }
    return v;
}

+ (<%= $cur_module.name %>_RubyValueFactory*) getSharedInstance {
    if (our_<%= $cur_module.name %>_RubyValueFactory == nil) {
        our_<%= $cur_module.name %>_RubyValueFactory = [[<%= $cur_module.name %>_RubyValueFactory alloc] init];
    }
    return our_<%= $cur_module.name %>_RubyValueFactory;
}

@end


id<I<%= $cur_module.name %>> <%= $cur_module.name %>_makeInstanceByRubyObject(VALUE obj) {
    const char* szID = rho_ruby_get_object_id( obj );
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

@interface rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params : NSObject

@property (nonatomic, copy) NSArray* params;
@property (assign) id<<%= interface_name %>> item;
@property (assign) CMethodResult* methodResult;

-(void)dealloc;

+(<%= "rb_"+$cur_module.name %>_<%= module_method.native_name %>_caller_params*) makeParams:(NSArray*)_params _item:(id<<%= interface_name %>>)_item _methodResult:(CMethodResult*)_methodResult;

@end

@implementation rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params

@synthesize params,item,methodResult;

-(void)dealloc {
    [params release];
    [super dealloc];
}

+(<%= "rb_"+$cur_module.name %>_<%= module_method.native_name %>_caller_params*) makeParams:(NSArray*)_params _item:(id<<%= interface_name %>>)_item _methodResult:(CMethodResult*)_methodResult {
    rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params* par = [[[rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params alloc] init] autorelease];
    par.params = _params;
    par.item = _item;
    par.methodResult = [_methodResult retain];
    return [par retain];
}

@end


@interface rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller : NSObject {

}
+(rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller*) getSharedInstance;
+(void) <%= module_method.native_name %>:(rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params;
+(void) <%= module_method.native_name %>_in_thread:(rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params;
+(void) <%= module_method.native_name %>_in_UI_thread:(rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params;

@end

static rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller* our_<%= $cur_module.name %>_<%= module_method.native_name %>_caller = nil;

@implementation rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller

+(rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller*) getSharedInstance {
    if (our_<%= $cur_module.name %>_<%= module_method.native_name %>_caller == nil) {
        our_<%= $cur_module.name %>_<%= module_method.native_name %>_caller = [[rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller alloc] init];
    }
    return our_<%= $cur_module.name %>_<%= module_method.native_name %>_caller;
}

-(void) command_<%= module_method.native_name %>:(rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params {
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

+(void) <%= module_method.native_name %>:(rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params {
    [[rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller getSharedInstance] command_<%= module_method.native_name %>:caller_params];
}

+(void) <%= module_method.native_name %>_in_thread:(rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params {
    [[rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller getSharedInstance] performSelectorInBackground:@selector(command_<%= module_method.native_name %>:) withObject:caller_params];
}

+(void) <%= module_method.native_name %>_in_UI_thread:(rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params*)caller_params {
    [[rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller getSharedInstance] performSelectorOnMainThread:@selector(command_<%= module_method.native_name %>:) withObject:caller_params waitUntilDone:NO];
}


@end


<%= "VALUE rb_"+$cur_module.name+"_"+module_method.native_name+"_Obj(int argc, VALUE *argv, id<#{interface_name}>objItem) {" %>

    CMethodResult* methodResult = [[CMethodResult alloc] init];

    NSObject* params[<%= module_method.params.size %>+1];
    NSString* callbackURL = nil;
    unsigned long callbackMethod = 0;
    NSString* callbackParam = nil;
    BOOL method_return_result = YES;

    [methodResult setMethodSignature:@"<%= $cur_module.name+"::"+module_method.name %>"];

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
        params[i] = [NSNull null];
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
                params[i] = <%= $cur_module.name %>_makeInstanceByRubyObject(argv[i]);
            }
            else {
                params[i] = [[CRubyConverter convertFromRuby:argv[i]] retain];
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
        VALUE callback = argv[<%= module_method.params.size %>];
        if (rho_ruby_is_string(callback)) {
            callbackURL = [((NSString*)[CRubyConverter convertFromRuby:callback]) retain];
        }
        else if (rho_ruby_is_proc(callback) || rho_ruby_is_method(callback)) {
            callbackMethod = callback;
        }
    }
    // check callback param
    if (argc >= (<%= module_method.params.size %>+2)) {
        VALUE callback_param = argv[<%= module_method.params.size %>+1];
        if (rho_ruby_is_string(callback_param)) {
            callbackParam = [((NSString*)[CRubyConverter convertFromRuby:callback_param]) retain];
        }
    }
    <% end %>

    <% if module_method.is_factory_method %>
    //[methodResult setRubyFactory:[<%= $cur_module.name %>_RubyValueFactory getSharedInstance]];
    //[methodResult enableObjectCreationModeWithRubyClassPath:@"<%= [$cur_module.parents.join("."),$cur_module.name].join(".") %>"];
    <% end %>
    <% if module_method.result != nil
        case module_method.result.type
            when *RhogenCore::BASE_TYPES 
                if module_method.result.type == RhogenCore::TYPE_ARRAY
                    if !RhogenCore::BASE_TYPES.include?(module_method.result.item_type)
                %>[methodResult enableObjectCreationModeWithRubyClassPath:@"<%= module_method.result.item_type %>"];<%
                    end
                end
            when RhogenCore::TYPE_CLASS
                %>[methodResult enableObjectCreationModeWithRubyClassPath:@"<%= module_method.result.self_type %>"];<%
            else
                %>[methodResult enableObjectCreationModeWithRubyClassPath:@"<%= module_method.result.type %>"];<%
        end
    end %>


    if ((callbackURL != nil) || (callbackMethod != 0)) {
        // we have callback - method should not call setResult if method execute from current thread - only later or in UI or separated threads !!!
        if (callbackURL != nil)
            [methodResult setRubyCallback:callbackURL];
        if (callbackMethod != 0)
            [methodResult setRubyCallbackMethod:callbackMethod];
        if (callbackParam != nil) {
            [methodResult setCallbackParam:callbackParam];
        }
        <% if module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI %>
        [rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller <%= module_method.native_name %>_in_UI_thread:[rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        <% else %>
        [rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller <%= module_method.native_name %>_in_thread:[rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        <% end %>
    }
    else {
        // we do not have callback
        <% if module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_UI %>
        [rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller <%= module_method.native_name %>_in_UI_thread:[rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        method_return_result = NO;
        <% else
             if (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_MODULE) || (module_method.run_in_thread == ModuleMethod::RUN_IN_THREAD_SEPARATED) %>
        [rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller <%= module_method.native_name %>_in_thread:[rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        method_return_result = NO;
             <% else %>
        [rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller <%= module_method.native_name %>:[rb_<%= $cur_module.name %>_<%= module_method.native_name %>_caller_params makeParams:params_array _item:objItem _methodResult:methodResult]];
        <% end
        end%>
    }
    VALUE resValue = rho_ruby_get_NIL();
    if ((callbackURL == nil) && (callbackMethod == 0) && (method_return_result)) {
        resValue = [methodResult toRuby];
    }
    [methodResult release];
    return resValue;
}


<%= api_generator_MakeRubyMethodDecl($cur_module.name, module_method, module_method.access == ModuleMethod::ACCESS_STATIC)%> {
<% if module_method.access == ModuleMethod::ACCESS_STATIC %>
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];
    return <%= "rb_"+$cur_module.name+"_"+module_method.native_name %>_Obj(argc, argv, singleton);
<% else %>
    id<I<%= $cur_module.name %>> item = <%= $cur_module.name %>_makeInstanceByRubyObject(obj);
    return <%= "rb_"+$cur_module.name+"_"+module_method.native_name %>_Obj(argc, argv, item);
<% end %>
}

<% if $cur_module.is_template_default_instance && module_method.access == ModuleMethod::ACCESS_INSTANCE
%><%= api_generator_MakeRubyMethodDecl($cur_module.name + "_def", module_method, true)%> {
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];

    NSString* defID = [singleton getDefaultID];

    id<I<%= $cur_module.name %>> item = [factory get<%= $cur_module.name %>ByID:defID];
    return <%= "rb_"+$cur_module.name+"_"+module_method.native_name %>_Obj(argc, argv, item);
}

<% end %>

<% end %>


<% if $cur_module.is_template_default_instance %>
VALUE rb_<%= $cur_module.name %>_s_default(VALUE klass)
{
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];

    NSString* defID = [singleton getDefaultID];

    return rho_ruby_create_object_with_id( klass, [defID UTF8String] );
}

VALUE rb_<%= $cur_module.name %>_s_setDefault(VALUE klass, VALUE valObj)
{
    const char* szID = rho_ruby_get_object_id( valObj );
    id<I<%= $cur_module.name %>Factory> factory = [<%= $cur_module.name %>FactorySingleton get<%= $cur_module.name %>FactoryInstance];
    id<I<%= $cur_module.name %>Singleton> singleton = [factory get<%= $cur_module.name %>Singleton];

    [singleton setDefaultID:[NSString stringWithUTF8String:szID]];

    return rho_ruby_get_NIL();
}
<% end %>


