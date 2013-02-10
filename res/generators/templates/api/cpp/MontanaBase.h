#include "I<%= $cur_module.name %>.h"
#include "common/StringConverter.h"

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>

using namespace rho::apiGenerator;

class C<%= $cur_module.name %>FactoryBase : public CModuleFactoryBase<I<%= $cur_module.name %>, I<%= $cur_module.name %>Singleton, I<%= $cur_module.name %>Factory>
{
protected:
    static rho::common::CAutoPtr<C<%= $cur_module.name %>FactoryBase> m_pInstance;

public:

    static void setInstance(C<%= $cur_module.name %>FactoryBase* pInstance){ m_pInstance = pInstance; }
    static C<%= $cur_module.name %>FactoryBase* getInstance(){ return m_pInstance; }

    static I<%= $cur_module.name %>Singleton* get<%= $cur_module.name %>SingletonS(){ return getInstance()->getModuleSingleton(); }
};


class C<%= $cur_module.name %>Base: public I<%= $cur_module.name %>
{
protected:
<% if $cur_module.is_template_propertybag %>
    rho::Hashtable<::rho::StringW, ::rho::StringW> m_hashProps;
<% end %>
public:
    C<%= $cur_module.name %>Base(){}

<% if $cur_module.is_template_propertybag %>
    virtual void getProperty( const rho::StringW& propertyName, CMethodResult& oResult);
    virtual void getProperties( const rho::Vector<::rho::StringW>& arrayofNames, CMethodResult& oResult);
    virtual void getAllProperties(CMethodResult& oResult);
    virtual void setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, CMethodResult& oResult);
    virtual void setProperties( const rho::Hashtable<::rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult);
    virtual void clearAllProperties(CMethodResult& oResult);
<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_INSTANCE
    next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_GETTER && module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_SETTER
    next if module_method.linked_property.use_property_bag_mode != ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG %>

    virtual void <%= module_method.native_name%>(<%= module_method.cached_data["cpp_params"] %>);<%
end; end %>
<% if $cur_module.is_template_default_instance%>
    static C<%= $cur_module.name %>Base* getInstance(){ return static_cast< C<%= $cur_module.name %>Base* >(C<%= $cur_module.name %>FactoryBase::getInstance()->getModuleByID(C<%= $cur_module.name %>FactoryBase::get<%= $cur_module.name %>SingletonS()->getDefaultID())); }
<% if false %>
    <%$cur_module.methods.each do |module_method|
        next if module_method.access != ModuleMethod::ACCESS_INSTANCE 
        param_list = ""
        module_method.params.each do |param|
            param_list += "#{param.name}, "
        end
        param_list += 'oResult' %>
    static void <%= module_method.native_name%>(<%= module_method.cached_data["cpp_params"] %>){getInstance()-><%= module_method.native_name%>(<%= param_list%>);}<%
end; end; end %> 

};

extern "C" void Init_<%= $cur_module.name %>_API();

<% $cur_module.parents.each do |parent| %>
}<%
end %>
