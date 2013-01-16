#pragma once

#include "common/RhoStd.h"
#include "GeneratorQueue.h"

template<typename ModuleClass>
class CModuleBase: public ModuleClass
{
protected:

    rho::String m_strID;
    rho::Hashtable<rho::String, rho::String> m_hashProps;

public:
    CModuleBase(const rho::String& strID)
    {
        m_strID = strID;
    }

    virtual void getProps(CMethodResult& oResult)
    { 
        oResult.set(m_hashProps);
    }

    virtual void getPropsWithString(const rho::String& strName, CMethodResult& oResult)
    {
        oResult.set(m_hashProps[strName]);
    }
    virtual void getPropsWithArray(const rho::Vector<rho::String>& arNames, CMethodResult& oResult)
    {
        oResult.set(m_hashProps);
    }

};

template<typename ModuleClass>
class CModuleSingletonBase : public ModuleClass
{
protected:
    rho::String m_strDefaultID;
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

    void setCommandQueue( rho::common::CThreadQueue* pQueue){ m_pCommandQueue = pQueue; }
    virtual void addCommandToQueue(rho::common::IRhoRunnable* pFunctor)
    {
        if ( !m_pCommandQueue )
        {
            m_pCommandQueue = new CGeneratorQueue();
            m_pCommandQueue->start(rho::common::CThreadQueue::epLow);
        }

        m_pCommandQueue->addQueueCommand( new CGeneratorQueue::CGeneratorQueueCommand(pFunctor) );
    }

    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor)
    {
        new CCallInThread(pFunctor);
    }

    virtual void setDefaultID(const rho::String& strDefaultID){ m_strDefaultID = strDefaultID; }
    virtual rho::String getDefaultID()
    { 
        if ( m_strDefaultID.length() == 0 )
            setDefaultID(getInitialDefaultID());
        return m_strDefaultID; 
    }
};

template<typename ModuleClass, typename SingletonClass, typename BaseClass>
class CModuleFactoryBase : public BaseClass
{
protected:
    rho::common::CAutoPtr<SingletonClass> m_pModuleSingleton;
    rho::Hashtable<rho::String,ModuleClass*> m_hashModules;

public:

    virtual SingletonClass* getModuleSingleton()
    {
        if ( !m_pModuleSingleton )
            m_pModuleSingleton = createModuleSingleton();

        return m_pModuleSingleton;
    }

    virtual ModuleClass* getModuleByID(const rho::String& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            ModuleClass* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }
    
    virtual SingletonClass* createModuleSingleton() = 0;
    virtual ModuleClass* createModuleByID(const rho::String& strID) = 0;

};
