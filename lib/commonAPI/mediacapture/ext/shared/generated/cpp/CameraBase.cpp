#include "CameraBase.h"
#include "common/RhodesApp.h"


namespace rho {

IMPLEMENT_LOGCLASS(CCameraSingletonBase, "Camera");
IMPLEMENT_LOGCLASS(CCameraBase, "Camera");

rho::common::CAutoPtr< CCameraFactoryBase> CCameraFactoryBase::m_pInstance;



///////////////////////////////////////
//string constants definiton 
const char ICamera::COMPRESSION_FORMAT_JPG[] = "jpg"; 
const char ICamera::COMPRESSION_FORMAT_PNG[] = "png"; 
const char ICamera::OUTPUT_FORMAT_IMAGE[] = "image"; 
const char ICamera::OUTPUT_FORMAT_DATAURI[] = "dataUri"; 
const char ICamera::FLASH_ON[] = "on"; 
const char ICamera::FLASH_OFF[] = "off"; 
const char ICamera::FLASH_AUTO[] = "auto"; 
const char ICamera::FLASH_RED_EYE[] = "redEye"; 
const char ICamera::FLASH_TORCH[] = "torch"; 

////////////////////////////////////////////////

CCameraBase::CCameraBase()
{

    m_mapPropAccessors["desiredWidth"] = 0;
    m_mapPropAccessors["desiredHeight"] = 0;
    m_mapPropAccessors["compressionFormat"] = 0;
    m_mapPropAccessors["format"] = 0;
    m_mapPropAccessors["outputFormat"] = 0;
    m_mapPropAccessors["colorModel"] = 0;
    m_mapPropAccessors["enableEditing"] = 0;
    m_mapPropAccessors["flashMode"] = 0;
    m_mapPropAccessors["saveToSharedGallery"] = 0;
    m_mapPropAccessors["captureSound"] = 0;
    m_mapPropAccessors["previewLeft"] = 0;
    m_mapPropAccessors["previewTop"] = 0;
    m_mapPropAccessors["previewWidth"] = 0;
    m_mapPropAccessors["previewHeight"] = 0;


}

void CCameraBase::getProperty( const rho::String& propertyName, CMethodResult& oResult)
{
    CMethodAccessor< ICamera >* pAccessor = m_mapPropAccessors[propertyName];
    if ( pAccessor )
        pAccessor->callGetter(this, oResult);
    else
    {
        
        oResult.set(m_hashProps[propertyName]);
        
    }
}

void CCameraBase::getProperties( const rho::Vector<rho::String>& arrayofNames, CMethodResult& oResult)
{
    rho::Hashtable<rho::String, rho::String> res;
    oResult.setCollectionMode(true);
    for ( int i = 0; i < (int)arrayofNames.size(); i++ )
    {
        getProperty(arrayofNames[i], oResult);

        if ( oResult.isError() )
            break;

        res[arrayofNames[i]] = oResult.toString();
    }

    oResult.setCollectionMode(false);
    if ( oResult.isError() )
        oResult.callCallback();
    else
        oResult.set(res);
}

void CCameraBase::getAllProperties(CMethodResult& oResult)
{
    oResult.set(m_hashProps);
}

void CCameraBase::setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult)
{
    CMethodAccessor< ICamera >* pAccessor = m_mapPropAccessors[propertyName];
    if (pAccessor && pAccessor->hasSetter())
        m_mapPropAccessors[propertyName]->callSetter(this, propertyValue, oResult);
    else
    {
        
        m_hashProps.put(propertyName, propertyValue);
        
    }
}

void CCameraBase::setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
{
    for ( rho::Hashtable<rho::String, rho::String>::const_iterator it = propertyMap.begin();  it != propertyMap.end(); ++it )
    {
        setProperty( it->first, it->second, oResult );
        if ( oResult.isError() )
            break;
    }
}

void CCameraBase::clearAllProperties(CMethodResult& oResult)
{
    m_hashProps.clear();
}



void CCameraBase::getDesiredWidth(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "desiredWidth", oResult); 
}

void CCameraBase::setDesiredWidth( int value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "desiredWidth", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getDesiredHeight(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "desiredHeight", oResult); 
}

void CCameraBase::setDesiredHeight( int value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "desiredHeight", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getCompressionFormat(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "compressionFormat", oResult); 
}

void CCameraBase::setCompressionFormat( const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "compressionFormat", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getOutputFormat(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "outputFormat", oResult); 
}

void CCameraBase::setOutputFormat( const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "outputFormat", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getColorModel(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "colorModel", oResult); 
}

void CCameraBase::setColorModel( const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "colorModel", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getEnableEditing(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "enableEditing", oResult); 
}

void CCameraBase::setEnableEditing( bool value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "enableEditing", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getFlashMode(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "flashMode", oResult); 
}

void CCameraBase::setFlashMode( const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "flashMode", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getSaveToSharedGallery(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "saveToSharedGallery", oResult); 
}

void CCameraBase::setSaveToSharedGallery( bool value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "saveToSharedGallery", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getCaptureSound(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "captureSound", oResult); 
}

void CCameraBase::setCaptureSound( const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "captureSound", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getPreviewLeft(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "previewLeft", oResult); 
}

void CCameraBase::setPreviewLeft( int value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "previewLeft", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getPreviewTop(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "previewTop", oResult); 
}

void CCameraBase::setPreviewTop( int value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "previewTop", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getPreviewWidth(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "previewWidth", oResult); 
}

void CCameraBase::setPreviewWidth( int value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "previewWidth", rho::common::convertToStringA(value), oResult );
}

void CCameraBase::getPreviewHeight(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "previewHeight", oResult); 
}

void CCameraBase::setPreviewHeight( int value, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "previewHeight", rho::common::convertToStringA(value), oResult );
}
CCameraSingletonBase::CCameraSingletonBase()
{
    RHODESAPP().getExtManager().registerExtension( "Camera", this );
}

CCameraSingletonBase::~CCameraSingletonBase()
{
    CCameraFactoryBase::setInstance(0);
}


}
