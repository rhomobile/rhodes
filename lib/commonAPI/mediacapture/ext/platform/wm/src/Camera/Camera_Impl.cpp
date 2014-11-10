//
//  CameraImpl.cpp
#include "../../../../shared/generated/cpp/CameraBase.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"
#include "Camera.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;
    
    class CCameraSingletonImpl: public CCameraSingletonBase
    {
	private:
		  rho::Hashtable<String, eCamType> m_CamType;
    public:
        
        CCameraSingletonImpl(): CCameraSingletonBase(){}
        
        //methods
        // enumerate Returns the cameras present on your device, allowing you to access your device's front or back camera. 
        virtual void enumerate(rho::apiGenerator::CMethodResult& oResult);
        // getCameraByType Returns the camera of requested type if that camera exist - else return nil. 
        virtual void getCameraByType( const rho::String& cameraType, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("getCameraByType","Camera");
            
        } 
        // choosePicture Choose a picture from the album. 
        virtual void choosePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("choosePicture","Camera");
            
        } 
        // saveImageToDeviceGallery Save an image to the device gallery. 
        virtual void saveImageToDeviceGallery( const rho::String& pathToImage, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("saveImageToDeviceGallery","Camera");
            
        } 

    };
	void CCardReaderSingleton::enumerate(CMethodResult& oResult)
	{
		BOOL bEnumerationStatus = FALSE;
		rho::Vector<rho::String> arIDs;
		bEnumerationStatus = ((CImager::enumerate(arIDs, m_CamType))|| (CDirectShowCam:: enumerate(arIDs, m_CamType)));
		if(TRUE == bEnumerationStatus)
		{
			oResult.set(arIDs);
		}
		else
		{
			oResult.set("");
		}
	}
    
	//CCameraImpl act as an interface between jav script and the actual implementation
	//or in other words it asct as a proxy and route the call to the actual implementation class
	class CCameraImpl : public CCameraBase
	{
	private:
		ICam* pCamera;
		static bool bIsCameraRunning;
	public:
		CCameraImpl();
		virtual ~CCameraImpl() {}

		//methods

		virtual void takePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {
			if(pCamera)
			{
				if(false == bIsCameraRunning)
				{
					//TODO: 
					//bIsCameraRunning logic for full screen has to be relooked, when will set it to false
					bIsCameraRunning = true;
					pCamera->takePicture(propertyMap, oResult);
					bIsCameraRunning = false;
					
				}
			}

		} 

		virtual void showPreview(rho::apiGenerator::CMethodResult& oResult) {
			if(pCamera)
			{
				if(false == bIsCameraRunning)
				{
					pCamera->showPreview(oResult);
					bIsCameraRunning = true;
				}
			}

		} 

		virtual void hidePreview(rho::apiGenerator::CMethodResult& oResult) {
			if(pCamera)
			{
				if(true == bIsCameraRunning)
				{
					pCamera->hidePreview(oResult);
					bIsCameraRunning = false;
				}
			}

		} 

		virtual void Capture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {
			if(pCamera)
			{
				if(true == bIsCameraRunning)
				{
					pCamera->Capture(propertyMap, oResult);
				}
			}

		} 

		virtual void getProperty( const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult)
		{
			if(pCamera)
			{				
				if (m_hashProps.containsKey(propertyName))
					oResult.set(m_hashProps.get(propertyName));
				else
				{			
					WCHAR szValue[MAX_PATH];
					BOOL bResult = pCamera->getProperty(convertToStringW(propertyName).c_str(), szValue);
					LOG(INFO) +  L"Getting Property " + convertToStringW(propertyName).c_str() + " as " + szValue;
					if (szValue && (bResult == TRUE))
					{
						rho::StringW szStringValue;
						szStringValue.insert(0, szValue);
						oResult.set(szStringValue);
					}			
					else
					{
						oResult.set(L"Unavailable");
					}
				}

			}

		} 

		virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, rho::apiGenerator::CMethodResult& oResult)
		{
			if(pCamera)
			{
				rho::Hashtable<rho::String, rho::String> propsHash;
				CMethodResult oRes;
				typedef std::vector<rho::String>::const_iterator it_type;
				for (it_type iterator = arrayofNames.begin(); iterator != arrayofNames.end(); iterator++)
				{
					getProperty(*iterator, oRes);
					propsHash.put(*iterator, convertToStringA(oRes.toString()));
				}
				oResult.set(propsHash);
			}

		} 

		virtual void getAllProperties(rho::apiGenerator::CMethodResult& oResult) 
		{
			if(pCamera)
			{

				rho::Hashtable<rho::String, rho::String> propsHash;
				CMethodResult oRes;
				rho::Vector<rho::String> arrayofNames;
				pCamera->getSupportedPropertyList(arrayofNames);				
				typedef std::vector<rho::String>::const_iterator it_type;
				for (it_type iterator = arrayofNames.begin(); iterator != arrayofNames.end(); iterator++)
				{
					getProperty(*iterator, oRes);			
					propsHash.put(*iterator, convertToStringA(oRes.toString()));
				}
				oResult.set(propsHash);		
			}

		} 

		virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, rho::apiGenerator::CMethodResult& oResult) 
		{
			if(pCamera)
			{				
				LOG(INFO) +  L"Setting Property " + propertyName.c_str() + " to " + propertyValue.c_str();		
				BOOL bRet = pCamera->setProperty(convertToStringW(propertyName).c_str(), convertToStringW(propertyValue).c_str());
				oResult.set(bRet == TRUE);
			}

		} 

		virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) 
		{
			if(pCamera)
			{	
				//  Set multiple properties				
				typedef std::map<rho::String, rho::String>::const_iterator it_type;
				for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
				{
					LOG(INFO) +  L"Setting Property " + iterator->first.c_str() + " to " + iterator->second.c_str();			
					pCamera->SetProperty(convertToStringW(iterator->first).c_str(), convertToStringW(iterator->second).c_str());
				}
				oResult.set(true);
			}

        } 

    };
	bool CCameraImpl::bIsCameraRunning= false;
    
    ////////////////////////////////////////////////////////////////////////
    
    class CCameraFactory: public CCameraFactoryBase    {
    public:
        CCameraFactory(){}
        
        ICameraSingleton* createModuleSingleton()
        { 
           // return new CCameraSingletonImpl();
        }
        
        virtual ICamera* createModuleByID(const rho::String& strID){ return new CCameraImpl(); };
        
    };
    
}

extern "C" void Init_Camera_extension()
{
    rho::CCameraFactory::setInstance( new rho::CCameraFactory() );
    rho::Init_Camera_API();
    
}