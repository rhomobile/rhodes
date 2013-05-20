#include "../../../shared/generated/cpp/NativeTabbarBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "rubyext/NativeToolBarExt.h"

namespace rho {

using namespace apiGenerator;
using namespace common;

class CNativeTabbarImpl: public CNativeTabbarSingletonBase
{
public:

    CNativeTabbarImpl(): CNativeTabbarSingletonBase(){}

    virtual void create( const rho::Vector<rho::String>& tabElements,  const rho::Hashtable<rho::String, rho::String>& tabBarProperties, rho::apiGenerator::CMethodResult& oResult)
    {
        rho_osximpl_tabbar_create( tabElements, tabBarProperties, oResult );
    }

    virtual void currentTabIndex(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( rho_osximpl_tabbar_currentTabIndex() );
    }

    virtual void remove(rho::apiGenerator::CMethodResult& oResult)
    {
        rho_osximpl_tabbar_remove();
    }

    virtual void setTabBadge( int tabIndex,  const rho::String& badge, rho::apiGenerator::CMethodResult& oResult)
    {
        rho_osximpl_tabbar_setTabBadge(tabIndex, badge);
    }

    virtual void switchTab( int tabIndex, rho::apiGenerator::CMethodResult& oResult)
    {
        rho_osximpl_tabbar_switchTab(tabIndex);
    }

    virtual void isCreated(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( rho_osximpl_tabbar_isStarted() );
    }

    virtual void removeTab(int, rho::apiGenerator::CMethodResult&)
    {
        // TODO: implement
    }
};

////////////////////////////////////////////////////////////////////////

class CNativeTabbarFactory: public CNativeTabbarFactoryBase
{
public:
    ~CNativeTabbarFactory(){}

    INativeTabbarSingleton* createModuleSingleton()
    { 
        return new CNativeTabbarImpl(); 
    }
};

}

extern "C" void Init_NativeTabbar()
{
    rho::CNativeTabbarFactory::setInstance( new rho::CNativeTabbarFactory() );
    rho::Init_NativeTabbar_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoNativeTabbarApi");

}
