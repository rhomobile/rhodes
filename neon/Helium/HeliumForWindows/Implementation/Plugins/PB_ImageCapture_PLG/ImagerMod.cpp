/**
 * \file ImagerMod.cpp
 * \brief Definition of CImagerMod class.
 *
 * CImagerMod Class is derived from base class PBModule. Handles META TAG processing request from Core, META TAG should be in EMML 1.1 format.
 *
 */
#include "ImagerMod.h"
#include "..\..\Engine\Common\pbengine_defs.h"

//to store wav file to be played during capture
TCHAR			m_tcImagerSound[MAX_PATH];
//Username for FTP server
//TCHAR			m_tcImagerUsername[MAX_PATH];
//password for FTP server
//TCHAR			m_tcImagerPassword[MAX_PATH];
///Flag indicating http transfer is requested
BOOL			m_bHttpTransfer=FALSE;
//Navigation url 
extern TCHAR m_ImagerURL[2050];
//Enum var to hold active image capture device
extern EnumImageCapDevType g_ActiveImageCapDevice;
//global list of available image capture device
extern PListDevName g_ListDevName;
//Vars for color camera , VF dimensions and position
extern DWORD g_dwTop,g_dwLeft,g_dwWidth,g_dwHeight;
extern BOOL g_bTop,g_bLeft,g_bWidth, g_bHeight;
//Events for synchronization
extern HANDLE g_hBuildGraph,g_hTaskPending;
//For Navigation
//extern TCHAR m_IMGNav[2050];
//AT any time only one instance of imager is possible
DWORD g_InstanceCount=0;
////////////////////////////////////////////////////////////////////////
// Function:	CImagerMod
// Description:	Constructor.
//              
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
CImagerMod::CImagerMod(void)
{
	m_bImgOn=FALSE;
	m_pImagerInst = NULL;
	
}
////////////////////////////////////////////////////////////////////////
// Function:	~CImagerMod
// Description:	Destructor.
//              
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
CImagerMod::~CImagerMod(void)
{
	return;
}
////////////////////////////////////////////////////////////////////////
// Function:	onInit
// Description:	Public function called when first instance of Imager is loaded.
// initializes member m_szModName with Imager.
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImagerMod::onInit(PPBSTRUCT pPBStructure)
{
UNREFERENCED_PARAMETER(pPBStructure);
	//Copy The module name
	wcscpy(m_szModName, L"Imager");
	RegisterForEvent(RHO_APPFOCUS_EVENT);
	m_bAppHasFocus = (pPBStructure->bLaunchingAppHasFocus==TRUE);
	if(RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT)){
		m_FileTransferIMO = CreateIMO(L"filetransfer");
		if(m_FileTransferIMO){
			if(SetIMOCallBack (m_FileTransferIMO,IMOCBFunc,(LPARAM)this)){
				return TRUE;
			}
		}
	}
	return FALSE;
}
////////////////////////////////////////////////////////////////////////
// Function:	onDeInit
// Description:	This Fucntion is called when dispose on final instance is called 
// 
//
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
void CImagerMod::onDeInit(PBStruct *pPBStructure)
{
UNREFERENCED_PARAMETER(pPBStructure);
	DeleteIMO(m_FileTransferIMO);	
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	UnRegisterForEvent(RHO_APPFOCUS_EVENT);
}
////////////////////////////////////////////////////////////////////////
// Function:	onAttachInstance
// Description:	Public function called every time new instance is created.
// does initializations for Imager instance upon creation.
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImagerMod::onAttachInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
	//Log
	BOOL bRet=FALSE;
	LPCTSTR pVal;

	CImager *pImager=NULL;
	if(g_InstanceCount == 0)
	{

		pInstStruct->pWrappedObj = pImager = 
			(CImager*)new CImager(pPBStructure->hInstance,
				pPBStructure->hWnd,
				pPBStructure->iTabID,
				this);
		if(pInstStruct->pWrappedObj){

			pImager->m_pPBStructure.hWnd = pPBStructure->hWnd;
			pImager->m_pPBStructure.hInstance = pPBStructure->hInstance;
			pImager->m_pPBStructure.bInvoked = pPBStructure->bInvoked;
			pImager->m_pPBStructure.iTabID = pPBStructure->iTabID;

			//Initialize the wav for imager from config file
			pVal = GetAppSettingPtr(pImager->m_pPBStructure.iTabID,L"Sound\\ImagerCaptureWav",L"Value");
	
			if(pVal)
			{
				int iStart = 0;
				if (wcslen(pVal) > wcslen(L"file://") &&
					wcsnicmp(pVal, L"file://", 7) == 0)
					iStart = 7;
				wcscpy(m_tcImagerSound, pVal + iStart);
			}

			bRet = TRUE;
			g_InstanceCount++;
		}
		
	}
	
	return bRet;
}
////////////////////////////////////////////////////////////////////////
// Function:	onReleaseInstance
// Description:	Called during scanner instance deletion also does cleanup activity 
//              before instance deletion.
//
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImagerMod::onReleaseInstance(PPBSTRUCT pPBStructure, PPBINSTSTRUCT pInstStruct)
{
UNREFERENCED_PARAMETER(pPBStructure);
	//Log
//	DWORD dwRetValue;
	if(1 == g_InstanceCount)
	{
//		CImager *pImager = (CImager *)pInstStruct->pWrappedObj;
		delete (CImager *)pInstStruct->pWrappedObj;
		pInstStruct->pWrappedObj = NULL;
		g_InstanceCount--;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Function:	MetaProc
// Description:	public function invked whenever META tag needs processing 
//              
//
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImagerMod::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, VOID* pParam)
{
	Log(PB_LOG_WARNING, L"The Imager API has been deprecated in 4.0, please transition your applications to use the Camera API for future releases", _T(__FUNCTION__), __LINE__);
	BOOL bRet=TRUE;

	CImager *pImager = (CImager *)pParam;
//	LPCTSTR pParameters = pbMetaStructure->lpParameter;
//	LPCTSTR pValue = pbMetaStructure->lpValue;
	
	//Check if Imager parameter is valid
	if(!pImager)
	{
		return FALSE;
	}


	//Initialize the PPBStruct
	if(pPBStructure!=NULL)
	{
		pImager->m_pPBStructure.hWnd = pPBStructure->hWnd;
		pImager->m_pPBStructure.hInstance = pPBStructure->hInstance;
		pImager->m_pPBStructure.bInvoked = pPBStructure->bInvoked;
		pImager->m_pPBStructure.iTabID = pPBStructure->iTabID;
	}

	if(cmp(pbMetaStructure->lpParameter,L"Enabled") || 
		cmp(pbMetaStructure->lpParameter,L"Enable"))
	{
		if (m_bAppHasFocus) {
			if (!m_bImgOn) {
				//m_bImgOn flag is going to be set within EnableImager
				EnableImager(pImager,pbMetaStructure->lpValue);
				Log(PB_LOG_DEBUG, L"Imager enabled", L"Imager",__LINE__);
			}
		}
		else {
			wcscpy(m_szBgImager, pbMetaStructure->lpValue);
		}
	}
	else if(cmp(pbMetaStructure->lpParameter,L"disabled") || cmp(pbMetaStructure->lpParameter,L"disable")){
		if (m_bAppHasFocus) {
			if (m_bImgOn) {
				// Imager is disabled so stop it
				m_bImgOn=FALSE;				
				pImager->DeInit();
				Log(PB_LOG_DEBUG, L"Imager disabled", L"Imager",__LINE__);
			}
		}
		else {
			m_szBgImager[0] = L'\0';
		}
	}
	else if(cmp(pbMetaStructure->lpParameter, L"capture"))
	{
		if (m_bImgOn)
		{
			bRet = FALSE;	
	
			if (pImager->m_hImager != INVALID_HANDLE_VALUE)
				pImager->Image_StopViewfinder(pImager->m_hImager);

			if (m_tcImagerSound[0] && (wcslen(m_tcImagerSound) >0) && 
				(!pImager->DirectShowShutterClick()))
				PlaySound(m_tcImagerSound,NULL,SND_FILENAME|SND_ASYNC);
						
			if(pImager->SaveImage()==SUCCESS)
			{
				bRet = TRUE;
				////////////////////////Send Image if configured//////////////////////////////////
				if(m_FileTransferIMO)
				{
					//make sure the inter module object to communicate with file transfer exists
					//ensure that a destination etc has been set otherwise IMO is not needed
					if(pImager->m_bTransfer)
					{				
						bRet = FALSE;
						// By default set the Overwrite parameter to TRUE
						SetIMOProperty(m_FileTransferIMO, L"overwrite", L"true");
						if(SetIMOProperty(m_FileTransferIMO, L"Source",pImager->m_lpSzFilename))
						{
							if (SetIMOProperty(m_FileTransferIMO, L"SetFileDestination", L"FALSE"))
							{
								bRet = CallIMOMethod(m_FileTransferIMO ,L"Transfer");
							}
						}
					}	
				}
			}
				
			if (pImager->m_hImager != INVALID_HANDLE_VALUE)
				pImager->Image_StartViewfinder(pImager->m_hImager);	
						
					////////////////////////Stop IMO///////////////////////////////////
					//pImager->SendImage();
		}
	}
	else if(cmp(pbMetaStructure->lpParameter, L"ImagerEvent")){
		if(pbMetaStructure->lpValue != NULL)
			bRet = SetIMOProperty(m_FileTransferIMO, L"TransferEvent",pbMetaStructure->lpValue);
			//wcscpy(m_IMGNav,pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"ImagerEnumEvent")){
		if(pbMetaStructure->lpValue != NULL)
			wcscpy(pImager->m_EnumIMGNav,pbMetaStructure->lpValue);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"ImagerCaptureEvent"))
	{
		if (pbMetaStructure->lpValue)
			return pImager->SetImagerCaptureEvent(pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Left")){
		if(NULL != pbMetaStructure->lpValue && isFloat(pbMetaStructure->lpValue) && _wtoi(pbMetaStructure->lpValue) >= 0)
			pImager->SetViewerPosition(_wtoi(pbMetaStructure->lpValue), -9999, -9999, -9999);
		else
			bRet = FALSE;
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Top")){
		if(NULL != pbMetaStructure->lpValue && isFloat(pbMetaStructure->lpValue) && _wtoi(pbMetaStructure->lpValue) >= 0)
			pImager->SetViewerPosition(-9999, -9999, _wtoi(pbMetaStructure->lpValue), -9999);
		else
			bRet = FALSE;
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Width")){
		if(NULL != pbMetaStructure->lpValue && isFloat(pbMetaStructure->lpValue) && _wtoi(pbMetaStructure->lpValue) >= 0)
			pImager->SetViewerPosition(-9999, _wtoi(pbMetaStructure->lpValue), -9999, -9999);		
		else
			bRet = FALSE;
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Height")){
		if(NULL != pbMetaStructure->lpValue && isFloat(pbMetaStructure->lpValue) && _wtoi(pbMetaStructure->lpValue) >= 0)
			pImager->SetViewerPosition(-9999, -9999, -9999, _wtoi(pbMetaStructure->lpValue));		
		else
			bRet = FALSE;
	}
	else if(cmp(pbMetaStructure->lpParameter, L"number"))
	{
		if(NULL != pbMetaStructure->lpValue)
			pImager->m_ImgNo = _wtoi(pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Name")) 
	{
		if (pbMetaStructure->lpValue)
			return pImager->SetName(pbMetaStructure->lpValue);	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"lamp"))
	{
			if(cmp(pbMetaStructure->lpValue, L"on")) pImager->SetOptions(1, 9999);
			else if (cmp(pbMetaStructure->lpValue, L"off")) pImager->SetOptions(0, 9999);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"aim"))
	{
		BOOL bFlag;
			if(CheckImagerOrCamera(pImager->m_szDevName,&bFlag))
			{
				if(!bFlag)
				{
					PBModule::Log(PB_LOG_INFO,L"Aim feature not available in Camera",L"Imager",__LINE__);
					bRet = FALSE;
				}
				else
				{
					if(cmp(pbMetaStructure->lpValue, L"on")) 
						pImager->SetOptions(9999, 1);
					else 
						if (cmp(pbMetaStructure->lpValue, L"off")) 
								pImager->SetOptions(9999, 0);
				}
			}
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Destination"))
	{
		if(pbMetaStructure->lpValue){
			if(SetIMOProperty(m_FileTransferIMO, L"Destination", pbMetaStructure->lpValue)){
				return pImager->m_bTransfer = TRUE;
				
			}
			bRet = FALSE;
		}
	}
	else if(cmp(pbMetaStructure->lpParameter, L"username"))
	{
		if(NULL != pbMetaStructure->lpValue)
			return SetIMOProperty(m_FileTransferIMO, L"Username", pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"password"))
	{
		if(NULL != pbMetaStructure->lpValue)
			return SetIMOProperty(m_FileTransferIMO, L"Password", pbMetaStructure->lpValue);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"protocol"))
	{
		if(NULL != pbMetaStructure->lpValue)
		{
			return SetIMOProperty(m_FileTransferIMO, L"Protocol", pbMetaStructure->lpValue);
		}
	}
	else if(cmp(pbMetaStructure->lpParameter, L"sound"))
	{
		if(NULL != pbMetaStructure->lpValue)
		{
			int iStart = 0;
			if (wcslen(pbMetaStructure->lpValue) > wcslen(L"file://") &&
				wcsnicmp(pbMetaStructure->lpValue, L"file://", 7) == 0)
				iStart = 7;
			wcscpy(m_tcImagerSound, pbMetaStructure->lpValue + iStart);

			if ((pImager) && (pImager->DirectShowShutterClick()))
			{
				pImager->SetDirectShowShutterSound(m_tcImagerSound);
			}
		}
	}
	else if(cmp(pbMetaStructure->lpParameter, L"Enumerate"))
	{
		EnumImager(pImager);	
	}
	else if (cmp(pbMetaStructure->lpParameter, L"desiredWidth"))
	{
		if (pbMetaStructure->lpValue)
			pImager->SetDesiredWidth(_wtoi(pbMetaStructure->lpValue));
	}
	else if (cmp(pbMetaStructure->lpParameter, L"desiredHeight"))
	{
		if (pbMetaStructure->lpValue)
			pImager->SetDesiredHeight(_wtoi(pbMetaStructure->lpValue));
	}

	return bRet;
}

////////////////////////////////////////////////////////////////////////
// Function:	CheckImagerOrCamera
// Description:	Checks if devName is valid imager or Color Camera devicename 
//              if yes return TRUE and bFlag will be TRUE if it imager else it will be FALSE
//
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImagerMod::CheckImagerOrCamera(LPCTSTR devName, BOOL *bFlag)
{
	//Check if device is available in List of imager and DShow camera
	BOOL bDevPresent=FALSE;
	PListDevName pRoot = g_ListDevName;
	
	if(pRoot)
	{
		do
		{
			if(cmp(devName, pRoot->DeviceName))
			{
				bDevPresent = TRUE;
				if(pRoot->bDeviceType)//Imager
				{
					(*bFlag) = TRUE;
				}
				else
					(*bFlag) = FALSE;
			}
			pRoot = pRoot->pNext;
		}while(pRoot != NULL);
	}

	return bDevPresent;
}
////////////////////////////////////////////////////////////////////////
// Function:	EnableImager
// Description:	Enables Imager device 
//             
//
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImagerMod::EnableImager(CImager *pImager, LPCTSTR szDevicename)
{
	BOOL bFlag=FALSE;

	if(pImager == NULL)
	{
		Log(PB_LOG_ERROR,L"imager object does not exist",L"Imager",__LINE__);
		return FALSE;
	}
	//Check if it is empty then open default Imager IMG1:
	if(szDevicename!=NULL)
	{
		if(cmp(szDevicename,L""))
		{
			// use the default imager device if there is one
			if(CheckImagerOrCamera(L"IMG1:",&bFlag))
			{			
				wcscpy(pImager->m_szDevName,L"IMG1:");//default device
			} // otherwise use the existing default (first device or last used device)
		}
		else
		{
			wcscpy(pImager->m_szDevName,szDevicename);//default device
			if (wcsncmp(pImager->m_szDevName,L"IMG",3) == 0)
			{
				pImager->m_szDevName[4]=L':';//Append ':' to device name
				pImager->m_szDevName[5]=L'\0';//Append null character to device name
			}
			else
			{
				wcscat(pImager->m_szDevName, L":\0");
			}
		}
	}
	else if (CheckImagerOrCamera(L"IMG1:",&bFlag)) 		// use the default imager device if there is one
		wcscpy(pImager->m_szDevName,L"IMG1:");//default device

	//Check if device is present on the terminal
	if(CheckImagerOrCamera(pImager->m_szDevName,&bFlag))//return TRue if device is present 
	{	                                        //bFlag will be True if Camera else it will be false
	}
	else
	{//SR 17045
		//already an Imager is active cannot have two image capture device simultaneously
		//Log error here
		Log(PB_LOG_INFO,L"Unknown Imager Device",L"Imager",__LINE__);

		return FALSE;

	}

	//Default Initialize Imager
	if(pImager->m_hImager == INVALID_HANDLE_VALUE)//m_hImager = INVALID_HANDLE_VALUE;
	{
		if (pImager->InitImager()!= E_IMG_SUCCESS)
		{ Log(PB_LOG_ERROR,L"InitImager failed",L"Imager",__LINE__);
		};
	}

	// Imager is enabled so try to start it
	if (pImager->StartViewer() != E_IMG_SUCCESS)
	{
		PBModule::Log(PB_LOG_ERROR,L"StartViewer failed, retrying",L"Imager",__LINE__);
		// if we're switching between apps with the camera active this can happen
		// so retry a few times
		int i=0;
		do 
		{
			if (++i >= 5)
			{
				PBModule::Log(PB_LOG_ERROR,L"StartViewer failed 5 times",L"Imager",__LINE__);
				return FALSE;
			}
			Sleep(100);
		} while (pImager->StartViewer() != E_IMG_SUCCESS);
	}
		
	m_bImgOn= TRUE;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Function:	EnableImager
// Description:	Enables Imager device 
//             
//
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImagerMod::EnumImager(CImager *pImager)
{

	if(pImager->m_EnumIMGNav[0])//if navigation string is valid
	{
		//  Variable to hold the read attribute
		//TCHAR szImagers[MAXURL + 1];
		//  Aiming for :
		//		new Array(
		//		new Array(0, 1, 2),
		//		new Array(1, 2, 3)
		//		);
		memset(&pImager->tc_szImagers, 0, sizeof(pImager->tc_szImagers));
		wcscpy(pImager->tc_szImagers,L"new Array( ");

		PListDevName pRoot = g_ListDevName;

		while(pRoot != NULL)
		{
			//  Remove Comma from end of Device Name
			TCHAR tcModifiedDeviceName[50];
			wcscpy(tcModifiedDeviceName, pRoot->DeviceName);

			if (wcslen(tcModifiedDeviceName) > 0 &&
				tcModifiedDeviceName[wcslen(tcModifiedDeviceName) - 1] == L':')
				tcModifiedDeviceName[wcslen(tcModifiedDeviceName) - 1] = L'\0';

			wcscat(pImager->tc_szImagers, L"new Array('");
			wcscat(pImager->tc_szImagers, tcModifiedDeviceName);
			wcscat(pImager->tc_szImagers, L"','");
			wcscat(pImager->tc_szImagers, pRoot->DeviceFriendlyName);
			wcscat(pImager->tc_szImagers, L" ')");
			
			pRoot = pRoot->pNext;

			if(pRoot!=NULL)//if one more device is there then
				wcscat(pImager->tc_szImagers, L",");
		}
		wcscat(pImager->tc_szImagers, L")");

		//  Send the built array to the caller
		m_pImagerInst = pImager;
		CloseHandle(CreateThread (NULL, 0, SendNavigateURI, this, 0, NULL));
		
	}

	return 0;
}
/**
* \author	Ramanagouda
* \date		January 2010 (Initial Creation)
*/
DWORD CImagerMod::SendNavigateURI (LPVOID pparam)
{
	//  Parameter is a pointer to the notification object
	CImagerMod *pImagerMod = (CImagerMod *) pparam;

	if(pImagerMod->m_pImagerInst !=NULL)
		pImagerMod->SendPBNavigate(tcImagerEnumEventNames, pImagerMod->m_pImagerInst->m_iInstanceID,
			pImagerMod->m_pImagerInst->m_EnumIMGNav,
			pImagerMod->m_pImagerInst->tc_szImagers,
			NULL);

	//pImagerMod->m_pImagerInst = NULL;
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Function:	onBeforeNavigate
// Description:	This Fucntion is a callback function invoked before navigating to new page 
//
//
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImagerMod::onBeforeNavigate(int iInstID)
{
	CImager *pImager;

	PBModule::Log(PB_LOG_DEBUG, L"Change of page for Imager", L"onNaviagtionEvent", __LINE__);

	// Look for app instance pointer of the currently focused app. 
	pImager = (CImager *) GetObjFromID(iInstID);

	//MessageBox(NULL,L"onBeforeNavigate",L"Scanner",MB_OK);
	
	if (pImager != NULL)
	{
		//Reset Navigate String 
		memset(pImager->m_EnumIMGNav, 0, wcslen(pImager->m_EnumIMGNav));
		
		//memset(m_IMGNav, 0, wcslen(m_IMGNav));

		if (m_bImgOn) 
		{
			// Imager is disabled so stop it
			//  Request from SAP to make disabling during navigate configurable
			if (pImager->m_bDisableDuringNavigate)
				pImager->DeInit();
		}
	}
	if (pImager->m_bDisableDuringNavigate)
		m_bImgOn = FALSE;
	return TRUE;
}


//called from another thread 
int CALLBACK CImagerMod::IMOCBFunc(PVARSTRUCT pVars,int iTABID,LPARAM lParam)
{
UNREFERENCED_PARAMETER(pVars);
UNREFERENCED_PARAMETER(iTABID);
UNREFERENCED_PARAMETER(lParam);
//	CImagerMod *pImgMod = (CImagerMod *)lParam;
	//do something with the Module object
	return IMO_PASS_ON;//allow the navigation to go through to Browser
	
}

BOOL CImagerMod::onRhoAppFocus(bool bActivate, int iInstID)
{
	CImager *pImager = (CImager *)GetObjFromID(iInstID);
	if (!pImager)
	{
		m_bAppHasFocus = bActivate;
		return FALSE;
	}

	if (!bActivate && m_bAppHasFocus)
	{
		// we're losing focus
		m_bAppHasFocus = false;
		if (m_bImgOn) 
		{
			// Imager is disabled so stop it
			wcscpy(m_szBgImager, pImager->m_szDevName);
			m_bImgOn=FALSE;				
			pImager->DeInit();
			Log(PB_LOG_DEBUG, L"Imager disabled", L"Imager",__LINE__);
		}
		else
		{
			m_szBgImager[0] = L'\0';
		}
		return TRUE;
	}
	else if (bActivate && !m_bAppHasFocus)
	{
		// we've regained focus
		m_bAppHasFocus = true;

		if (m_szBgImager[0] != L'\0')
		{
			// we need to re-enable the imager, but we can't do it from this thread - we
			// need to send an ONMETA message back to the core to enable the correct imager
			// from our MetaProc on the main thread
			if (m_szBgImager[wcslen(m_szBgImager)-1] == L':')
				m_szBgImager[wcslen(m_szBgImager)-1] = L'\0';
			EngineMETATag tag;
			tag.tcHTTPEquiv = L"Imager";
			WCHAR contents[50];
			wsprintf(contents, L"Enabled:%s", m_szBgImager);
			tag.tcContents = contents;
			SendMessage(m_pCoreStructure->hParentWnd,PB_ONMETA,(WPARAM)iInstID,(LPARAM)&tag);
		}
		return TRUE;
	}
	return FALSE;	
};