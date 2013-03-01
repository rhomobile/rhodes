#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"


namespace rho {
///////////////////////////////////////////////////////////
struct ICamera
{
//constants

    static const char COMPRESSION_FORMAT_JPG[];// "jpg" 
    static const char COMPRESSION_FORMAT_PNG[];// "png" 
    static const char OUTPUT_FORMAT_IMAGE[];// "image" 
    static const char OUTPUT_FORMAT_DATAURI[];// "dataUri" 
    static const char FLASH_ON[];// "on" 
    static const char FLASH_OFF[];// "off" 
    static const char FLASH_AUTO[];// "auto" 
    static const char FLASH_RED_EYE[];// "redEye" 
    static const char FLASH_TORCH[];// "torch" 

//methods
    virtual ~ICamera(){}

    virtual void getDesiredWidth(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setDesiredWidth( int value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getDesiredHeight(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setDesiredHeight( int value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getCompressionFormat(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setCompressionFormat( const rho::String& value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getOutputFormat(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setOutputFormat( const rho::String& value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getColorModel(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setColorModel( const rho::String& value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getEnableEditing(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setEnableEditing( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getFlashMode(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setFlashMode( const rho::String& value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getSaveToSharedGallery(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setSaveToSharedGallery( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getCaptureSound(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setCaptureSound( const rho::String& value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getPreviewLeft(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setPreviewLeft( int value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getPreviewTop(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setPreviewTop( int value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getPreviewWidth(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setPreviewWidth( int value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getPreviewHeight(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setPreviewHeight( int value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void takePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getCameraInfo(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void choosePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void saveImageToDeviceGallery(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getProperty( const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getAllProperties(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void clearAllProperties(rho::apiGenerator::CMethodResult& oResult) = 0;

};

struct ICameraSingleton
{
//constants


    virtual ~ICameraSingleton(){}

//methods
    virtual void enumerate(rho::apiGenerator::CMethodResult& oResult) = 0;


    virtual rho::String getDefaultID() = 0;
    virtual rho::String getInitialDefaultID() = 0;
    virtual void setDefaultID(const rho::String& strID) = 0;

    virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct ICameraFactory
{
    virtual ~ICameraFactory(){}

    virtual ICameraSingleton* getModuleSingleton() = 0;


    virtual ICamera* getModuleByID(const rho::String& strID) = 0;

};


}
