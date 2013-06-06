#include "../../../shared/generated/cpp/<%= name.camel_case %>Base.h"

namespace rho {

using namespace apiGenerator;

class C<%= name.camel_case %>Impl: public C<%= name.camel_case %>Base
{
public:
    C<%= name.camel_case %>Impl(const rho::String& strID): C<%= name.camel_case %>Base()
    {
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

};

class C<%= name.camel_case %>Singleton: public C<%= name.camel_case %>SingletonBase
{
    ~C<%= name.camel_case %>Singleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class C<%= name.camel_case %>Factory: public C<%= name.camel_case %>FactoryBase
{
    ~C<%= name.camel_case %>Factory(){}
    virtual I<%= name.camel_case %>Singleton* createModuleSingleton();
    virtual I<%= name.camel_case %>* createModuleByID(const rho::String& strID);
};

extern "C" void Init_<%= name.camel_case %>_extension()
{
    C<%= name.camel_case %>Factory::setInstance( new C<%= name.camel_case %>Factory() );
    Init_<%= name.camel_case %>_API();
}

I<%= name.camel_case %>* C<%= name.camel_case %>Factory::createModuleByID(const rho::String& strID)
{
    return new C<%= name.camel_case %>Impl(strID);
}

I<%= name.camel_case %>Singleton* C<%= name.camel_case %>Factory::createModuleSingleton()
{
    return new C<%= name.camel_case %>Singleton();
}

void C<%= name.camel_case %>Singleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String C<%= name.camel_case %>Singleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}