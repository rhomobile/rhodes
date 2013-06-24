#pragma once

#include "IScreenOrientation.h"
#include "logging/RhoLog.h"
#include "common/StringConverter.h"
#include "common/ExtManager.h"


namespace rho {

using namespace rho::apiGenerator;

// hash keys used in properties and parameters 

class CScreenOrientationFactoryBase : public CModuleFactoryBase<IScreenOrientation, IScreenOrientationSingleton, IScreenOrientationFactory>
{
protected:
    static rho::common::CAutoPtr<CScreenOrientationFactoryBase> m_pInstance;
    HashtablePtr<rho::String,IScreenOrientation*> m_hashModules;

public:

    static void setInstance(CScreenOrientationFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CScreenOrientationFactoryBase* getInstance(){ return m_pInstance; }

    static IScreenOrientationSingleton* getScreenOrientationSingletonS(){ return getInstance()->getModuleSingleton(); }

    virtual IScreenOrientation* getModuleByID(const rho::String& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            IScreenOrientation* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }

    virtual IScreenOrientation* createModuleByID(const rho::String& strID){ return (IScreenOrientation*)0; };
    virtual void deleteModuleByID(const rho::String& strID)
    {
        m_hashModules.remove(strID);
    }

};

class CScreenOrientationSingletonBase : public CModuleSingletonBase< IScreenOrientationSingleton >, public rho::common::IRhoExtension
{
protected:
    DEFINE_LOGCLASS;





public:
    virtual rho::LogCategory getModuleLogCategory(){ return getLogCategory(); }

    CScreenOrientationSingletonBase();
    ~CScreenOrientationSingletonBase();






};

class CScreenOrientationBase: public IScreenOrientation
{
protected:
    DEFINE_LOGCLASS;


public:



 

};

extern "C" void Init_ScreenOrientation_API();


}
