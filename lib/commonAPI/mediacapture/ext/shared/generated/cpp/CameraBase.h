#include "ICamera.h"
#include "logging/RhoLog.h"
#include "common/StringConverter.h"
#include "common/ExtManager.h"


namespace rho {

using namespace rho::apiGenerator;

class CCameraFactoryBase : public CModuleFactoryBase<ICamera, ICameraSingleton, ICameraFactory>
{
protected:
    static rho::common::CAutoPtr<CCameraFactoryBase> m_pInstance;
    HashtablePtr<rho::String,ICamera*> m_hashModules;

public:

    static void setInstance(CCameraFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CCameraFactoryBase* getInstance(){ return m_pInstance; }

    static ICameraSingleton* getCameraSingletonS(){ return getInstance()->getModuleSingleton(); }

    virtual ICamera* getModuleByID(const rho::String& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            ICamera* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }

    virtual ICamera* createModuleByID(const rho::String& strID){ return (ICamera*)0; };
    virtual void deleteModuleByID(const rho::String& strID)
    {
        m_hashModules.remove(strID);
    }

};

class CCameraSingletonBase : public CModuleSingletonBase< ICameraSingleton >, public rho::common::IRhoExtension
{
protected:
    DEFINE_LOGCLASS;


    rho::String m_strDefaultID;




public:
    virtual rho::LogCategory getModuleLogCategory(){ return getLogCategory(); }

    CCameraSingletonBase();
    ~CCameraSingletonBase();




    virtual void setDefaultID(const rho::String& strDefaultID){ m_strDefaultID = strDefaultID; }
    virtual rho::String getDefaultID()
    { 
        if ( m_strDefaultID.length() == 0 )
            setDefaultID(getInitialDefaultID());
        return m_strDefaultID; 
    }

};

class CCameraBase: public ICamera
{
protected:
    DEFINE_LOGCLASS;


    rho::Hashtable<rho::String, rho::String> m_hashProps;
    rho::Hashtable<rho::String, rho::apiGenerator::CMethodAccessor< ICamera > *> m_mapPropAccessors;

public:


    CCameraBase();

    virtual void getProperty( const rho::String& propertyName, CMethodResult& oResult);
    virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, CMethodResult& oResult);
    virtual void getAllProperties(CMethodResult& oResult);
    virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult);
    virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult);
    virtual void clearAllProperties(CMethodResult& oResult);


    virtual void getDesiredWidth(rho::apiGenerator::CMethodResult& oResult);

    virtual void setDesiredWidth( int value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getDesiredHeight(rho::apiGenerator::CMethodResult& oResult);

    virtual void setDesiredHeight( int value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getCompressionFormat(rho::apiGenerator::CMethodResult& oResult);

    virtual void setCompressionFormat( const rho::String& value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getOutputFormat(rho::apiGenerator::CMethodResult& oResult);

    virtual void setOutputFormat( const rho::String& value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getColorModel(rho::apiGenerator::CMethodResult& oResult);

    virtual void setColorModel( const rho::String& value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getEnableEditing(rho::apiGenerator::CMethodResult& oResult);

    virtual void setEnableEditing( bool value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getFlashMode(rho::apiGenerator::CMethodResult& oResult);

    virtual void setFlashMode( const rho::String& value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getSaveToSharedGallery(rho::apiGenerator::CMethodResult& oResult);

    virtual void setSaveToSharedGallery( bool value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getCaptureSound(rho::apiGenerator::CMethodResult& oResult);

    virtual void setCaptureSound( const rho::String& value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getPreviewLeft(rho::apiGenerator::CMethodResult& oResult);

    virtual void setPreviewLeft( int value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getPreviewTop(rho::apiGenerator::CMethodResult& oResult);

    virtual void setPreviewTop( int value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getPreviewWidth(rho::apiGenerator::CMethodResult& oResult);

    virtual void setPreviewWidth( int value, rho::apiGenerator::CMethodResult& oResult);

    virtual void getPreviewHeight(rho::apiGenerator::CMethodResult& oResult);

    virtual void setPreviewHeight( int value, rho::apiGenerator::CMethodResult& oResult);


    static CCameraBase* getInstance(){ return static_cast< CCameraBase* >(CCameraFactoryBase::getInstance()->getModuleByID(CCameraFactoryBase::getCameraSingletonS()->getDefaultID())); }
 

};

extern "C" void Init_Camera_API();


}
