#include "<%= $cur_module.name %>Base.h"
#include "common/RhodesApp.h"

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>

IMPLEMENT_LOGCLASS(C<%= $cur_module.name %>SingletonBase, "<%= $cur_module.name %>");
IMPLEMENT_LOGCLASS(C<%= $cur_module.name %>Base, "<%= $cur_module.name %>");

rho::common::CAutoPtr< C<%= $cur_module.name %>FactoryBase> C<%= $cur_module.name %>FactoryBase::m_pInstance;

<% if $cur_module.properties_access == ModuleMethod::ACCESS_STATIC 
     propBaseClass = "C#{$cur_module.name}SingletonBase"
     propBaseI = "I#{$cur_module.name}Singleton"
   else
     propBaseClass = "C#{$cur_module.name}Base"
     propBaseI = "I#{$cur_module.name}"
   end
%>

///////////////////////////////////////
//string constants definiton <%
$cur_module.constants.each do |module_constant|
    if module_constant.type == RhogenCore::TYPE_STRING && module_constant.name && module_constant.name.length() > 0%>
const char <%= propBaseI %>::<%= module_constant.name %>[] = "<%= module_constant.value %>"; <%
end; end %>

////////////////////////////////////////////////
<% if $cur_module.is_template_propertybag %>
<%= propBaseClass %>::<%= propBaseClass %>()
{
<% if propBaseClass == "C#{$cur_module.name}SingletonBase" %>
    RHODESAPP().getExtManager().registerExtension( "<%= $cur_module.name %>", this ); <%   
end
 $cur_module.methods.each do |module_method|
    next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_GETTER
    if module_method.linked_property.use_property_bag_mode == ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS
%>
    m_mapPropAccessors["<%= module_method.linked_property.name %>"] = new rho::apiGenerator::CMethodAccessor< <%= propBaseI %> >( &<%= propBaseI %>::<%= module_method.native_name%> ); <%
    $cur_module.getPropAliases(module_method.linked_property.name).each do |alias_name| %>
    m_mapPropAccessors["<%=alias_name %>"] = new rho::apiGenerator::CMethodAccessor< <%= propBaseI %> >( &<%= propBaseI %>::<%= module_method.native_name%> ); <%
end; else %>
    m_mapPropAccessors["<%= module_method.linked_property.name %>"] = 0;<%
    $cur_module.getPropAliases(module_method.linked_property.name).each do |alias_name| %>
    m_mapPropAccessors["<%= alias_name %>"] = 0;<%
end; end; end%>

<% $cur_module.methods.each do |module_method|
    next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_SETTER
    next if module_method.linked_property.use_property_bag_mode != ModuleProperty::USE_PROPERTY_BAG_MODE_PROPERTY_BAG_VIA_ACCESSORS
%>
    m_mapPropAccessors["<%= module_method.linked_property.name %>"]->addSetter( new rho::apiGenerator::CMethodAccessor< <%= propBaseI %>>::CSetter< <%= CppGen::native_type_arg(module_method.linked_property) %>, <%= CppGen::native_type(module_method.linked_property) %> >(&<%= propBaseI %>::<%= module_method.native_name%>) );<%
end%>
}

void <%= propBaseClass %>::getProperty( const rho::String& propertyName, CMethodResult& oResult)
{
<% if $cur_module.is_property_bag_limit_to_only_declared_properties %>
    if ( !m_mapPropAccessors.containsKey(propertyName) )
    {
        oResult.setArgError("Get unknown property: " + propertyName);
        return;
    }
<% end %>
    oResult.resetToEmptyString();
    CMethodAccessor< <%= propBaseI %> >* pAccessor = m_mapPropAccessors[propertyName];
    if ( pAccessor )
        pAccessor->callGetter(this, oResult);
    else
    {
<% if $cur_module.is_template_propertybag %>
        oResult.set(m_hashProps[propertyName]);
<% else%>
        oResult.setArgError("Get unknown property: " + propertyName);        
<% end %>
    }
}

void <%= propBaseClass %>::getProperties( const rho::Vector<rho::String>& arrayofNames, CMethodResult& oResult)
{
    rho::Hashtable<rho::String, rho::String> res;
    oResult.setCollectionMode(true);
    for ( int i = 0; i < (int)arrayofNames.size(); i++ )
    {
        getProperty(arrayofNames[i], oResult);

        if ( !oResult.isError() )
        {
            res[arrayofNames[i]] = oResult.toString();
        }
    }
    oResult.setCollectionMode(false);

    oResult.set(res);
}

void <%= propBaseClass %>::getAllProperties(CMethodResult& oResult)
{
    rho::Hashtable<rho::String, rho::String> res;
    oResult.setCollectionMode(true);

    <% if $cur_module.is_template_propertybag && !$cur_module.is_property_bag_limit_to_only_declared_properties%>
    // user defined properties
    for ( rho::Hashtable<rho::String, rho::String>::const_iterator it = m_hashProps.begin();  it != m_hashProps.end(); ++it )
    {
        res[it->first] = it->second;
    }
    <% end %>

    // existing properties
    for ( rho::Hashtable<rho::String, rho::apiGenerator::CMethodAccessor< <%= propBaseI %> > *>::const_iterator it = m_mapPropAccessors.begin();  it != m_mapPropAccessors.end(); ++it )
    {
        getProperty(it->first, oResult);
        
        if ( oResult.isError() )
            break;
        
        res[it->first] = oResult.toString();
    }
    
    oResult.setCollectionMode(false);
    if ( oResult.isError() )
        oResult.callCallback();
    else
        oResult.set(res);
}

void <%= propBaseClass %>::setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult)
{
    CMethodAccessor< <%= propBaseI %> >* pAccessor = m_mapPropAccessors[propertyName];
    if (pAccessor && pAccessor->hasSetter())
        m_mapPropAccessors[propertyName]->callSetter(this, propertyValue, oResult);
    else
    {
        <% if $cur_module.is_property_bag_limit_to_only_declared_properties %>
        if ( !m_mapPropAccessors.containsKey(propertyName) )
        {
            oResult.setArgError("Set unknown property: " + propertyName);
            return;
        }
        <% end %>

        m_hashProps.put(propertyName, propertyValue);
    }
}

void <%= propBaseClass %>::setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
{
    for ( rho::Hashtable<rho::String, rho::String>::const_iterator it = propertyMap.begin();  it != propertyMap.end(); ++it )
    {
        setProperty( it->first, it->second, oResult );
        if ( oResult.isError() )
            break;
    }
}

void <%= propBaseClass %>::clearAllProperties(CMethodResult& oResult)
{
    m_hashProps.clear();
    // ToDo: set default values to existing properties 
}

<% $cur_module.methods.each do |module_method|
next if module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_GETTER && module_method.special_behaviour != ModuleMethod::SPECIAL_BEHAVIOUR_SETTER
next if module_method.linked_property.use_property_bag_mode != ModuleProperty::USE_PROPERTY_BAG_MODE_ACCESSORS_VIA_PROPERTY_BAG %>

void <%= propBaseClass %>::<%= module_method.native_name%>(<%= module_method.cached_data["cpp_params"] %>)
{ <% if module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_GETTER %>
    getProperty( "<%= module_method.linked_property.native_name %>", oResult); <%
elsif module_method.special_behaviour == ModuleMethod::SPECIAL_BEHAVIOUR_SETTER %>
    setProperty( "<%= module_method.linked_property.native_name %>", rho::common::convertToStringA(<%= module_method.cached_data["cpp_call_params"] %>), oResult );<%
end %>
}<% end ; else %>
<%= propBaseClass %>::<%= propBaseClass %>()
{
<% if propBaseClass == "C#{$cur_module.name}SingletonBase" %>
    RHODESAPP().getExtManager().registerExtension( "<%= $cur_module.name %>", this ); <%   
end %>
}<%end

if propBaseClass != "C#{$cur_module.name}SingletonBase" %>
C<%= $cur_module.name %>SingletonBase::C<%= $cur_module.name %>SingletonBase()
{
    RHODESAPP().getExtManager().registerExtension( "<%= $cur_module.name %>", this );
}<%end%>

C<%= $cur_module.name %>SingletonBase::~C<%= $cur_module.name %>SingletonBase()
{
    C<%= $cur_module.name %>FactoryBase::setInstance(0);
}

<%$cur_module.parents.each do |parent| %>
}<%
end %>
