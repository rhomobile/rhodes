#include "../../../shared/generated/cpp/<%= name.camel_case %>Base.h"
#include "MethodResultImpl.h"

using namespace <%= name.camel_case() %>Runtime;

namespace rho {

using namespace apiGenerator;

class C<%= name.camel_case() %>Impl: public C<%= name.camel_case %>Base
{
    I<%= name.camel_case() %>Impl^ _runtime;
public:
    C<%= name.camel_case %>Impl(const rho::String& strID): C<%= name.camel_case %>Base()
    {
        m_hashProps.put( "display", "LCD");
        m_hashProps.put( "sound", "Dolby");
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
    {
        // _runtime->enable(..., ref new CMethodResultImpl((int64)&oResult));
    }
    
    virtual void start(CMethodResult& oResult)
    {
        _runtime->start(ref new CMethodResultImpl((int64)&oResult));
    }

    virtual void stop(CMethodResult& oResult)
    {
        _runtime->stop(ref new CMethodResultImpl((int64)&oResult));
    }

    virtual void disable(CMethodResult& oResult)
    {
        _runtime->disable(ref new CMethodResultImpl((int64)&oResult));
    }

    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
    {
        // _runtime->take(..., ref new CMethodResultImpl((int64)&oResult));
    }

    virtual void clearProps(CMethodResult& oResult)
    {
    }

    virtual void registerRuntime(I<%= name.camel_case() %>Impl^ runtime)
    {
        _runtime = runtime;
    }
};

class C<%= name.camel_case() %>Singleton: public C<%= name.camel_case() %>SingletonBase
{
    ~C<%= name.camel_case %>Singleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class C<%= name.camel_case() %>Factory: public C<%= name.camel_case() %>FactoryBase
{
    ~C<%= name.camel_case() %>Factory(){}
    virtual I<%= name.camel_case() %>Singleton* createModuleSingleton();
    virtual I<%= name.camel_case() %>* createModuleByID(const rho::String& strID);
};

extern "C" void Init_<%= name.camel_case() %>_extension()
{
    C<%= name.camel_case() %>Factory::setInstance( new C<%= name.camel_case() %>Factory() );
    Init_<%= name.camel_case() %>_API();
}

I<%= name.camel_case() %>* C<%= name.camel_case() %>Factory::createModuleByID(const rho::String& strID)
{
    return new C<%= name.camel_case() %>Impl(strID);
}

I<%= name.camel_case() %>Singleton* C<%= name.camel_case() %>Factory::createModuleSingleton()
{
    return new C<%= name.camel_case() %>Singleton();
}

void C<%= name.camel_case() %>Singleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String C<%= name.camel_case() %>Singleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}
