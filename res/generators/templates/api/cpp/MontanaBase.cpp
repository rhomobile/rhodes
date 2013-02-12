#include "<%= $cur_module.name %>Base.h"

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>

rho::common::CAutoPtr< C<%= $cur_module.name %>FactoryBase> C<%= $cur_module.name %>FactoryBase::m_pInstance;

///////////////////////////////////////
//string constants definiton <%
$cur_module.constants.each do |module_constant|
    if module_constant.type == MethodParam::TYPE_STRING %>
const wchar_t I<%= $cur_module.name %>::<%= module_constant.name %>[] = L"<%= module_constant.value %>"; <%
end; end %>

////////////////////////////////////////////////
<% if $cur_module.is_template_propertybag %>
void C<%= $cur_module.name %>Base::getProperty( const rho::StringW& propertyName, CMethodResult& oResult)
{
<% if $cur_module.use_property_bag_mode == ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS%>
    if (false){}
<%   $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_INSTANCE
    next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_GETTER%>
    else if (_wcsicmp(L"<%= module_method.linked_property.native_name %>",propertyName.c_str()) == 0){
<%if module_method.linked_property.use_property_bag_mode == ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS %>
        <%= module_method.native_name%>( oResult );<%
else%>
        oResult.set(m_hashProps[propertyName]);<%
end;%>
    }<%
end; else%>
    oResult.set(m_hashProps[propertyName]);<%
end%>
}

void C<%= $cur_module.name %>Base::getProperties( const rho::Vector<::rho::StringW>& arrayofNames, CMethodResult& oResult)
{
    rho::Hashtable<::rho::StringW, rho::StringW> res;
    oResult.setCollectionMode(true);
    for ( int i = 0; i < (int)arrayofNames.size(); i++ )
    {
        getProperty(arrayofNames[i], oResult);

        if ( oResult.isError() )
            break;

        res[arrayofNames[i]] = common::convertToStringW(oResult.toString());
    }

    oResult.setCollectionMode(false);
    if ( oResult.isError() )
        oResult.callCallback();
    else
        oResult.set(res);
}

void C<%= $cur_module.name %>Base::getAllProperties(CMethodResult& oResult)
{
    oResult.set(m_hashProps);
}

void C<%= $cur_module.name %>Base::setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, CMethodResult& oResult)
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
        <%= module_method.native_name%>( arg, oResult);<%
else%>
        m_hashProps.put(propertyName, propertyValue);<%
end;%>
    }<%
end; else%>
    m_hashProps.put(propertyName, propertyValue);<%
end%>
}

void C<%= $cur_module.name %>Base::setProperties( const rho::Hashtable<::rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult)
{
    for ( rho::Hashtable<rho::StringW, rho::StringW>::const_iterator it = propertyMap.begin();  it != propertyMap.end(); ++it )
    {
        setProperty( it->first, it->second, oResult );
        if ( oResult.isError() )
            break;
    }
}

void C<%= $cur_module.name %>Base::clearAllProperties(CMethodResult& oResult)
{
    m_hashProps.clear();
}

<% $cur_module.methods.each do |module_method|
next if module_method.access != ModuleMethod::ACCESS_INSTANCE
next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_GETTER && module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_SETTER
next if module_method.linked_property.use_property_bag_mode != ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG %>

void C<%= $cur_module.name %>Base::<%= module_method.native_name%>(<%= module_method.cached_data["cpp_params"] %>)
{ <% if module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_GETTER %>
    getProperty( L"<%= module_method.linked_property.native_name %>", oResult); <%
elsif module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_SETTER %>
    setProperty( L"<%= module_method.linked_property.native_name %>", rho::common::convertToStringW(value), oResult );<%
end %>
}<% end ; end
$cur_module.parents.each do |parent| %>
}<%
end %>
