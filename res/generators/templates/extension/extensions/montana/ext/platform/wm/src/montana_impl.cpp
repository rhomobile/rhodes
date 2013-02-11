#include "../../../shared/generated/cpp/<%= name.camel_case %>Base.h"

namespace rho {

using namespace apiGenerator;

class C<%= name.camel_case %>Impl: public C<%= name.camel_case %>Base
{
public:
    C<%= name.camel_case %>Impl(const rho::StringW& strID): C<%= name.camel_case %>Base()
    {
        m_hashProps.put( L"display", L"LCD");
        m_hashProps.put( L"sound", L"Dolby");
    }

    virtual void enable( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult){}

};

class C<%= name.camel_case %>Singleton: public CModuleSingletonBase<I<%= name.camel_case %>Singleton>
{
    ~C<%= name.camel_case %>Singleton(){}
    virtual rho::StringW getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class C<%= name.camel_case %>Factory: public C<%= name.camel_case %>FactoryBase
{
    ~C<%= name.camel_case %>Factory(){}
    virtual I<%= name.camel_case %>Singleton* createModuleSingleton();
    virtual I<%= name.camel_case %>* createModuleByID(const rho::StringW& strID);
};

extern "C" void Init_<%= name.camel_case %>_extension()
{
    C<%= name.camel_case %>Factory::setInstance( new C<%= name.camel_case %>Factory() );
    Init_<%= name.camel_case %>_API();
}

I<%= name.camel_case %>* C<%= name.camel_case %>Factory::createModuleByID(const rho::StringW& strID)
{
    return new C<%= name.camel_case %>Impl(strID);
}

I<%= name.camel_case %>Singleton* C<%= name.camel_case %>Factory::createModuleSingleton()
{
    return new C<%= name.camel_case %>Singleton();
}

void C<%= name.camel_case %>Singleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::StringW> arIDs;
    arIDs.addElement(L"SC1");
    arIDs.addElement(L"SC2");

    oResult.set(arIDs);
}

rho::StringW C<%= name.camel_case %>Singleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::StringW>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}