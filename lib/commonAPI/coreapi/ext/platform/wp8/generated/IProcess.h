#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"


namespace rho {
namespace system {
///////////////////////////////////////////////////////////
struct IProcess
{
    virtual ~IProcess(){}

    virtual void waitForApplication(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void closeHandle(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getProcessExitCode(rho::apiGenerator::CMethodResult& oResult) = 0;

};

struct IProcessSingleton
{
    virtual ~IProcessSingleton(){}

    virtual void runApplication( const rho::StringW& appName,  const rho::StringW& params,  bool blockingCall, rho::apiGenerator::CMethodResult& oResult) = 0;


    virtual void addCommandToQueue(rho::common::IRhoRunnable* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct IProcessFactory
{
    virtual ~IProcessFactory(){}

    virtual IProcessSingleton* getModuleSingleton() = 0;


};

class CProcessFactoryBase : public rho::apiGenerator::CModuleFactoryBase<IProcess, IProcessSingleton, IProcessFactory>
{
protected:
    static rho::common::CAutoPtr<CProcessFactoryBase> m_pInstance;

public:

    static void setInstance(CProcessFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CProcessFactoryBase* getInstance(){ return m_pInstance; }

    static IProcessSingleton* getProcessSingletonS(){ return getInstance()->getModuleSingleton(); }
};

extern "C" void Init_Process_API();


}
}
