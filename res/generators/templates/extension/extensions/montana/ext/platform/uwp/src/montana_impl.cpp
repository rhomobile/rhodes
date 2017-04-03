#include "../../../shared/generated/cpp/<%= namecamelcase %>Base.h"

namespace rho {

using namespace apiGenerator;

class C<%= namecamelcase %>Impl: public C<%= namecamelcase %>Base
{
public:
    C<%= namecamelcase %>Impl(const rho::String& strID): C<%= namecamelcase %>Base()
    {
    }

    virtual void getPlatformName(rho::apiGenerator::CMethodResult& oResult) {
         oResult.set("UWP");
    }

    virtual void calcSumm( int a,  int b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }
    
    virtual void joinStrings( const rho::String& a,  const rho::String& b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }
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