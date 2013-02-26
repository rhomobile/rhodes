#include "ProcessBase.h"


namespace rho {
namespace system {

IMPLEMENT_LOGCLASS(CProcessSingletonBase, "Process");
IMPLEMENT_LOGCLASS(CProcessBase, "Process");

rho::common::CAutoPtr< CProcessFactoryBase> CProcessFactoryBase::m_pInstance;



///////////////////////////////////////
//string constants definiton 

////////////////////////////////////////////////

CProcessBase::CProcessBase()
{
}
}
}
