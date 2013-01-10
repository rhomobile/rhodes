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

    virtual void getProps(const rho::String& strName, CMethodResult& oResult)
    {
        oResult.set(m_hashProps[strName]);
    }
    virtual void getProps(const rho::Vector<rho::String>& arNames, CMethodResult& oResult)
    {
        oResult.set(m_hashProps);
    }

};

template<typename ModuleClass, typename BaseClass>
class CModuleSingletonBase : public BaseClass
{
protected:
    rho::String m_strDefaultID;
    rho::Hashtable<rho::String,ModuleClass*> m_hashModules;
    rho::common::CAutoPtr<rho::common::CThreadQueue> m_pCommandQueue;
    
public:
    
    rho::Hashtable<rho::String,ModuleClass*>& getModules(){ return m_hashModules; }

    void setDefaultID(const rho::String& strDefaultID){ m_strDefaultID = strDefaultID; }

    void setCommandQueue( rho::common::CThreadQueue* pQueue){ m_pCommandQueue = pQueue; }
    void addCommandToQueue(rho::common::IRhoRunnable* pFunctor)
    {
        if ( !m_pCommandQueue )
        {
            m_pCommandQueue = new CGeneratorQueue();
            m_pCommandQueue->start(rho::common::CThreadQueue::epLow);
        }

        m_pCommandQueue->addQueueCommand( new CGeneratorQueue::CGeneratorQueueCommand(pFunctor) );
    }

    rho::String getDefaultIDEx()
    { 
        if ( m_strDefaultID.length() == 0 )
            setDefaultID(getDefaultID());

        if ( !getModules().containsKey(m_strDefaultID) )
        {
            ModuleClass* pObj = create(m_strDefaultID);
            getModules().put(m_strDefaultID, pObj );
        }
        return m_strDefaultID; 
    }
};

