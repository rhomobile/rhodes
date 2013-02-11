#include "IProcess.h"
#include "common/StringConverter.h"


namespace rho {
namespace system {

using namespace rho::apiGenerator;

class CProcessFactoryBase : public CModuleFactoryBase<IProcess, IProcessSingleton, IProcessFactory>
{
protected:
    static rho::common::CAutoPtr<CProcessFactoryBase> m_pInstance;

public:

    static void setInstance(CProcessFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CProcessFactoryBase* getInstance(){ return m_pInstance; }

    static IProcessSingleton* getProcessSingletonS(){ return getInstance()->getModuleSingleton(); }
};


class CProcessBase: public IProcess
{
protected:

public:
    CProcessBase(){}


 

};

extern "C" void Init_Process_API();


}
}
