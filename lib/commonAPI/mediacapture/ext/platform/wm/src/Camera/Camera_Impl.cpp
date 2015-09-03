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
		rho::StringW m_ImageUriPath; //hold the imageUri path, needs to delete during every new choosePictureOperation
		bool m_bIsDeprecated;
	public:

		CCameraSingletonImpl(): CCameraSingletonBase()
		{
			m_bIsDeprecated = false;
		}

		//methods
		// enumerate Returns the cameras present on your device, allowing you to access your device's front or back camera. 
		virtual void enumerate(rho::apiGenerator::CMethodResult& oResult);
		// getCameraByType Returns the camera of requested type if that camera exist - else return nil. 
		virtual void getCameraByType( const rho::String& cameraType, rho::apiGenerator::CMethodResult& oResult) {

			bool bCamFound = false;
			rho::String  camId;

			if(m_DeviceNameMap.size() == 0)
			{
				CMethodResult oRes;
				enumerate(oRes);
			}			
			//get first occurance of the camera name from the m_DeviceNameMap
			if(m_DeviceNameMap.size() > 0)
			{	
				eCamType camType = getCamTypeEnum(cameraType);
				if(eUnknownCamType != camType)
				{
					for ( HashtablePtr<String, eCamType>::iterator it = m_DeviceNameMap.begin(); it != m_DeviceNameMap.end(); ++it )
					{
						if (it->second == camType)
						{
							camId = it->first;
							bCamFound = true;
							break;
						}
					}
				}
			}
			if(bCamFound)
			{
				oResult.set(camId);
			}
			else
			{
				oResult.set("");
			}

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
				eImageOutputFormat eFormat = eImagePath;
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
						else if(cmp(convertToStringW(iterator->second).c_str(), L"image"))
						{
							eFormat = eImageUri;
						}
						else if(cmp(convertToStringW(iterator->second).c_str(), L"imagePath"))
						{
							eFormat = eImagePath;
						}
						break;
							
					}
				}
				m_bIsDeprecated = false;
				if(propertyMap.containsKey("deprecated"))
				{
					if("true" == propertyMap.find("deprecated")->second)
					{
						m_bIsDeprecated = true;
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
		virtual void copyImageToDeviceGallery( const rho::String& pathToImage, rho::apiGenerator::CMethodResult& oResult) {
			// RAWLOGC_INFO("saveImageToDeviceGallery","Camera");

		} 
		rho::String getInitialDefaultID();
		eCamType getCamType(const String& deviceName)
		{
			return m_DeviceNameMap[deviceName];
		}
        eCamType getCamTypeEnum(const String& cameraType)
        {
            eCamType camType = eUnknownCamType;
            if( 0 == strcmp(cameraType.c_str(), "color"))
            {
                camType = eColorCam;
            }
            else
            {
                if( 0 == strcmp(cameraType.c_str(), "imager"))
                {
                    camType = eImager;
                }
            }
            return camType;
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
			ofnex.lpstrFilter     = _T("JPEG Files (*.jpg)\0*.jpg\0");;
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
				int nWidth =0;
				int nHeight =0;

				if(eDataUri == eFormat)
				{
					//  Rather than get bogged down the Direct Show again we'll just
					//  read the file back in from disk.
					HANDLE hFile = CreateFile(strFullName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 
						 FILE_ATTRIBUTE_NORMAL, NULL);			

					if(hFile)
					{
						DWORD dwFileSize = GetFileSize(hFile, NULL);
						bool bCanSupportDataUri = false;
						bCanSupportDataUri = CAN_SUPPORT_DATA_URI(dwFileSize) ? true : false;
						if(bCanSupportDataUri)
						{
							LPVOID pImageBuffer = NULL;///< Buffer store the image
							bool bFileReadSuccess = false;
							if (dwFileSize > 0)
							{		
								DWORD dwBytesRead = 0;
								pImageBuffer = new BYTE[dwFileSize];
								if(pImageBuffer)
								{								
									do
									{
										if (!ReadFile(hFile, pImageBuffer, dwFileSize, &dwBytesRead, NULL))
										{
											//  Some error has occured reading the file
											LOG(INFO) + L"Unable to read image";	
											bFileReadSuccess = false;
											break;
										}
										else
										{
											bFileReadSuccess = true;
										}
									}while (dwBytesRead != 0);


								}
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
						if(!bCanSupportDataUri)
						{
							//enter if cannot support data uri
							rho::common::GetJpegResolution(strFullName.c_str(), nWidth, nHeight);
							UpdateCallbackStatus("error","Failed to prepare dataUri.",imageUri, eDataUri,nWidth, nHeight );

						}
						
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
				WCHAR errorMsg[100];
				DWORD dwResult = GetLastError();
				wsprintf(errorMsg, L"choose picture error code: %d", dwResult);
				LOG(INFO) + errorMsg;
				UpdateErrorStatus(dwResult);
			}
		}
		void UpdateCallbackStatus(rho::String status, rho::String message, rho::String imageUri, eImageOutputFormat eFormat =eImagePath, int nImageWidth =0, int nImageHeight =0)
		{		
			char imageHeight[6];
			char imageWidth[6];

			rho::Hashtable<rho::String, rho::String> statusData;
			statusData.put( "status", status);	

			imageHeight[0] = 0;
			imageWidth[0] = 0;
			sprintf(imageHeight,"%d",nImageHeight);
			sprintf(imageWidth,"%d",nImageWidth);

			if(false == m_bIsDeprecated)
			{
				statusData.put( "imageHeight",imageHeight);
				statusData.put( "imageWidth", imageWidth);	
			}
			else
			{
				statusData.put( "image_height", imageHeight);
				statusData.put( "image_width", imageWidth);
			}

			if("ok" == status)
			{	

				rho::String outputFormat;
				if(eFormat == eImageUri)
				{
					DeleteFile(m_ImageUriPath.c_str());
					rho::String appRootPath;
					rho::String fileName;
					rho::String newFilePath;
					appRootPath = RHODESAPP().getAppRootPath();
					unsigned int index = imageUri.find_last_of("\\");		
					if(index > 0)
					{
						fileName = imageUri.substr(index);
					}
					else
					{
						fileName = imageUri;
					}
					newFilePath = appRootPath + "/" + fileName;
					m_ImageUriPath = rho::common::convertToStringW(newFilePath);
					rho::StringW szExistingPath= rho::common::convertToStringW(imageUri);
					CopyFile(szExistingPath.c_str(), m_ImageUriPath.c_str(), TRUE);					 
					imageUri = fileName;
				}
				outputFormat = "jpg";		
				statusData.put( "message", "");

				if(false == m_bIsDeprecated)
				{
					statusData.put( "imageFormat", outputFormat);
					statusData.put( "imageUri", imageUri);
				}
				else
				{
					statusData.put( "image_format", imageUri);
					statusData.put( "image_uri", outputFormat);
				}
				

			}
			else
			{
				//for cancel or error set only message
				statusData.put( "message", message);
				if(false == m_bIsDeprecated)
				{
					statusData.put( "imageFormat", "");
					statusData.put( "imageUri", "");
				}
				else
				{
					statusData.put( "image_format", "");
					statusData.put( "image_uri", "");	
				}

			}
			m_pCb.set(statusData);		

		} 
		void UpdateErrorStatus(DWORD dwResult)
		{
			switch(dwResult)
			{
			case ERROR_SUCCESS:
				{
					UpdateCallbackStatus("cancel", "User canceled operation.", "");
					break;
				}
			case ERROR_OUTOFMEMORY:
				{
					UpdateCallbackStatus("error", "Operation ran out of memory..", "");
					break;
				}
			case ERROR_INVALID_WINDOW_HANDLE:
				{
					//some wm devices gives error code as ERROR_INVALID_WINDOW_HANDLE on closing the dialog
					//in this case user expects a cancel message
					UpdateCallbackStatus("cancel", "User canceled operation.", "");
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
	static rho::Hashtable<rho::String, rho::String> hashAliasProps;

	//CCameraImpl act as an interface between jav script and the actual implementation
	//or in other words it asct as a proxy and route the call to the actual implementation class
	class CCameraImpl :public IRhoExtension, public CCameraBase
	{
	private:
		ICam* pCamera;
	public:
		CCameraImpl(const rho::String& strID)
		{
			LOG(INFO) + "Initialising interface for Camera " + strID; 
			m_hashProps.put( "ID", strID);
			RHODESAPP().getExtManager().registerExtension(strID, this );//to receive notification from main window, eg:app focus
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
				CCamera::SetAPICallType(false);
				if (oResult.hasCallback())
				{
					if(propertyMap.containsKey("deprecated"))
					{
						if("true" == propertyMap.find("deprecated")->second)
						{
							CCamera::SetAPICallType(true);
						}
					}
					
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
					CCamera::SetAPICallType(false);
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
				{
					oResult.set(m_hashProps.get(propertyName));
				}
				else
				{			
					WCHAR szValue[MAX_PATH];
					BOOL bResult = FALSE;
					if(hashAliasProps.containsKey(propertyName))
					{
						bResult = pCamera->getProperty(convertToStringW(hashAliasProps[propertyName]).c_str(), szValue);
					}
					else
					{
						bResult = pCamera->getProperty(convertToStringW(propertyName).c_str(), szValue);
					}
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
				BOOL bRet = FALSE;
				if(hashAliasProps.containsKey(propertyName))
				{
					//for deprecated property name, get the exisitng name and invoke method
					bRet = pCamera->setProperty(convertToStringW(hashAliasProps[propertyName]).c_str(), convertToStringW(propertyValue).c_str());
				}
				else
				{
					bRet = pCamera->setProperty(convertToStringW(propertyName).c_str(), convertToStringW(propertyValue).c_str());
				}
				oResult.set(bRet == TRUE);
			}

		} 

		virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) 
		{
			if(pCamera)
			{	
				CMethodResult oTempRes;
				//  Set multiple properties				
				typedef std::map<rho::String, rho::String>::const_iterator it_type;
				for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
				{
					LOG(INFO) +  L"Setting Property " + iterator->first.c_str() + " to " + iterator->second.c_str();			
					setProperty(iterator->first, iterator->second, oTempRes);
				}
				oResult.set(true);
			}

		} 
		virtual void getSupportedSizeList(rho::apiGenerator::CMethodResult& oResult)
		{
			if(pCamera)
			{
				StringifyVector supportesSizeList;				
				pCamera->getSupportedSizeList(supportesSizeList);
				rho::String buffer;
				supportesSizeList.toString(buffer);
				oResult.setJSON(buffer);				

			}
		}
		//IRHoExtension manager overrides

		virtual void OnAppActivate(bool bActivate, const CRhoExtData& oExtData)
		{
			if (!pCamera)
			{
				return;
			}
			else
			{
				pCamera->ApplicationFocusChange(bActivate);
			}
		}
		virtual void OnPowerButton(bool bPowerOn, const CRhoExtData& oExtData)
		{
			if (!pCamera)
			{
				return;
			}
			else
			{
				pCamera->OnPowerButton(bPowerOn);
			}
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
//Below method initAliasParams needs to be updated whenever new alias param is introduced
void initAliasParams()
{
	rho::hashAliasProps.put("desired_width", "desiredWidth");
	rho::hashAliasProps.put("desired_height", "desiredHeight");
	rho::hashAliasProps.put("camera_type", "cameraType");
	rho::hashAliasProps.put("flash_mode", "flashMode");
	rho::hashAliasProps.put("left", "previewLeft");
	rho::hashAliasProps.put("top", "previewTop");
	rho::hashAliasProps.put("format", "compressionFormat");
}

extern "C" void Init_Camera_extension()
{
	rho::CCameraFactory::setInstance( new rho::CCameraFactory() );
	rho::Init_Camera_API();
	initAliasParams();//prepare a lookup table to support alias parameters for propertybag

}
extern "C" void Init_Videocapture()
{
}