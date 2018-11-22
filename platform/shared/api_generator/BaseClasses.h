#pragma once

#include "common/RhoStd.h"
#include "logging/RhoLog.h"
#include "common/StringConverter.h"
#include "GeneratorQueue.h"

namespace rho
{
namespace apiGenerator
{

template<typename ModuleClass>
class CModuleSingletonBase : public ModuleClass
{
protected:
    rho::common::CAutoPtr<rho::common::CThreadQueue> m_pCommandQueue;

    class CCallInThread : public rho::common::CRhoThread
    {
        rho::common::CAutoPtr<rho::common::IRhoRunnable> m_pFunctor;
    public:
        CCallInThread(rho::common::IRhoRunnable* pFunctor): CRhoThread(), m_pFunctor(pFunctor)
        {
            start(epNormal);
        }
        
    private:
        virtual void run(){ m_pFunctor->runObject();  }
        virtual void runObject() {
            rho::common::CRhoThread::runObject();
            delete this;
        }
    };

public:
    ~CModuleSingletonBase()
    {
        if ( m_pCommandQueue )
        {
            m_pCommandQueue->stop(-1);
        }
    }

    void setCommandQueue( rho::common::CThreadQueue* pQueue){ m_pCommandQueue = pQueue; }
    rho::common::CThreadQueue* getCommandQueue() const { return m_pCommandQueue; }
    virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<CMethodResult>* pFunctor)
    {
        if ( !m_pCommandQueue )
        {
            m_pCommandQueue = new CGeneratorQueue();
            #if defined(OS_WINDOWS_DESKTOP) || defined(RHODES_EMULATOR)
                m_pCommandQueue->setPollInterval(1);
            #endif
            m_pCommandQueue->setLogCategory(getModuleLogCategory());
            m_pCommandQueue->start(rho::common::CThreadQueue::epLow);
        }

        m_pCommandQueue->addQueueCommand( createQueueCommand(pFunctor) );
    }

    virtual rho::common::CThreadQueue::IQueueCommand* createQueueCommand(rho::common::CInstanceClassFunctorBase<CMethodResult>* pFunctor)
    {
        return new CGeneratorQueue::CGeneratorQueueCommand(pFunctor);
    }

    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor)
    {
        new CCallInThread(pFunctor);
    }

    virtual rho::LogCategory getModuleLogCategory() = 0;
};

template<typename ModuleClass, typename SingletonClass, typename BaseClass>
class CModuleFactoryBase : public BaseClass
{
protected:
    SingletonClass* m_pModuleSingleton;

public:

    CModuleFactoryBase() : m_pModuleSingleton(0){}
    virtual SingletonClass* getModuleSingleton()
    {
        if ( !m_pModuleSingleton )
            m_pModuleSingleton = createModuleSingleton();

        return m_pModuleSingleton;
    }

    virtual SingletonClass* createModuleSingleton() = 0;

};

template<typename ModuleClass>
class CMethodAccessor
{
public:
    typedef void (ModuleClass::*TGetter)(CMethodResult& oResult);

    struct CSetterBase
    {
        virtual void call(ModuleClass* pModule, const rho::String& strArg, CMethodResult& oResult ) = 0;
    };
    template<typename TArg, typename TValue>        
    class CSetter: public CSetterBase
    {
        typedef void (ModuleClass::*TSetter)(TArg arg, CMethodResult& oResult);
        TSetter m_pSetterFunc;
    public:
        CSetter( TSetter pSetter ) : m_pSetterFunc(pSetter){}
        virtual void call(ModuleClass* pModule, const rho::String& strArg, CMethodResult& oResult )
        {
            TValue arg;
            rho::common::convertFromStringA( strArg.c_str(), arg );
            (pModule->*m_pSetterFunc)( arg, oResult );
        }
    };
private:
    TGetter m_pGetter;
    CSetterBase* m_pSetter;
public:
    CMethodAccessor( TGetter pGetter) : m_pGetter(pGetter), m_pSetter(0){}

    void addSetter(CSetterBase* pSetter){ m_pSetter = pSetter; }
    void callGetter( ModuleClass* pModule, CMethodResult& oResult ){ (pModule->*m_pGetter)(oResult); }
    void callSetter( ModuleClass* pModule, const rho::String& strArg, CMethodResult& oResult){ m_pSetter->call( pModule, strArg, oResult); }
    bool hasSetter()const{ return m_pSetter != 0; }
};

}
}
