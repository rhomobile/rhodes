package <%= api_generator_java_makePackageName($cur_module) %>;

import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;

public class <%= $cur_module.name %>Base {

<% $cur_module.methods.each do |module_method|
    next if module_method.access == ModuleMethod::ACCESS_STATIC
    #next if module_method.generated_by_template == TEMPLATE_PROPERTY_BAG

    param_hash = {}
    module_method.params.each do |param|
        param_hash[param.name] = api_generator_java_makeNativeType(param.type)
    end %>

    public static class <%= module_method.native_name %>Task implements Runnable {
        private I<%= $cur_module.name %> mApiObject; <%
param_hash.each do |name, type| %>
        private <%= type %> <%= name %>; <% end %>
        private IMethodResult mResult;

        public <%= module_method.native_name %>Task(I<%= $cur_module.name %> obj, <%
param_hash.each do |name, type| %>
                <%= type %> <%= name %>, <% end %>        
                IMethodResult result) {
            this.mApiObject = obj; <%
param_hash.each do |name, type| %>
            this.<%= name %> = <%= name %>;<% end %>        
            this.mResult = result;
        }

        @Override
        public void run() {
            mApiObject.<%= module_method.native_name %>(<%
param_hash.each do |name, type| %>
                <%= name %>, <% end %> mResult);
        }
    }
<% end %>
}
