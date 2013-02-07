package com.<%= $cur_module.parent.downcase %>.<%= $cur_module.name.downcase %>;

import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;

public class <%= $cur_module.name %>Base {

<% if $cur_module.is_template_propertybag

%>    public static class GetAllPropertiesTask implements Runnable {
        private I<%= $cur_module.name %> mApiObject;
        private IMethodResult mResult;

        public GetPropertyTask(I<%= $cur_module.name %> obj, IMethodResult result) {
            mApiObject = obj;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getProperty(mResult);
        }
    }

    public static class GetPropertyTask implements Runnable {
        private I<%= $cur_module.name %> mApiObject;
        private String mName;
        private IMethodResult mResult;

        public GetPropertyTask(I<%= $cur_module.name %> obj, String name, IMethodResult result) {
            mApiObject = obj;
            mName = name;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getProperty(mName, mResult);
        }
    }

    public static class GetPropertiesTask implements Runnable {
        private I<%= $cur_module.name %> mApiObject;
        private List<String> mNames;
        private IMethodResult mResult;

        public GetPropertiesTask(I<%= $cur_module.name %> obj, List<String> names, IMethodResult result) {
            mApiObject = obj;
            mNames = names;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getProperties(mNames, mResult);
        }
    }
    
    public static class SetPropertyTask implements Runnable {
        private I<%= $cur_module.name %> mApiObject;
        private String mName;
        private String mValue;
        private IMethodResult mResult;
        
        public SetPropertyTask(I<%= $cur_module.name %> obj, String name, String val, IMethodResult result) {
            mApiObject = obj;
            mName = name;
            mVAlue = val;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.setProperty(mName, mValue);
        }
    }

    public static class SetPropertiesTask implements Runnable {
        private I<%= $cur_module.name %> mApiObject;
        private Map<String, String> mProps;
        private IMethodResult mResult;
        
        public SetPropertiesTask(I<%= $cur_module.name %> obj, Map<String, String> props, IMethodResult result) {
            mApiObject = obj;
            mProps = props;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.setProperties(mProps);
        }
    }

    public static class ClearAllPropertiesTask implements Runnable {
        private I<%= $cur_module.name %> mApiObject;
        private IMethodResult mResult;

        public ClearAllPropertiesTask(I<%= $cur_module.name %> obj, IMethodResult result) {
            mApiObject = obj;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.clearAllProperties(mResult);
        }
    }
<% end %>

<% $cur_module.methods.each do |module_method|
    next if module_method.access == ModuleMethod::ACCESS_STATIC
    next if module_method.generated_by_template == TEMPLATE_PROPERTY_BAG

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
