//
//  CameraImpl.cpp
#include "../../../../shared/generated/cpp/CameraBase.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoUtil.h"
#include "logging/RhoLog.h"
#include "Imager.h"
#include "DirectShowCam.h"
#include "agyshelldef.h" //not an sdk file
#include "aygshell.h"
#include <atltime.h>



namespace rho {

	using namespace apiGenerator;
	using namespace common;
	
	class CCameraSingletonImpl: public CCameraSingletonBase
	{
	private:
		rho::Hashtable<String, eCamType> m_DeviceNameMap;
		rho::apiGenerator::CMethodResult m_pCb;
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

			bool bRunningOnWM = false;
			OSVERSIONINFO osvi;
			m_pCb = oResult;
			memset(&osvi, 0, sizeof(OSVERSIONINFO));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx(&osvi);
			bRunningOnWM = (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) ||
				(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1);		
			if(bRunningOnWM)
			{
				eImageOutputFormat eFormat = eImageUri;
				//get output format
				typedef std::map<rho::String, rho::String>::const_iterator it_type;
				for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
				{
					if(cmp(convertToStringW(iterator->first).c_str(), L"outputFormat"))
					{
						if(cmp(convertToStringW(iterator->second).c_str(), L"dataUri"))
						{
							eFormat = eDataUri;
						}
						break;
							
					}
				}

				choosePicture(eFormat, oResult);
				
				
				
			}
			else
			{
				UpdateCallbackStatus("error", "ChoosePicture not supported on this device!!","");
			}

		} 
		// saveImageToDeviceGallery Save an image to the device gallery. 
		virtual void saveImageToDeviceGallery( const rho::String& pathToImage, rho::apiGenerator::CMethodResult& oResult) {
			// RAWLOGC_INFO("saveImageToDeviceGallery","Camera");

		} 
		rho::String getInitialDefaultID();
		eCamType getCamType(String deviceName)
		{
			return m_DeviceNameMap[deviceName];
		}
		void choosePicture( eImageOutputFormat eFormat, rho::apiGenerator::CMethodResult& oResult)
		{
			TCHAR image_uri[MAX_PATH];
			OPENFILENAMEEX ofnex = {0};
			//OPENFILENAME ofn = {0};
			image_uri[0] = 0;
			// Get the parent window handle, i.e. the webkit window, and the application instance
			rho::common::CRhoExtData rhodes_data = RHODESAPP().getExtManager().makeExtData();

			ofnex.lStructSize     = sizeof(ofnex);
			ofnex.hwndOwner       = rhodes_data.m_hBrowserWnd;
			ofnex.lpstrFilter     = NULL;
			ofnex.lpstrFile       = image_uri;
			ofnex.nMaxFile        = MAX_PATH;
			ofnex.lpstrInitialDir = NULL;
			ofnex.lpstrTitle      = _T("Select an image");
			BOOL bRes = FALSE;
			ofnex.ExFlags = OFN_EXFLAG_THUMBNAILVIEW|OFN_EXFLAG_NOFILECREATE|OFN_EXFLAG_LOCKDIRECTORY;				
			bRes = lpfn_GetOpen_FileEx(&ofnex);
			if (bRes)
			{
				//get the file name				
				StringW strFullName = image_uri;	

				rho::String imageUri;
				int nWidth;
				int nHeight;

				if(eDataUri == eFormat)
				{
					//  Rather than get bogged down the Direct Show again we'll just
					//  read the file back in from disk.
					HANDLE hFile = CreateFile(strFullName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 
						FILE_ATTRIBUTE_NORMAL, NULL);			

					if(hFile)
					{
						LPVOID pImageBuffer;///< Buffer store the image						
						DWORD dwFileSize = GetFileSize(hFile, NULL);
						if (dwFileSize > 0)
						{		
							DWORD dwBytesRead = 0;
							pImageBuffer = new BYTE[dwFileSize];
							if(pImageBuffer)
							{
								bool bFileReadSuccess = true;
								do
								{
									if (!ReadFile(hFile, pImageBuffer, dwFileSize, &dwBytesRead, NULL))
									{
										//  Some error has occured reading the file
										LOG(INFO) + L"Unable to read image";	
										bFileReadSuccess = false;
										break;
									}
								}while (dwBytesRead != 0);

								if(bFileReadSuccess)
								{									
									rho::common::GetDataURI((BYTE*)pImageBuffer, dwFileSize, imageUri);
									rho::common::GetJpegResolution((BYTE*)pImageBuffer, dwFileSize, nWidth, nHeight);
								}
								delete[] pImageBuffer;
								pImageBuffer = NULL;
							}


						}
						CloseHandle(hFile);
					}
				}
				else
				{
					imageUri = rho::common::convertToStringA(strFullName);
					rho::common::GetJpegResolution(strFullName.c_str(), nWidth, nHeight);
				}

				UpdateCallbackStatus("ok","",imageUri, eFormat, nWidth, nHeight);



			}
			else
			{
				DWORD dwResult = GetLastError();
				UpdateErrorStatus(dwResult);
			}
		}
		void UpdateCallbackStatus(rho::String status, rho::String message, rho::String imageUri, eImageOutputFormat eFormat =eImageUri, int nImageWidth =0, int nImageHeight =0)
		{
			char tempVal[6];

			rho::Hashtable<rho::String, rho::String> statusData;
			statusData.put( "status", status);	

			tempVal[0] = 0;
			sprintf(tempVal,"%d",nImageHeight);
			statusData.put( "imageHeight",tempVal);	
			statusData.put( "image_height", tempVal);
			tempVal[0] = 0;
			sprintf(tempVal,"%d",nImageWidth);
			statusData.put( "imageWidth", tempVal);		
			statusData.put( "image_width", tempVal);

			if("ok" == status)
			{	

				rho::String outputFormat;
				if(eFormat == eImageUri)
				{
					outputFormat = "image";
					//for image path, set file:// as well so that user can access the link
					rho::String pathPrefix = "file://";
					imageUri= pathPrefix + imageUri;

				}
				else
				{
					outputFormat = "dataUri";
				}		
				statusData.put( "imageFormat", outputFormat);
				statusData.put( "imageUri", imageUri);
				statusData.put( "image_format", imageUri);
				statusData.put( "image_uri", outputFormat);
				statusData.put( "message", "");

			}
			else
			{
				//for cancel or error set only message
				statusData.put( "message", message);
				statusData.put( "imageFormat", "");
				statusData.put( "imageUri", "");	
				statusData.put( "image_format", "");
				statusData.put( "image_uri", "");		

			}
			m_pCb.set(statusData);		

		} 
		void UpdateErrorStatus(DWORD dwResult)
		{
			switch(dwResult)
			{
			case ERROR_SUCCESS:
				{
					UpdateCallbackStatus("cancel", "User cancelled the dialog..", "");
					break;
				}
			case ERROR_OUTOFMEMORY:
				{
					UpdateCallbackStatus("error", "Operation ran out of memory..", "");
					break;
				}
			default:
				{
					UpdateCallbackStatus("error", "Operation failed..", "");
					break;
				}
			}
		}

	};
	void CCameraSingletonImpl::enumerate(CMethodResult& oResult)
	{
		LOG(INFO) +  L"inside enumerate method";	
		BOOL bEnumerationStatus = FALSE;
		rho::Vector<rho::String> arIDs;
		if(m_DeviceNameMap.size() > 0)
		{
			bEnumerationStatus = TRUE;
			//if already enumerated
			for ( HashtablePtr<String, eCamType>::iterator it = m_DeviceNameMap.begin(); it != m_DeviceNameMap.end(); ++it )
			{
				arIDs.addElement(it->first);
			}
		}
		else
		{
			bEnumerationStatus = CDirectShowCam:: enumerate(arIDs, m_DeviceNameMap) ;
			bEnumerationStatus = CImager::enumerate(arIDs, m_DeviceNameMap) || bEnumerationStatus;
		}
		if(TRUE == bEnumerationStatus)
		{
			oResult.set(arIDs);
		}
		else
		{
			oResult.set("");
		}
	}
	rho::String CCameraSingletonImpl::getInitialDefaultID()
	{
		CMethodResult oRes;
		enumerate(oRes);

		rho::Vector<rho::String>& arIDs = oRes.getStringArray();

		if (arIDs.isEmpty())
			return "";
		else        
			return arIDs[0];
	}

	//CCameraImpl act as an interface between jav script and the actual implementation
	//or in other words it asct as a proxy and route the call to the actual implementation class
	class CCameraImpl : public CCameraBase
	{
	private:
		ICam* pCamera;
	public:
		CCameraImpl(const rho::String& strID)
		{
			LOG(INFO) + "Initialising interface for Camera " + strID; 
			m_hashProps.put( "ID", strID);
			//RHODESAPP().getExtManager().registerExtension(/*convertToStringA*/(strID), this );
			eCamType camType= ((CCameraSingletonImpl*)(rho::CCameraFactoryBase::getCameraSingletonS()))->getCamType(strID);
			rho::StringW id = rho::common::convertToStringW(strID);
			if(eImager == camType)
			{
				pCamera = new CImager(id.c_str());
			}
			else
			{
				pCamera = new CDirectShowCam(id.c_str());
			}
		}
		virtual ~CCameraImpl() 
		{
			LOG(INFO) + "Shutting down Camera "; 
			if (pCamera)
				delete pCamera;
		}

		//methods

		virtual void takePicture( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {
			if(pCamera)
			{

				CMethodResult oRes;
				setProperties(propertyMap, oRes);              	
				if (oResult.hasCallback())
				{
					DEBUGMSG(true, (L"Callback"));
					pCamera->SetCallback(oResult);                      
					pCamera->takeFullScreen();
				}


			}

		} 

		virtual void showPreview( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {
			if(pCamera)
			{

				CMethodResult oRes;
				setProperties(propertyMap, oResult);
				pCamera->showPreview();                

			}

		} 

		virtual void hidePreview(rho::apiGenerator::CMethodResult& oResult) {
			if(pCamera)
			{
				pCamera->hidePreview();               

			}

		} 

		virtual void capture(rho::apiGenerator::CMethodResult& oResult) {
			if(pCamera)
			{              	
				if (oResult.hasCallback()){
					DEBUGMSG(true, (L"Callback"));
					pCamera->SetCallback(oResult);                      
					pCamera->Capture();
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
					pCamera->setProperty(convertToStringW(iterator->first).c_str(), convertToStringW(iterator->second).c_str());
				}
				oResult.set(true);
			}

		} 
		virtual void getSupportedSizeList(rho::apiGenerator::CMethodResult& oResult)
		{
			//this API is unsupported
		}
	};


	////////////////////////////////////////////////////////////////////////

	class CCameraFactory: public CCameraFactoryBase    {
	public:
		CCameraFactory(){}

		ICameraSingleton* createModuleSingleton()
		{ 
			return new CCameraSingletonImpl();		
		}

		virtual ICamera* createModuleByID(const rho::String& strID){ return new CCameraImpl(strID); };

	};

}

extern "C" void Init_Camera_extension()
{
	rho::CCameraFactory::setInstance( new rho::CCameraFactory() );
	rho::Init_Camera_API();

}