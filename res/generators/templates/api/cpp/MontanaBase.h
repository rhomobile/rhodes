#include "I<%= $cur_module.name %>.h"
#include "common/StringConverter.h"

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>

using namespace rho::apiGenerator;
class C<%= $cur_module.name %>Base: public I<%= $cur_module.name %>
{
protected:
<% if $cur_module.is_template_propertybag %>
    rho::Hashtable<::rho::StringW, ::rho::StringW> m_hashProps;
<% end %>
public:
    C<%= $cur_module.name %>Base(){}

<% if $cur_module.is_template_propertybag %>
    virtual void getProperty( const rho::StringW& propertyName, CMethodResult& oResult)
    {
        oResult.set(m_hashProps[propertyName]);
    }
    virtual void getProperties( const rho::Vector<::rho::StringW>& arrayofNames, CMethodResult& oResult)
    {
        rho::Hashtable<::rho::StringW, rho::StringW> res;
        oResult.setCollectionMode(true);
        for ( int i = 0; i < (int)arrayofNames.size(); i++ )
        {
            getProperty(arrayofNames[i], oResult);

            if ( oResult.isError() )
                break;

            res[arrayofNames[i]] = oResult.toStringW();
        }

        oResult.setCollectionMode(false);
        if ( oResult.isError() )
            oResult.callCallback();
        else
            oResult.set(res);
    }
    virtual void getAllProperties(CMethodResult& oResult)
    {
        oResult.set(m_hashProps);
    }
    virtual void setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, CMethodResult& oResult)
    {
<% if $cur_module.use_property_bag_mode == ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS%>
        if (false){}
<%   $cur_module.methods.each do |module_method|
        next if module_method.access != ModuleMethod::ACCESS_INSTANCE
        next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_SETTER%>

        else if (_wcsicmp(L"<%= module_method.linked_property.native_name %>",propertyName.c_str()) == 0){
<%if module_method.linked_property.use_property_bag_mode == ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS %>
            <%= api_generator_cpp_makeNativeType(module_method.linked_property.type) %> arg;
            rho::common::convertFromStringW(propertyValue.c_str(), arg);
            <%= module_method.native_name%>( arg, oResult);
<%else%>
            m_hashProps.put(propertyName, propertyValue);<%
end;%>
        }<%
end; else%>
        m_hashProps.put(propertyName, propertyValue);<%
end%>
    }
    virtual void setProperties( const rho::Hashtable<::rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult)
    {
        for ( rho::Hashtable<rho::StringW, rho::StringW>::const_iterator it = propertyMap.begin();  it != propertyMap.end(); ++it )
        {
            setProperty( it->first, it->second, oResult );
            if ( oResult.isError() )
                break;
        }
    }
    virtual void clearAllProperties(CMethodResult& oResult)
    {
        m_hashProps.clear();
    }

<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_INSTANCE
    next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_GETTER && module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_SETTER
    next if module_method.linked_property.use_property_bag_mode != ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG %>

    virtual void <%= module_method.native_name%>(<%= module_method.cached_data["cpp_params"] %>)
    { <% if module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_GETTER %>
        getProperty( L"<%= module_method.linked_property.native_name %>", oResult); <%
elsif module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_SETTER %>
        setProperty( L"<%= module_method.linked_property.native_name %>", rho::common::convertToStringW(value), oResult );<%
end %>
    }<% end %>

<% end %>
};

class C<%= $cur_module.name %>FactoryBase : public CModuleFactoryBase<I<%= $cur_module.name %>, I<%= $cur_module.name %>Singleton, I<%= $cur_module.name %>Factory>
{
protected:
    static rho::common::CAutoPtr<C<%= $cur_module.name %>FactoryBase> m_pInstance;

public:

    static void setInstance(C<%= $cur_module.name %>FactoryBase* pInstance){ m_pInstance = pInstance; }
    static C<%= $cur_module.name %>FactoryBase* getInstance(){ return m_pInstance; }

    static I<%= $cur_module.name %>Singleton* get<%= $cur_module.name %>SingletonS(){ return getInstance()->getModuleSingleton(); }
};

extern "C" void Init_<%= $cur_module.name %>_API();

<% $cur_module.parents.each do |parent| %>
}<%
end %>
