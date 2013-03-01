#include "SimpleOnlyStaticModuleBase.h"
#include "common/RhodesApp.h"


namespace rho {
namespace examples {

IMPLEMENT_LOGCLASS(CSimpleOnlyStaticModuleSingletonBase, "SimpleOnlyStaticModule");
IMPLEMENT_LOGCLASS(CSimpleOnlyStaticModuleBase, "SimpleOnlyStaticModule");

rho::common::CAutoPtr< CSimpleOnlyStaticModuleFactoryBase> CSimpleOnlyStaticModuleFactoryBase::m_pInstance;



///////////////////////////////////////
//string constants definiton 

////////////////////////////////////////////////

CSimpleOnlyStaticModuleBase::CSimpleOnlyStaticModuleBase()
{

}
CSimpleOnlyStaticModuleSingletonBase::CSimpleOnlyStaticModuleSingletonBase()
{
    RHODESAPP().getExtManager().registerExtension( "SimpleOnlyStaticModule", this );
}

CSimpleOnlyStaticModuleSingletonBase::~CSimpleOnlyStaticModuleSingletonBase()
{
    CSimpleOnlyStaticModuleFactoryBase::setInstance(0);
}


}
}
