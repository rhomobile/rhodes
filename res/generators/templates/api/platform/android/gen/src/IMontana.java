package <%= api_generator_java_makePackageName($cur_module) %>;

<%
  base = "IRhoApiObject"
  base = "IRhoApiPropertyBag" if $cur_module.is_template_propertybag
%>

import java.util.Map;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.IRhoApiObject;
<%
if $cur_module.is_template_propertybag %>
import com.rhomobile.rhodes.api.IRhoApiPropertyBag;<%
end %>

public interface I<%= $cur_module.name %> extends IRhoApiObject<% if $cur_module.is_template_propertybag %>, IRhoApiPropertyBag<% end %> {

<% $cur_module.methods.each do |module_method|
    next if module_method.access == ModuleMethod::ACCESS_STATIC
    next if module_method.generated_by_template == TEMPLATE_PROPERTY_BAG

    params = ''
    module_method.params.each do |param|
        params += "#{api_generator_java_makeNativeType(param)} #{param.name}, "
    end

    params += 'IMethodResult result'

%>    void <%= module_method.native_name%>(<%= params%>);
<% end %>};
