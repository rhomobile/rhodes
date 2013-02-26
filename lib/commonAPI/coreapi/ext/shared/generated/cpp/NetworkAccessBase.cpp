#include "NetworkAccessBase.h"
#include "common/RhodesApp.h"


namespace rho {

IMPLEMENT_LOGCLASS(CNetworkAccessSingletonBase, "NetworkAccess");
IMPLEMENT_LOGCLASS(CNetworkAccessBase, "NetworkAccess");

rho::common::CAutoPtr< CNetworkAccessFactoryBase> CNetworkAccessFactoryBase::m_pInstance;



///////////////////////////////////////
//string constants definiton 
const char INetworkAccessSingleton::AUTH_BASIC[] = "basic"; 

////////////////////////////////////////////////

CNetworkAccessSingletonBase::CNetworkAccessSingletonBase()
{

    RHODESAPP().getExtManager().registerExtension( "NetworkAccess", this ); 
}

CNetworkAccessSingletonBase::~CNetworkAccessSingletonBase()
{
    CNetworkAccessFactoryBase::setInstance(0);
}


}
