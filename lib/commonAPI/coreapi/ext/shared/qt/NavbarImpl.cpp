#include "../generated/cpp/NavbarBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"

namespace rho {

using namespace apiGenerator;
using namespace common;

class CNavbarImpl: public CNavbarSingletonBase
{
public:

    CNavbarImpl(): CNavbarSingletonBase(){}

    virtual void create(const rho::Hashtable<rho::String, rho::String>& NavbarProperties, rho::apiGenerator::CMethodResult& oResult)
    {
        RAWLOGC_WARNING("Navbar", "create is not supported");
    }

    virtual void remove(rho::apiGenerator::CMethodResult& oResult)
    {
        RAWLOGC_WARNING("Navbar", "remove is not supported");
    }

    virtual void started(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(false);
    }
};

////////////////////////////////////////////////////////////////////////

class CNavbarFactory: public CNavbarFactoryBase
{
public:
    ~CNavbarFactory(){}

    INavbarSingleton* createModuleSingleton()
    { 
        return new CNavbarImpl();
    }
};

}

//extern "C" void Init_Navbar_API();

extern "C" void Init_Navbar()
{
    rho::CNavbarFactory::setInstance( new rho::CNavbarFactory() );
    rho::Init_Navbar_API();
    //RHODESAPP().getExtManager().requireRubyFile("RhoNavbarApi");
}
