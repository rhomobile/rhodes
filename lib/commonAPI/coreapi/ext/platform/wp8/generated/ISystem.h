#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"

///////////////////////////////////////////////////////////
struct ISystem
{
    virtual ~ISystem(){}

    virtual void getProperty( const rho::StringW& propertyName, CMethodResult& oResult) = 0;
    virtual void getProperties( const rho::Vector<rho::StringW>& arrayofNames, CMethodResult& oResult) = 0;
    virtual void getAllProperties(CMethodResult& oResult) = 0;
    virtual void setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, CMethodResult& oResult) = 0;
    virtual void setProperties( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult) = 0;
    virtual void clearAllProperties(CMethodResult& oResult) = 0;

};

struct ISystemSingleton
{
    virtual ~ISystemSingleton(){}



    virtual rho::StringW getDefaultID() = 0;
    virtual rho::StringW getInitialDefaultID() = 0;
    virtual void setDefaultID(const rho::StringW& strID) = 0;

    virtual void addCommandToQueue(rho::common::IRhoRunnable* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct ISystemFactory
{
    virtual ~ISystemFactory(){}

    virtual ISystemSingleton* getModuleSingleton() = 0;


    virtual ISystem* getModuleByID(const rho::StringW& strID) = 0;

};

class CSystemFactoryBase : public CModuleFactoryBase<ISystem, ISystemSingleton, ISystemFactory>
{
protected:
    static rho::common::CAutoPtr<CSystemFactoryBase> m_pInstance;

public:

    static void setInstance(CSystemFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CSystemFactoryBase* getInstance(){ return m_pInstance; }

    static ISystemSingleton* getSystemSingletonS(){ return getInstance()->getModuleSingleton(); }
};

extern "C" void Init_System_API();
