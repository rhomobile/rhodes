#pragma once

#include "I<%= $cur_module.name %>.h"
#include "logging/RhoLog.h"
#include "common/StringConverter.h"
#include "common/ExtManager.h"

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>

using namespace rho::apiGenerator;
using namespace rho;

// hash keys used in properties and parameters <% 
if $cur_module.hash_key_names.size > 0 %>
namespace <%= $cur_module.name.downcase() %> {
<% $cur_module.hash_key_names.each do |key_names| 
%>
    static const char <%= key_names.const_tag %>[] = "<%= key_names.name %>"; 
<% end %> 
} <% end %>

class C<%= $cur_module.name %>FactoryBase : public CModuleFactoryBase<I<%= $cur_module.name %>, I<%= $cur_module.name %>Singleton, I<%= $cur_module.name %>Factory>
{
protected:
    static rho::common::CAutoPtr<C<%= $cur_module.name %>FactoryBase> m_pInstance;
    HashtablePtr<rho::String,I<%= $cur_module.name %>*> m_hashModules;

public:

    static void setInstance(C<%= $cur_module.name %>FactoryBase* pInstance){ m_pInstance = pInstance; }
    static C<%= $cur_module.name %>FactoryBase* getInstance(){ return m_pInstance; }

    static I<%= $cur_module.name %>Singleton* get<%= $cur_module.name %>SingletonS(){ return getInstance()->getModuleSingleton(); }

    virtual I<%= $cur_module.name %>* getModuleByID(const rho::String& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            I<%= $cur_module.name %>* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }

    virtual I<%= $cur_module.name %>* createModuleByID(const rho::String& strID){ return (I<%= $cur_module.name %>*)0; };
    virtual void deleteModuleByID(const rho::String& strID)
    {
        m_hashModules.remove(strID);
    }

};

class C<%= $cur_module.name %>SingletonBase : public CModuleSingletonBase< I<%= $cur_module.name %>Singleton >, public rho::common::IRhoExtension
{
protected:
    DEFINE_LOGCLASS;

<% if $cur_module.is_template_default_instance %>
    rho::String m_strDefaultID;
<% end %>

<% if $cur_module.is_template_propertybag  && $cur_module.properties_access == ModuleMethod::ACCESS_STATIC %>
    rho::Hashtable<rho::String, rho::String> m_hashProps;
    rho::Hashtable<rho::String, rho::apiGenerator::CMethodAccessor< I<%= $cur_module.name %>Singleton > *> m_mapPropAccessors;
<% end %>

public:
    virtual rho::LogCategory getModuleLogCategory(){ return getLogCategory(); }

    C<%= $cur_module.name %>SingletonBase();
    ~C<%= $cur_module.name %>SingletonBase();

<% if $cur_module.properties_access == ModuleMethod::ACCESS_STATIC%>

<% if $cur_module.is_template_propertybag %>
    virtual void getProperty( const rho::String& propertyName, CMethodResult& oResult);
    virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, CMethodResult& oResult);
    virtual void getAllProperties(CMethodResult& oResult);
    virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult);
    virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult);
    virtual void clearAllProperties(CMethodResult& oResult);
<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_INSTANCE
    next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_GETTER && module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_SETTER
    next if module_method.linked_property.use_property_bag_mode != ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG %>

    virtual void <%= module_method.native_name%>(<%= module_method.cached_data["cpp_params"] %>);<%
end; end; end %>

<% if $cur_module.is_template_default_instance %>
    virtual void setDefaultID(const rho::String& strDefaultID){ m_strDefaultID = strDefaultID; }
    virtual rho::String getDefaultID()
    { 
        if ( m_strDefaultID.length() == 0 )
            setDefaultID(getInitialDefaultID());
        return m_strDefaultID; 
    }
<% end %>
};

class C<%= $cur_module.name %>Base: public I<%= $cur_module.name %>
{
protected:
    DEFINE_LOGCLASS;

<% if $cur_module.is_template_propertybag && $cur_module.properties_access != ModuleMethod::ACCESS_STATIC %>
    rho::Hashtable<rho::String, rho::String> m_hashProps;
    rho::Hashtable<rho::String, rho::apiGenerator::CMethodAccessor< I<%= $cur_module.name %> > *> m_mapPropAccessors;
<% end %>
public:

<% if $cur_module.properties_access != ModuleMethod::ACCESS_STATIC %>
    C<%= $cur_module.name %>Base();
<% if $cur_module.is_template_propertybag %>
    virtual void getProperty( const rho::String& propertyName, CMethodResult& oResult);
    virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, CMethodResult& oResult);
    virtual void getAllProperties(CMethodResult& oResult);
    virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult);
    virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult);
    virtual void clearAllProperties(CMethodResult& oResult);
<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_INSTANCE
    next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_GETTER && module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_SETTER
    next if module_method.linked_property.use_property_bag_mode != ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG %>

    virtual void <%= module_method.native_name%>(<%= module_method.cached_data["cpp_params"] %>);<%
end; end; end %>

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
