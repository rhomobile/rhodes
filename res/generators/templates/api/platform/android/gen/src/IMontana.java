package com.<%= $cur_module.parent.downcase %>.<%= $cur_module.name.downcase %>;

<%
  base = "IRhoApiObject"
  base = "IRhoApiPropertyBag" if $cur_module.is_template_propertybag
%>

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.<%= base %>;

public interface I<%= $cur_module.name %> extends <%= base %> {

<% $cur_module.methods.each do |module_method|
    next if module_method.access == ModuleMethod::ACCESS_STATIC
    next if module_method.generated_by_template == TEMPLATE_PROPERTY_BAG

    params = ''
    module_method.params.each do |param|
        params += "#{api_generator_java_makeNativeType(param.type)} #{param.name}, "
    end

    params += 'IMethodResult result'

%>    void <%= module_method.native_name%>(<%= params%>);
<% end %>};
