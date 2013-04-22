#include "../../../shared/generated/cpp/<%= namecamelcase %>Base.h"

namespace rho {

using namespace apiGenerator;

class C<%= namecamelcase %>Impl: public C<%= namecamelcase %>Base
{
public:
    C<%= namecamelcase %>Impl(const rho::String& strID): C<%= namecamelcase %>Base()
    {
        m_hashProps.put( "display", "LCD");
        m_hashProps.put( "sound", "Dolby");
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

};

class C<%= namecamelcase %>Singleton: public C<%= namecamelcase %>SingletonBase
{
    ~C<%= namecamelcase %>Singleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class C<%= namecamelcase %>Factory: public C<%= namecamelcase %>FactoryBase
{
    ~C<%= namecamelcase %>Factory(){}
    virtual I<%= namecamelcase %>Singleton* createModuleSingleton();
    virtual I<%= namecamelcase %>* createModuleByID(const rho::String& strID);
};

extern "C" void Init_<%= namecamelcase %>_extension()
{
    C<%= namecamelcase %>Factory::setInstance( new C<%= namecamelcase %>Factory() );
    Init_<%= namecamelcase %>_API();
}

I<%= namecamelcase %>* C<%= namecamelcase %>Factory::createModuleByID(const rho::String& strID)
{
    return new C<%= namecamelcase %>Impl(strID);
}

I<%= namecamelcase %>Singleton* C<%= namecamelcase %>Factory::createModuleSingleton()
{
    return new C<%= namecamelcase %>Singleton();
}

void C<%= namecamelcase %>Singleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String C<%= namecamelcase %>Singleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}