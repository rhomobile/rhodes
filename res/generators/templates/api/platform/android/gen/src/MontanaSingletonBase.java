package com.<%= $cur_module.parent.downcase %>.<%= $cur_module.name.downcase %>;

<% if $cur_module.is_template_default_instance %>
import com.rhomobile.rhodes.api.RhoApiDefaultId;
<% end %>

public abstract class <%= $cur_module.name %>SingletonBase
<% if $cur_module.is_template_default_instance %>
    extends RhoApiDefaultId<I<%= $cur_module.name %>, I<%= $cur_module.name %>Factory> <% end %> {

<% if $cur_module.is_template_default_instance %>
    public <%= $cur_module.name %>SingletonBase(<%= $cur_module.name %>Factory factory) {
        super(factory);
    } <% end %>
    
<% $cur_module.methods.each do |module_method|
    next unless module_method.access == ModuleMethod::ACCESS_STATIC

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
