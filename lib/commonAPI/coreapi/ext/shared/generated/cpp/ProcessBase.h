#include "IProcess.h"
#include "common/StringConverter.h"


namespace rho {
namespace system {

using namespace rho::apiGenerator;

class CProcessFactoryBase : public CModuleFactoryBase<IProcess, IProcessSingleton, IProcessFactory>
{
protected:
    static rho::common::CAutoPtr<CProcessFactoryBase> m_pInstance;
    Hashtable<rho::StringW,IProcess*> m_hashModules;

public:

    static void setInstance(CProcessFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CProcessFactoryBase* getInstance(){ return m_pInstance; }

    static IProcessSingleton* getProcessSingletonS(){ return getInstance()->getModuleSingleton(); }

    virtual IProcess* getModuleByID(const rho::StringW& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            IProcess* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }

    virtual IProcess* createModuleByID(const rho::StringW& strID){ return (IProcess*)0; };

};

class CProcessSingletonBase : public CModuleSingletonBase< IProcessSingleton >
{
protected:

public:


};

class CProcessBase: public IProcess
{
protected:

public:
    CProcessBase();


 

};

extern "C" void Init_Process_API();


}
}
