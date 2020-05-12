#include "../../../shared/generated/cpp/NativeToolbarBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "rubyext/NativeToolbarExt.h"
#include "qt/rhodes/impl/NativeToolbarImpl.h"

namespace rho {

using namespace apiGenerator;
using namespace common;

class CNativeToolbarImpl: public CNativeToolbarSingletonBase
{
public:

    CNativeToolbarImpl(): CNativeToolbarSingletonBase(){}

    virtual void create( const rho::Vector<rho::String>& toolbarElements,  const rho::Hashtable<rho::String, rho::String>& toolBarProperties, rho::apiGenerator::CMethodResult& oResult)
    {
        rho_osximpl_toolbar_create(toolbarElements, toolBarProperties );
    }

    virtual void remove(rho::apiGenerator::CMethodResult& oResult)
    {
        rho_osximpl_toolbar_remove();
    }

    virtual void isCreated(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( rho_osximpl_toolbar_isStarted() );
    }
};

////////////////////////////////////////////////////////////////////////

class CNativeToolbarFactory: public CNativeToolbarFactoryBase
{
public:
    ~CNativeToolbarFactory(){}

    INativeToolbarSingleton* createModuleSingleton()
    { 
        return new CNativeToolbarImpl(); 
    }
};

}

extern "C" void Init_NativeToolbar()
{
    rho::CNativeToolbarFactory::setInstance( new rho::CNativeToolbarFactory() );
    rho::Init_NativeToolbar_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoNativeToolbarApi");

}
