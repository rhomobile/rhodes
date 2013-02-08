#include "I<%= $cur_module.name %>.h"

<% $cur_module.parents.each do |parent| %>
namespace <%= parent.downcase() %> {<%
end %>

using namespace ::rho::apiGenerator;
class C<%= $cur_module.name %>Base: public I<%= $cur_module.name %>
{
protected:
<% if $cur_module.is_template_propertybag %>
    ::rho::Hashtable<::rho::StringW, ::rho::StringW> m_hashProps;
<% end %>
public:
    C<%= $cur_module.name %>Base(){}

<% if $cur_module.is_template_propertybag %>
    virtual void getProperty( const ::rho::StringW& propertyName, CMethodResult& oResult)
    {
        oResult.set(m_hashProps[propertyName]);
    }
    virtual void getProperties( const ::rho::Vector<::rho::StringW>& arrayofNames, CMethodResult& oResult)
    {
        ::rho::Hashtable<::rho::StringW, ::rho::StringW> res;
        oResult.setCollectionMode(true);
        for ( int i = 0; i < (int)arrayofNames.size(); i++ )
        {
            getProperty(arrayofNames[i], oResult);

            if ( oResult.isError() )
                break;

            res[arrayofNames[i]] = oResult.toString();
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
    virtual void setProperty( const ::rho::StringW& propertyName,  const ::rho::StringW& propertyValue, CMethodResult& /*oResult*/)
    {
        m_hashProps.put(propertyName, propertyValue);
    }
    virtual void setProperties( const ::rho::Hashtable<::rho::StringW, ::rho::StringW>& propertyMap, CMethodResult& oResult)
    {
        for ( ::rho::Hashtable<::rho::StringW, ::rho::StringW>::const_iterator it = propertyMap.begin();  it != propertyMap.end(); ++it )
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
<% end %>
};

class C<%= $cur_module.name %>FactoryBase : public ::rho::apiGenerator::CModuleFactoryBase<I<%= $cur_module.name %>, I<%= $cur_module.name %>Singleton, I<%= $cur_module.name %>Factory>
{
protected:
    static ::rho::common::CAutoPtr<C<%= $cur_module.name %>FactoryBase> m_pInstance;

public:

    static void setInstance(C<%= $cur_module.name %>FactoryBase* pInstance){ m_pInstance = pInstance; }
    static C<%= $cur_module.name %>FactoryBase* getInstance(){ return m_pInstance; }

    static I<%= $cur_module.name %>Singleton* get<%= $cur_module.name %>SingletonS(){ return getInstance()->getModuleSingleton(); }
};

extern "C" void Init_<%= $cur_module.name %>_API();

<% $cur_module.parents.each do |parent| %>
}<%
end %>
