<%
class FalseClass; def to_i; 0 end end
class TrueClass; def to_i; 1 end end

    def js_compatible_name(name)
        # name.end_with?('=') ? 'set' + name[0].upcase + name[1..name.length-2] : name
        name
    end

    def namespace(a_module)
        a_module.parents.clone().push(a_module.name).join(".")
    end

    valid_module_properties = $cur_module.properties.select {|value| 
        !(
             (value.name.nil? || value.name.empty?) ||
             (value.is_deprecated) ||
             (value.readonly && value.writeonly) ||
             (value.type == MethodParam::TYPE_CALLBACK)
        )
    }

    instance_module_properties = valid_module_properties.select {|value| value.access != ModuleMethod::ACCESS_STATIC}
    static_module_properties = valid_module_properties.select {|value| value.access == ModuleMethod::ACCESS_STATIC} 
%>
(function ($, rho, rhoUtil) {
    'use strict';

    var moduleNS = '<%= namespace $cur_module %>';
    var apiReq = rhoUtil.apiReqFor(moduleNS);

<% if $cur_module.entities.size > 0 
%>    // === entities definition === 
<%   $cur_module.entities.each do |entity| %>
    // === <%= entity.name %> object definition ===
<%  # // prepare constants ###########################################
    have_init_fn = entity.fields.size > 0 
    updateable_fields = entity.fields.select { |f| !f.const } 
    updateable_binding_fields = entity.fields.select { |f| !f.const && f.binding }
    updateable_non_binding_fields = entity.fields.select { |f| !f.const && !f.binding }  
    binding_arg = entity.binding_fields.size > 0 ? "binding" : "";
    updateable_binding_arg = entity.binding_fields.size > 0 ? "binding" : "";

    is_update_used = (entity.binding_fields.size > 0) && (updateable_non_binding_fields.size > 0)
%>
     // <%= entity.name %> namespace 
    var entity<%=entity.name%>NS = '<%= namespace($cur_module) + '.' + entity.name  %>'; 
    // <%= entity.name %> data holder
    var <%=entity.name%>Impl = function()
    {
        this.isInitialized = false;
        this.hash = {<%= entity.fields.map{ |f| "#{f.name} : #{f.default_value}" }.join(", ") %>};<% if (entity.binding_fields.size > 0) %>
        this.binding = {}; <% end 
            if is_update_used%>
        this.isChanged = false;
        this.to_update = {}; <% end %>
    } 
<% # // prepare class function ########################################### %>
<% 
    proto_name = [entity.name+"Impl","prototype"].join('.') 

    entity.fields.each do |f| %>
    // <%= entity.name %> field <%= f.name %> access function
    <%= proto_name %>.store<%= f.field_index %> = function(value) { 
        if (null != value && undefined != value) {<% 
            case f.type
                when MethodParam::TYPE_STRING
            %>
            value = (("string" == typeof value) ? value : String(value) );<%
                when MethodParam::TYPE_INT
            %>
            value = (("number" == typeof value) ? value : parseInt(value) ); <%
                when MethodParam::TYPE_DOUBLE
            %>
            value = (("number" == typeof value) ? value : parseFloat(value) ); <%
                when MethodParam::TYPE_BOOL
            %>
            value = (("boolean" == typeof value) ? value : (value.toLowerCase() == "true") ); <%
            end %>
            this.hash.<%= f.name %> = value; <%
        if f.binding 
            if entity.binding_fields.size == 1 %>this.binding = value;<% 
            else %>this.binding.<%= f.name %> = value; <% 
            end 
        end %>
            return value
        }   
        return null
    }<%
        if f.is_readable %>
    <%= proto_name %>.get<%= f.field_index %> = function() { 
        return this.hash.<%= f.name %>; 
    } <% 
        end 
        if f.is_writeable %>
    <%= proto_name %>.set<%= f.field_index %> = function(value) { 
        value = this.store<%= f.field_index %>(value);
        if (null != value) {
            <% if is_update_used %>this.to_update["<%= f.name %>"] = value; this.isChanged = true; <% end %>
        }
    } <% 
        end %>
 
<%
    end
    
    init_args = [];
    if (updateable_fields.size > 0)
        if (updateable_fields.size == 1)
            init_args << "hash.#{updateable_fields[0].name}"
        else
            init_args << "hash"
        end
    end

    if have_init_fn 

    set_args = []
    set_args << "binding" if (entity.binding_fields.size > 0)
    set_args << "hash" %>
    // <%= entity.name %> fields initialization
    <%= proto_name %>.init = function(fields)
    {
<%      entity.fields.each do |f| 
%>        this.store<%= f.field_index %>(fields.<%= f.name%>);
<%      end 
%>        this.isInitialized = true;
    } 
    <% end

    have_update_fn = updateable_fields.size > 0 
    if have_update_fn 
        update_args = []
        update_args << "binding" if (updateable_binding_fields.size > 0)
        update_args << "hash" 
    %>  // <%= entity.name %> fields update
    <%= proto_name %>.update = function(fields)
    {
        if (null != fields) {
<%      entity.fields.each do |f| 
            next if f.const 
%>            this.store<%= f.field_index %>(fields.<%= f.name%>);
<%      end 
%>            this.to_update = {}; this.isChanged = false; 
        }
    }
    <% end %>
    <%
    fn_update_args = []
    if entity.binding_fields.size > 0
        fn_update_args << "binding" 
        if updateable_non_binding_fields.size > 0
            fn_update_args << "to_update"
        end
    else
        fn_update_args << "hash"
    end
%><%
    entity.methods.each do |entity_method|
        next if !entity_method.is_generated 

        params = entity_method.params.map do |param|
            "/* #{api_generator_cpp_makeNativeTypeArg(param.type)} */ #{param.name}"
        end.push("/* optional function */ oResult").join(', ')

%>    // function(<%= params %>)
    var fn<%=entity_method.name.camelcase%> = rhoUtil.methodAccessReqFunc(<%
       %>'<%= entity_method.name %>', <%= 
        if entity_method.has_callback != ModuleMethod::CALLBACK_NONE 
            " #{entity_method.params.size},"
        else
            "null," 
        end%> <%= 
        entity_method.params.size + (entity_method.has_callback == ModuleMethod::CALLBACK_NONE ? 0 : 2) %>, apiReq); 
<%  end %>    // <%= entity.name %> push changes to native 
    <%=proto_name %>.save = function() {
        if (this.isInitialized) {
<% if entity.update_method 
%>            if (this.isChanged) {
                <%= "var updateobj = " if have_update_fn %>fn<%=entity.update_method.name.camelcase%>(<%= fn_update_args.map{ |arg| "this.#{arg}" }.join(', ') %>);
                <% if have_update_fn 
                %>this.update(updateobj); <%  end %>
            } 
<% end %>        }  else {
            <% if entity.init_method != nil 
                %><%= "var init_object = " if have_init_fn %>fn<%=entity.init_method.name.camelcase%>(<%=init_args.map{ |arg| "this.#{arg}" }.join(', ')%>); <% 
                if have_init_fn 
                    %>
            this.init(init_object); <% else %>
            this.isInitialized = true;  <% end 
            end %>
        }
    }
    // <%= entity.name %> wrapper
    function <%= entity.name %>() {
        this.__impl = new <%= entity.name %>Impl();

        
        if (1 == arguments.length) {
            var init_object = arguments[0][rhoUtil.rhoIdParam()];
            if (init_object) {
                if (entity<%=entity.name%>NS != arguments[0][rhoUtil.rhoClassParam()]) {
                    throw "Wrong class instantiation!";
                } 
                <% if have_init_fn 
                %>this.__impl.init(init_object); <% else %>this.__impl.isInitialized = true; <% 
                end %>
            }
        }         
    };

<%  proto_name = [entity.name,"prototype"].join('.')
    entity.fields.each do |f| %>
    // <%= entity.name %> field access function  
    <% if f.is_readable 
        %><%= proto_name %>.get<%= f.name.capitalize_first %> = function() { return this.__impl.get<%= f.field_index %>(); }
    <% end %>
    <% if f.is_writeable
        %><%= proto_name %>.set<%= f.name.capitalize_first %> = function(value) { this.__impl.set<%= f.field_index %>(value); }
    <% end  
    end
%>
    <%=proto_name %>.save = function() { this.__impl.save(); }

<%  
    entity.methods.select{|m| !m.is_generated }.each do |entity_method|
        params = entity_method.params.map do |param|
            "/* #{api_generator_cpp_makeNativeTypeArg(param.type)} */ #{param.name}"
        end.push("/* optional function */ oResult").join(', ')

        call_params = []
        if ( !entity_method.is_static_for_entity)
            if entity.binding_fields.size > 0 
                call_params << "this.__impl.binding"
            else
                if entity.fields.size > 0 
                    call_params << "this.__impl.hash"
                end
            end
        end

%>    // function <%=entity_method.name.camelcase%> (<%= params %>)
    var fn<%=entity_method.binding_name.camelcase%> = rhoUtil.methodAccessReqFunc(<%
       %>'<%= entity_method.binding_name %>', <%= 
        if entity_method.has_callback != ModuleMethod::CALLBACK_NONE 
            " #{entity_method.params.size},"
        else
            "null," 
        end%> <%= 
        entity_method.params.size + (entity_method.has_callback == ModuleMethod::CALLBACK_NONE ? 0 : 2) %>, apiReq); <%
    fn_call_params = entity_method.params.select{|param| !param.is_generated}.map{|param| "#{param.name}"}
    %>
    <%= entity.name %><%=entity_method.is_static_for_entity ? "" : ".prototype"%>.<%=entity_method.name%> = function(<%= fn_call_params.join(', ')%>){
        <%= "this.__impl.save();
        " if !entity_method.is_static_for_entity %><%= 
        "return " if entity_method.is_return_value %>fn<%=entity_method.binding_name.camelcase%>(<%= (call_params + fn_call_params).join(', ') %>);
    };

<%   end  %>
    rhoUtil.createRawPropsProxy(<%= entity.name %>.prototype, [<% 
        first_prop = true
        entity.fields.each do |f|
            next if f.name.nil? || f.name.empty?
%>
         <%= first_prop ? '  ' : ', ' %>{ propName: '<%= f.name %>'<%
    %>, propGetter : <%
            if f.is_readable 
                %><%= proto_name %>.get<%= f.name.capitalize_first %><%      
            else %>null<% 
            end 
    %>, propSetter : <%
            if f.is_writeable 
                %><%= proto_name %>.set<%= f.name.capitalize_first %><%      
            else %>null<%
            end %> } <% 
            first_prop = false
        end %>
    ]);
<%   
    end  
end %>
    // === <%= $cur_module.name %> class definition ===

    function <%= $cur_module.name %>() {
        var id = null;
        this.getId = function () {return id;};

        if (1 == arguments.length && arguments[0][rhoUtil.rhoIdParam()]) {
            if (moduleNS != arguments[0][rhoUtil.rhoClassParam()]) {
                throw "Wrong class instantiation!";
            }
            id = arguments[0][rhoUtil.rhoIdParam()];
        } else {
            id = rhoUtil.nextId();
            // constructor methods are following:
            <% ($cur_module.methods.select{|m| m.is_constructor}).each do |module_method| %>
                this.<%= js_compatible_name module_method.binding_name %>.apply(this, arguments);
            <% end %>
        }
    };

    // === <%= $cur_module.name %> instance properties ===

    rhoUtil.createPropsProxy(<%= $cur_module.name %>.prototype, [
    <% first_prop = true
        instance_module_properties.each do |module_property|
            rw_mode = ''
            rw_mode += 'r' if !module_property.writeonly
            rw_mode += 'w' if !module_property.readonly 
    %>  <%= first_prop ? '  ' : ', ' %>{ propName: '<%= module_property.name %>', propAccess: '<%= rw_mode %>' }
    <% first_prop = false
       end %>], apiReq, function(){ return this.getId(); });

    // === <%= $cur_module.name %> instance methods ===

    rhoUtil.createMethodsProxy(<%= $cur_module.name %>.prototype, [
    <%  first_method = true
        $cur_module.methods.each do |module_method|
        next if module_method.access == ModuleMethod::ACCESS_STATIC
        next if module_method.is_accessor
        next if module_method.is_deprecated

        params = module_method.params.map do |param|
            "/* #{api_generator_cpp_makeNativeTypeArg(param.type)} */ #{param.name}"
        end.push("/* optional function */ oResult").join(', ')
    %>
          // function(<%= params %>)
        <%= first_method ? '  ' : ', ' %>{ methodName: '<%= js_compatible_name module_method.binding_name %>', nativeName: '<%= module_method.binding_name %>',<%= " persistentCallbackIndex: #{module_method.params.size}," if module_method.has_callback != ModuleMethod::CALLBACK_NONE %> valueCallbackIndex: <%= module_method.params.size + (module_method.has_callback == ModuleMethod::CALLBACK_NONE ? 0 : 2) %> }
    <% first_method = false
       end %>
    ], apiReq, function(){ return this.getId(); });

    <%
    callback_accessors = $cur_module.methods.select{ |m| m.is_callback_accessor }

    callback_accessors.each do |module_method|
        fn_signature = "#{$cur_module.name}.prototype.#{module_method.native_name}"
        fn_calling = "";

    %><%= fn_signature %> = rhoUtil.methodAccessReqFunc(<%
       %>'<%= module_method.binding_name %>', <%= 
        if module_method.has_callback != ModuleMethod::CALLBACK_NONE 
            " #{module_method.params.size},"
        else
            "null," 
        end%> <%= 
        module_method.params.size + (module_method.has_callback == ModuleMethod::CALLBACK_NONE ? 0 : 2) %>, apiReq, function(){ return this.getId(); } );
    <% end %>

    rhoUtil.createRawPropsProxy(<%= $cur_module.name %>.prototype, [<% 
        first_prop = true
        callback_accessors.each do |m|
            fn_signature = "#{$cur_module.name}.prototype.#{m.native_name}"        
%>
         <%= first_prop ? '  ' : ', ' %>{ propName: '<%= m.linked_property.name %>'<%
    %>, propSetter : <%=fn_signature  %>} <% 
            first_prop = false
        end %>
    ]);

    // === <%= $cur_module.name %> constants ===

    <% $cur_module.constants.each do |module_constant|
        # next if module_constant.is_deprecated
        next if module_constant.name.nil? || module_constant.name.empty?

        if module_constant.type == MethodParam::TYPE_STRING %>
            <%= $cur_module.name %>.<%= module_constant.name %> = '<%= module_constant.value %>'; <% 
        else %>
            <%= $cur_module.name %>.<%= module_constant.name %> = <%= module_constant.value %>;<% 
        end %>
    <% end %>

<% if false && $cur_module.hash_key_names.size > 0 %>
    // === <%= $cur_module.name %> hash keys ===
    
    <% $cur_module.hash_key_names.each do |key_names| %>
            <%= $cur_module.name %>.<%= key_names.const_tag %> = "<%= key_names.name %>"; 
<% end 
end %>

    // === <%= $cur_module.name %> static properties ===

    rhoUtil.createPropsProxy(<%= $cur_module.name %>, [
    <% first_prop = true
        static_module_properties.each do |module_property|
            rw_mode = ''
            rw_mode += 'r' if !module_property.writeonly
            rw_mode += 'w' if !module_property.readonly
    %>  <%= first_prop ? '  ' : ', ' %>{ propName: '<%= module_property.name %>', propAccess: '<%= rw_mode %>' }
    <% first_prop = false
       end %>], apiReq);

    // === <%= $cur_module.name %> static methods ===

    rhoUtil.createMethodsProxy(<%= $cur_module.name %>, [
    <%  first_method = true
        $cur_module.methods.each do |module_method|
        next if module_method.access != ModuleMethod::ACCESS_STATIC
        next if module_method.is_accessor
        next if module_method.is_deprecated
        next if module_method.linked_entity

        params = module_method.params.map do |param|
            "/* #{api_generator_cpp_makeNativeTypeArg(param.type)} */ #{param.name}"
        end.push("/* optional function */ oResult").join(', ')
    %>
          // function(<%= params %>)
        <%= first_method ? '  ' : ', ' %>{ methodName: '<%= js_compatible_name module_method.binding_name %>', nativeName: '<%= module_method.binding_name %>',<%= " persistentCallbackIndex: #{module_method.params.size}," if module_method.has_callback != ModuleMethod::CALLBACK_NONE %> valueCallbackIndex: <%= module_method.params.size + (module_method.has_callback == ModuleMethod::CALLBACK_NONE ? 0 : 2) %> }
    <% first_method = false
       end %>
    ], apiReq);

    // === <%= $cur_module.name %> default instance support ===
    <% if $cur_module.is_template_default_instance %>

        rhoUtil.createPropsProxy(<%= $cur_module.name %>, [
            { propName: 'defaultInstance:getDefault:setDefault', propAccess: 'rw', customSet: function(obj) { if(!obj || 'function' != typeof obj.getId){ throw 'Default object should provide getId method!' }; <%= $cur_module.name %>.setDefaultID(obj.getId()); } }
          , { propName: 'defaultID:getDefaultID:setDefaultID', propAccess: 'rw' }
        ], apiReq);

        <%= $cur_module.name %>.getId = function() {
            return <%= $cur_module.name %>.getDefaultID();
        }

        // === <%= $cur_module.name %> default instance properties ===

        rhoUtil.createPropsProxy(<%= $cur_module.name %>, [
        <% first_prop = true
           $cur_module.properties.each do |module_property|
             next if module_property.access == ModuleMethod::ACCESS_STATIC
             next if module_property.is_deprecated
             next if module_property.readonly && module_property.writeonly
        %>  <%= first_prop ? '  ' : ', ' %>{ propName: '<%= module_property.name %>', propAccess: '<%= 'r' if !module_property.writeonly %><%= 'w' if !module_property.readonly %>' }
        <% first_prop = false
           end %>], apiReq, function(){ return this.getId(); });

        // === <%= $cur_module.name %> default instance methods ===

        rhoUtil.createMethodsProxy(<%= $cur_module.name %>, [
        <%  first_method = true
            $cur_module.methods.each do |module_method|
            next if module_method.access == ModuleMethod::ACCESS_STATIC
            next if module_method.is_accessor
            next if module_method.is_deprecated

            params = module_method.params.map do |param|
                "/* #{api_generator_cpp_makeNativeTypeArg(param.type)} */ #{param.name}"
            end.push("/* optional function */ oResult").join(', ')
        %>
              // function(<%= params %>)
            <%= first_method ? '  ' : ', ' %>{ methodName: '<%= js_compatible_name module_method.binding_name %>', nativeName: '<%= module_method.binding_name %>',<%= " persistentCallbackIndex: #{module_method.params.size}," if module_method.has_callback != ModuleMethod::CALLBACK_NONE %> valueCallbackIndex: <%= module_method.params.size + (module_method.has_callback == ModuleMethod::CALLBACK_NONE ? 0 : 2) %> }
        <% first_method = false
           end %>
        ], apiReq, function(){ return this.getId(); });

        // will reuse already defined methods
        rhoUtil.createRawPropsProxy(<%= $cur_module.name %>, [<% 
            first_prop = true
            callback_accessors.each do |m|
                fn_signature = "#{$cur_module.name}.prototype.#{m.native_name}"        
    %>
             <%= first_prop ? '  ' : ', ' %>{ propName: '<%= m.linked_property.name %>'<%
        %>, propSetter : <%=fn_signature  %>} <% 
                first_prop = false
            end %>
        ]);

    <% end %>

    rhoUtil.namespace(moduleNS, <%= $cur_module.name %>);

    <%  $cur_module.entities.each do |entity| %>
    // Add entity <%= entity.name %>
    rhoUtil.namespace(entity<%=entity.name%>NS, <%= entity.name %>);
    <% end %>

    <%  $cur_module.module_aliases.each do |module_alias|
        next if module_alias.deprecated
    %>
        rhoUtil.namespaceAlias(<%= namespace $cur_module %>, <%= $cur_module.parents.clone().join(".") %>, '<%= module_alias.new_name %>');
    <% end %>

})(Rho.jQuery, Rho, Rho.util);
