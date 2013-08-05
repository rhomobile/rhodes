#include "../../../shared/generated/cpp/CameraBase.h"
#include <common/RhodesApp.h>							// Rhodes Application utilities
#include "Imager.h"
#include <sstream>

#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "dmoguids.lib")
#pragma comment(lib, "strmiids.lib") 
#pragma comment(lib, "strmbase.lib")

PListDevName g_ListDevName=NULL;

namespace rho {

using namespace apiGenerator;
using namespace common;

class CCameraImpl: public IRhoExtension, public CCameraBase
{
private:
	CImager *m_pImager; // Main Imager object ported from RE2 plugin

	// Overridden properties
	unsigned int m_iDesiredHeight;
	unsigned int m_iDesiredWidth;
public:
    CCameraImpl(const rho::String& strID): CCameraBase()
    {
		LOG(INFO) + "2100:Calling CCameraImpl constructor for [" + strID + "] ...";

		m_hashProps.put( "ID", strID);
	
		LOG(INFO) + "2110:Initialising Scanner Interface for Camera [" + strID + "]";
		RHODESAPP().getExtManager().registerExtension("Camera", this );

		rho::common::CRhoExtData rhodes_data = rho::common::CRhodesApp::getInstance()->getExtManager().makeExtData();

		HWND hwndMainWindow = rhodes_data.m_hBrowserWnd;
		HINSTANCE hInstance = (HINSTANCE) (rhodes_data.m_hInstance);

		m_pImager = new CImager(hInstance, hwndMainWindow, 1);

		if (m_pImager)
		{
			LOG(INFO) + "2120:Initialising imager ...";
			m_pImager->InitImager();
			LOG(INFO) + "2130:Imager initialized";
		}
		else
		{
			LOG(ERROR) + "2140:Imager object NOT created";
		}

		LOG(INFO) + "2199:CCameraImpl constructor for [" + strID + "] has been called.";
    }

	~CCameraImpl()
	{
		LOG(INFO) + "2210:Shutting down Camera ...";

		m_pImager->DeInit();

		if (m_pImager)
			delete m_pImager;

		LOG(INFO) + "2299:Camera shut down.";
	}

	virtual void takePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(INFO) + "CCameraImpl::Takepicture ...";

		if (!oResult.hasCallback())
		{
			LOG(ERROR) + "CCameraImpl::Takepicture: Callback not defined";
			oResult.set(FALSE);
			return;
		}
	
		LOG(INFO) + "CCameraImpl::Takepicture: Set callback for camera event";
		m_pImager->SetImagerCaptureEvent(&oResult);

#ifndef IMG1
		wcscpy(m_pImager->m_szDevName,L"CAM1:");
		m_pImager->SetName(L"CAM1-Test.jpg");
#else
		wcscpy(m_pImager->m_szDevName,L"IMG1:");
		m_pImager->SetName(L"IMG1-Test.jpg");
#endif
		LOG(INFO) + "CCameraImpl::Takepicture: StartViewer[" + m_pImager->m_szDevName + "]";
		m_pImager->StartViewer();

		LOG(INFO) + "... CCameraImpl::Takepicture";

		oResult.set(false);
	}

	/////////////////////////////////////////////////////////////////////////////////
	virtual bool onWndMsg(MSG& oMsg)
 	///////////////////////////////////////////////////////////////////////////////
    {
        if ((oMsg.message == WM_KEYDOWN) &&
            (oMsg.wParam == VK_RETURN))
        {
			LOG(INFO) + "Return received";
            return (m_pImager->SoftTrigger() != TRUE);
        }
        return true;
    }

	virtual void getCameraInfo(rho::apiGenerator::CMethodResult& oResult) {}
	virtual void choosePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {}
	virtual void saveImageToDeviceGallery(rho::apiGenerator::CMethodResult& oResult) {}
	virtual void getSupportedSizeList(rho::apiGenerator::CMethodResult& oResult) {}

	// Override properties to set desired width and height of image.
	void setDesiredHeight( int desiredHeight, rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(INFO) + __FUNCTION__ + "m_iDesiredHeight is being set to " + desiredHeight;
		setProperty( "desiredHeight", rho::common::convertToStringA(desiredHeight), oResult );
		m_iDesiredHeight = desiredHeight;
		if (m_pImager) {
			m_pImager->SetDesiredHeight(m_iDesiredHeight);
		}
	}

	void setDesiredWidth( int desiredWidth, rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(INFO) + __FUNCTION__ + "m_iDesiredWidth is being set to " + desiredWidth;
		setProperty( "desiredWidth", rho::common::convertToStringA(desiredWidth), oResult );
		m_iDesiredWidth = desiredWidth;
		if (m_pImager) {
			m_pImager->SetDesiredWidth(m_iDesiredWidth);
		}
	}
};

class CCameraSingleton: public CCameraSingletonBase
{
public:
	CCameraSingleton(): CCameraSingletonBase() {}
private:
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
    arIDs.addElement("IMG1");
    arIDs.addElement("CAM2");

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