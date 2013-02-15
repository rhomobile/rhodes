package <%= api_generator_java_makePackageName($cur_module) %>;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.RhoApiObject;
<%
if $cur_module.is_template_propertybag %>
import com.rhomobile.rhodes.api.RhoApiPropertyBag; <%
end %>

public class <%= $cur_module.name %>Base extends RhoApiObject{
<%
if $cur_module.is_template_propertybag %>
    private RhoApiPropertyBag mPropertyBag;
<%
if $cur_module.is_property_bag_limit_to_only_declared_properties %>
    static private List<String> sAllowedPropertyNames;
    static {
        sAllowedPropertyNames = new ArrayList<String>(); <%
$cur_module.properties.each do |property| %>
        sAllowedPropertyNames.add("<%= property.name %>");<%
end %>
    }<%
end
end %>
    public <%= $cur_module.name %>Base(String id) {
        super(id);
<%
if $cur_module.is_template_propertybag %>
        mPropertyBag = new RhoApiPropertyBag(<%
if $cur_module.is_property_bag_limit_to_only_declared_properties %>sAllowedPropertyNames<%
end %>);<%
end %>
    }
<% $cur_module.methods.each do |module_method|
    next if module_method.access == ModuleMethod::ACCESS_STATIC
    #next if module_method.generated_by_template == TEMPLATE_PROPERTY_BAG

    param_hash = {}
    module_method.params.each do |param|
        param_hash[param.name] = api_generator_java_makeNativeType(param.type)
    end %>
<%
if module_method.generated_by_template == TEMPLATE_PROPERTY_BAG %>
    public void <%= module_method.native_name %>(<%
param_hash.each do |name, type| %><%= type %> <%= name %>, <% end %>IMethodResult result) {
        mPropertyBag.<%= module_method.native_name %>(<%
param_hash.each do |name, type| %><%= name %>, <% end %>result);
    }<%
end %>

    public static class <%= module_method.native_name %>Task implements Runnable {
        private I<%= $cur_module.name %> mApiObject; <%
param_hash.each do |name, type| %>
        private <%= type %> <%= name %>;<%
end %>
        private IMethodResult mResult;

        public <%= module_method.native_name %>Task(I<%= $cur_module.name %> obj, <%
param_hash.each do |name, type| %>
                <%= type %> <%= name %>, <%
end %>
                IMethodResult result) {
            this.mApiObject = obj; <%
param_hash.each do |name, type| %>
            this.<%= name %> = <%= name %>;<%
end %>        
            this.mResult = result;
        }

        @Override
        public void run() {
            mApiObject.<%= module_method.native_name %>(<%
param_hash.each do |name, type| %>
                <%= name %>, <% end %>mResult);
        }
    }
<% end %>
}
