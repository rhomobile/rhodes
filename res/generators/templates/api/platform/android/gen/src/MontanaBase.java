package <%= api_generator_java_makePackageName($cur_module) %>;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.RhoApiObject;
<%
if $cur_module.is_template_propertybag %>
import com.rhomobile.rhodes.api.RhoApiPropertyBag; <%

custom_props = {}
$cur_module.properties.each do |property|
    custom_props[property.name] = property.type if property.use_property_bag_mode == ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS
end
end %>

public class <%= $cur_module.name %>Base extends RhoApiObject {
<%
if $cur_module.is_template_propertybag %>
    private RhoApiPropertyBag mPropertyBag;
    public Map<String, String> getPropertiesMap() {
        return mPropertyBag.getPropertiesMap();
    }<%
unless custom_props.empty? %>
    private static Map<String, Class<?> > sCustomAccessNames;
    static {
        sCustomAccessNames = new HashMap<String, Class<?> >();
<%
custom_props.each do |name, type| %>
        sCustomAccessNames.put("<%= name %>", <%= api_generator_java_makeSimpleNativeType(type) %>.class);
<%
end %>
    }<%
end
if $cur_module.is_property_bag_limit_to_only_declared_properties %>
    static private List<String> sAllowedPropertyNames;
    static {
        sAllowedPropertyNames = new ArrayList<String>(); <%
$cur_module.properties.each do |property| %>
        sAllowedPropertyNames.add("<%= property.name %>");<%
end 
if $cur_module.is_template_propertybag %>
        sAllowedPropertyNames.add("id");<%
end %>
    }<%
end
end %>
    public <%= $cur_module.name %>Base(String id) {
        super(id);
<%
if $cur_module.is_template_propertybag %>
        mPropertyBag = new RhoApiPropertyBag(<%
if $cur_module.is_property_bag_limit_to_only_declared_properties %>sAllowedPropertyNames, <%
end %>id);<%
end %>
    }
<% $cur_module.methods.each do |method|
    next if method.access == ModuleMethod::ACCESS_STATIC
    #next if method.generated_by_template == TEMPLATE_PROPERTY_BAG

    param_hash = {}
    method.params.each do |param|
        param_hash[param.name] = api_generator_java_makeNativeType(param)
    end

    if $cur_module.is_template_propertybag or (method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_GETTER and method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_SETTER)

        if method.generated_by_template == TEMPLATE_PROPERTY_BAG %>
    public void <%= method.native_name %>(<%
            param_hash.each do |name, type| %><%= type %> <%= name %>, <% end %>IMethodResult result) { <%
                unless custom_props.empty? %>
        mPropertyBag.<%= method.native_name %>(<% param_hash.each do |name, type| %><%= name %>, <% end %>sCustomAccessNames, this, result);<%
                else %>
        mPropertyBag.<%= method.native_name %>(<% param_hash.each do |name, type| %><%= name %>, <% end %>result);<%
                end %>
    }<%
            end

            if method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_GETTER or method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_SETTER
                if method.linked_property.use_property_bag_mode == ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG %>
    public void <%= method.native_name %>(<% param_hash.each do |name, type| %><%= type %> <%= name %>, <% end %>IMethodResult result) {
                    <%
                    if method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_GETTER
                        if method.linked_property.type == RhogenCore::TYPE_BOOL %>
        result.forceBooleanType();<%
                        elsif method.linked_property.type == RhogenCore::TYPE_INT %>
        result.forceIntegerType();<%
                        elsif method.linked_property.type == RhogenCore::TYPE_DOUBLE %>
        result.forceDoubleType();<%
                        end %> 
        getProperty("<%= method.linked_property.name %>", result);<%
                    else %>
        setProperty("<%= method.linked_property.name %>", String.valueOf(<%= method.params.first.name %>), result);<%
                end %>
    }<%
            end
        end 
    end %>

    public static class <%= method.native_name %>Task implements Runnable {
        private I<%= $cur_module.name %> mApiObject; <%
method.params.each do |param| %>
        private <%= param_hash[param.name] %> <%= param.name %>;<%
end %>
        private IMethodResult mResult;

        public <%= method.native_name %>Task(I<%= $cur_module.name %> obj, <%
method.params.each do |param| %>
                <%= param_hash[param.name] %> <%= param.name %>, <%
end %>
                IMethodResult result) {
            this.mApiObject = obj; <%
method.params.each do |param| %>
            this.<%= param.name %> = <%= param.name %>;<%
end %>
            this.mResult = result;
        }

        @Override
        public void run() {
            try {
                mApiObject.<%= method.native_name %>(<%
method.params.each do |param| %>
                    <%= param.name %>, <% end %>mResult);
            } catch (Throwable ex) {
                mResult.set(ex);
            }
        }
    }
<% end %>
}
