<%
    def js_compatible_name(name)
        # name.end_with?('=') ? 'set' + name[0].upcase + name[1..name.length-2] : name
        name
    end

    def capitalize_safely(str)
        str[0..0].upcase + str[1..-1]
    end

    def namespace(a_module)
        a_module.parents.clone().push(a_module.name).join(".")
    end
%>
(function ($, rho, rhoUtil) {
    'use strict';

    var moduleNS = '<%= namespace $cur_module %>';
    var apiReq = rhoUtil.apiReqFor(moduleNS);

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
                this.<%= js_compatible_name module_method.name %>.apply(this, arguments);
            <% end %>
        }
    };

    // === <%= $cur_module.name %> instance properties ===

    rhoUtil.createPropsProxy(<%= $cur_module.name %>.prototype, {
    <% first_prop = true
       $cur_module.properties.each do |module_property|
         next if module_property.access == ModuleMethod::ACCESS_STATIC
         next if module_property.is_deprecated
         next if module_property.readonly && module_property.writeonly
    %>  <%= ',' if !first_prop %> '<%= module_property.name %>': '<%= 'r' if !module_property.writeonly %><%= 'w' if !module_property.readonly %>'
    <% first_prop = false
       end %>}, apiReq);

    // === <%= $cur_module.name %> instance methods ===

    <% $cur_module.methods.each do |module_method|
        next if module_method.access == ModuleMethod::ACCESS_STATIC
        next if module_method.is_accessor
        next if module_method.is_deprecated

        params = module_method.params.map do |param|
            "/* #{api_generator_cpp_makeNativeTypeArg(param.type)} */ #{param.name}"
        end.push("/* optional function */ oResult").join(', ')
    %>
        <%= $cur_module.name %>.prototype['<%= js_compatible_name module_method.name %>'] = function(<%= params %>) {
            return apiReq({
                instanceId: this.getId(),
                args: arguments,
                method: '<%= module_method.name %>',
                <%= "persistentCallbackIndex: #{module_method.params.size}," if module_method.has_callback != ModuleMethod::CALLBACK_NONE %>
                valueCallbackIndex: <%= module_method.params.size + (module_method.has_callback == ModuleMethod::CALLBACK_NONE ? 0 : 2) %>
            });
        };

    <% end %>

    // === <%= $cur_module.name %> constants ===

    <% $cur_module.constants.each do |module_constant|
        # next if module_constant.is_deprecated

        if module_constant.type == MethodParam::TYPE_STRING %>
            <%= $cur_module.name %>.<%= module_constant.name %> = '<%= module_constant.value %>';
        <% else %>
            <%= $cur_module.name %>.<%= module_constant.name %> = <%= module_constant.value %>;
        <% end %>
    <% end %>

    // === <%= $cur_module.name %> static properties ===

    rhoUtil.createPropsProxy(<%= $cur_module.name %>, {
    <% first_prop = true
       $cur_module.properties.each do |module_property|
        next if module_property.access != ModuleMethod::ACCESS_STATIC
        next if module_property.is_deprecated
        next if module_property.readonly && module_property.writeonly
    %>  <%= ',' if !first_prop %> '<%= module_property.name %>': '<%= 'r' if !module_property.writeonly %><%= 'w' if !module_property.readonly %>'
    <% first_prop = false
       end %>}, apiReq);

    // === <%= $cur_module.name %> static methods ===

    <% $cur_module.methods.each do |module_method|
        next if module_method.access != ModuleMethod::ACCESS_STATIC
        next if module_method.is_accessor
        next if module_method.is_deprecated

        params = module_method.params.map do |param|
            "/* #{api_generator_cpp_makeNativeTypeArg(param.type)} */ #{param.name}"
        end.push("/* optional function */ oResult").join(', ')
    %>
        <%= $cur_module.name %>['<%= module_method.name %>'] = function(<%= params %>) {
            return apiReq({
                instanceId: '0',
                args: arguments,
                method: '<%= module_method.name %>',
                <%= "persistentCallbackIndex: #{module_method.params.size}," if module_method.has_callback != ModuleMethod::CALLBACK_NONE %>
                valueCallbackIndex: <%= module_method.params.size + (module_method.has_callback == ModuleMethod::CALLBACK_NONE ? 0 : 2) %>
            });
        };
    <% end %>

    // === <%= $cur_module.name %> default instance support ===
    <% if $cur_module.is_template_default_instance %>
        rhoUtil.createPropsProxy(<%= $cur_module.name %>, {
            'default:getDefault': 'r'
          , 'defaultID:getDefaultID::setDefaultID': 'rw'
        }, apiReq);
    <% end %>

    rhoUtil.namespace(moduleNS, <%= $cur_module.name %>);

})(jQuery, Rho, Rho.util);
