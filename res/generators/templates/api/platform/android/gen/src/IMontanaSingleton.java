package <%= api_generator_java_makePackageName($cur_module) %>;

<%
  base = nil
  base = "IRhoApiDefaultId" if $cur_module.is_template_default_instance
%>

import java.util.Map;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;
<% if base %>
import com.rhomobile.rhodes.api.<%= base %>;
<% end %>

public interface I<%= $cur_module.name %>Singleton
<% if base %>       extends <%= base %> <% end %> {

<% $cur_module.constants.each do |module_constant|
        # next if module_constant.is_deprecated
  next if module_constant.name.nil? || module_constant.name.empty?

  if module_constant.type == MethodParam::TYPE_STRING %>
    static final String <%= module_constant.name %> = "<%= module_constant.value %>";<%
  else %>
    static final <%= api_generator_java_makeSimpleNativeType(module_constant.type) %> <%= module_constant.name %> = <%= module_constant.value %>;<%
  end
end %>

  // hash keys used in properties and parameters
<% if $cur_module.hash_key_names.size > 0 
    $cur_module.hash_key_names.each do |key_names| %>
    static final String <%= key_names.const_tag %> = "<%= key_names.name %>"; <% 
    end 
end %>

<% $cur_module.methods.each do |module_method|
    next unless module_method.access == ModuleMethod::ACCESS_STATIC
    
    params = ''
    module_method.params.each do |param|
        params += "#{api_generator_java_makeNativeType(param)} #{param.name}, "
    end

    params += 'IMethodResult result'

%>
    void <%= module_method.native_name%>(<%= params%>); <% end %>

}
