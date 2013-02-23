#include "NetworkAccessBase.h"


namespace rho {

rho::common::CAutoPtr< CNetworkAccessFactoryBase> CNetworkAccessFactoryBase::m_pInstance;



///////////////////////////////////////
//string constants definiton 
const char INetworkAccessSingleton::AUTH_BASIC[] = "basic"; 

////////////////////////////////////////////////

CNetworkAccessSingletonBase::CNetworkAccessSingletonBase()
{
}
}
