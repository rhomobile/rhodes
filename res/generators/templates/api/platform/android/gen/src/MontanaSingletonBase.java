package <%= api_generator_java_makePackageName($cur_module) %>;

import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;
<% if $cur_module.is_template_default_instance %>
import com.rhomobile.rhodes.api.RhoApiDefaultId;
<% end %>
public abstract class <%= $cur_module.name %>SingletonBase <%
if $cur_module.is_template_default_instance %>
    extends RhoApiDefaultId<I<%= $cur_module.name %>, I<%= $cur_module.name %>Factory> <% end %> {
<% if $cur_module.is_template_default_instance %>
    public <%= $cur_module.name %>SingletonBase(I<%= $cur_module.name %>Factory factory) {
        super(factory);
    } <% end %>
<% $cur_module.methods.each do |module_method|
    next unless module_method.access == ModuleMethod::ACCESS_STATIC

    param_hash = {}
    module_method.params.each do |param|
        param_hash[param.name] = api_generator_java_makeNativeType(param)
    end %>
    public static class <%= module_method.native_name %>Task implements Runnable {
        private I<%= $cur_module.name %>Singleton mApiSingleton; <%
module_method.params.each do |param| %>
        private <%= param_hash[param.name] %> <%= param.name %>; <% end %>
        private IMethodResult mResult;

        public <%= module_method.native_name %>Task(I<%= $cur_module.name %>Singleton obj, <%
module_method.params.each do |param| %>
                <%= param_hash[param.name] %> <%= param.name %>, <% end %>        
                IMethodResult result) {
            this.mApiSingleton = obj; <%
module_method.params.each do |param| %>
            this.<%= param.name %> = <%= param.name %>;<% end %>        
            this.mResult = result;
        }

        @Override
        public void run() {
            try {
                mApiSingleton.<%= module_method.native_name %>(<%
module_method.params.each do |param| %>
                    <%= param.name %>, <% end %> mResult);
            } catch (Throwable ex) {
                mResult.set(ex);
            }
        }
    }
<% end %>
    
}
