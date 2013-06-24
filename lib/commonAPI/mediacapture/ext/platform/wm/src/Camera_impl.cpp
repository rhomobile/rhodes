#include "../../../shared/generated/cpp/CameraBase.h"

namespace rho {

using namespace apiGenerator;

class CCameraImpl: public CCameraBase
{
public:
    CCameraImpl(const rho::String& strID): CCameraBase()
    {
        /*m_hashProps.put( "display", "LCD");
        m_hashProps.put( "sound", "Dolby");*/
    }

	virtual void takePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {}
	virtual void getCameraInfo(rho::apiGenerator::CMethodResult& oResult) {}
	virtual void choosePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {}
	virtual void saveImageToDeviceGallery(rho::apiGenerator::CMethodResult& oResult) {}
	virtual void getSupportedSizeList(rho::apiGenerator::CMethodResult& oResult) {}

};

class CCameraSingleton: public CCameraSingletonBase
{
    ~CCameraSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
	virtual void saveImageToDeviceGallery( const rho::String& pathToImage, rho::apiGenerator::CMethodResult& oResult) {}
	virtual void choosePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {}
	virtual void getCameraByType( const rho::String& cameraType, rho::apiGenerator::CMethodResult& oResult) {}
};

class CCameraFactory: public CCameraFactoryBase
{
    ~CCameraFactory(){}
    virtual ICameraSingleton* createModuleSingleton();
    virtual ICamera* createModuleByID(const rho::String& strID);
};

extern "C" void Init_camera_extension()
{
    CCameraFactory::setInstance( new CCameraFactory() );
    Init_Camera_API();
}

ICamera* CCameraFactory::createModuleByID(const rho::String& strID)
{
    return new CCameraImpl(strID);
}

ICameraSingleton* CCameraFactory::createModuleSingleton()
{
    return new CCameraSingleton();
}

void CCameraSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CCameraSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}