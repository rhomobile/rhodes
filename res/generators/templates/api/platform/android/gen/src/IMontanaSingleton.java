package com.<%= $cur_module.parent.downcase %>.<%= $cur_module.name.downcase %>;

<%
  base = nil
  base = "IRhoApiDefaultId" if $cur_module.is_template_default_instance
%>

import com.rhomobile.rhodes.api.IMethodResult;
<% if base %>
import com.rhomobile.rhodes.api.<%= base %>;
<% end %>

public interface I<%= $cur_module.name %>Singleton
<% if base %>       extends <%= base %> <% end %> {

<% $cur_module.methods.each do |module_method|
    next unless module_method.access == ModuleMethod::ACCESS_STATIC
    params = ''
    module_method.params.each do |param|
        params += "#{api_generator_java_makeNativeType(param.type)} #{param.name}, "
    end

    params += 'IMethodResult result'

%>    void <%= module_method.native_name%>(<%= params%>); <% end %>

}
