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
//TODO: support property aliases
C<%= $cur_module.name %>Base::C<%= $cur_module.name %>Base()
{
<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_INSTANCE
    next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_GETTER
    if module_method.linked_property.use_property_bag_mode == ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS
%>
    m_mapPropAccessors[L"<%= module_method.linked_property.name %>"] = new rho::apiGenerator::CMethodAccessor< I<%= $cur_module.name %> >( &I<%= $cur_module.name %>::<%= module_method.native_name%> ); <%
    $cur_module.getPropAliases(module_method.linked_property.name).each do |alias_name| %>
    m_mapPropAccessors[L"<%=alias_name %>"] = new rho::apiGenerator::CMethodAccessor< I<%= $cur_module.name %> >( &I<%= $cur_module.name %>::<%= module_method.native_name%> ); <%
end; else %>
    m_mapPropAccessors[L"<%= module_method.linked_property.name %>"] = 0;<%
    $cur_module.getPropAliases(module_method.linked_property.name).each do |alias_name| %>
    m_mapPropAccessors[L"<%= alias_name %>"] = 0;<%
end; end; end%>

<% $cur_module.methods.each do |module_method|
    next if module_method.access != ModuleMethod::ACCESS_INSTANCE
    next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_SETTER
    next if module_method.linked_property.use_property_bag_mode != ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS
%>
    m_mapPropAccessors[L"<%= module_method.linked_property.name %>"]->addSetter( new rho::apiGenerator::CMethodAccessor< I<%= $cur_module.name %>>::CSetter< <%= api_generator_cpp_makeNativeTypeArg(module_method.linked_property.type) %>, <%= api_generator_cpp_makeNativeType(module_method.linked_property.type) %> >(&I<%= $cur_module.name %>::<%= module_method.native_name%>) );<%
end%>
}

void C<%= $cur_module.name %>Base::getProperty( const rho::StringW& propertyName, CMethodResult& oResult)
{
    CMethodAccessor< I<%= $cur_module.name %> >* pAccessor = m_mapPropAccessors[propertyName];
    if ( pAccessor )
        pAccessor->callGetter(this, oResult);
    else
    {
        <% if $cur_module.is_property_bag_limit_to_only_declared_properties %>
        if ( !m_mapPropAccessors.containsKey(propertyName) )
            oResult.setArgError(L"Get unknown property: " + propertyName);
        <% else %>
        oResult.set(m_hashProps[propertyName]);
        <% end %>
    }
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
    CMethodAccessor< I<%= $cur_module.name %> >* pAccessor = m_mapPropAccessors[propertyName];
    if (pAccessor && pAccessor->hasSetter())
        m_mapPropAccessors[propertyName]->callSetter(this, propertyValue, oResult);
    else
    {
        <% if $cur_module.is_property_bag_limit_to_only_declared_properties %>
        if ( !m_mapPropAccessors.containsKey(propertyName) )
            oResult.setArgError(L"Set unknown property: " + propertyName);
        <% else %>
        m_hashProps.put(propertyName, propertyValue);
        <% end %>
    }
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
}<% end ; else %>
C<%= $cur_module.name %>Base::C<%= $cur_module.name %>Base()
{
}<%
end; $cur_module.parents.each do |parent| %>
}<%
end %>
