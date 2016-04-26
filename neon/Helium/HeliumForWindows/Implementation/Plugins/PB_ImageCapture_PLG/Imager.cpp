/**
 * \file Imager.cpp
 * \brief Implementation of the Imager Module Plugin
 * CImager is an abstraction of the Image Capture functionality supported by
 * Symbol Technologies Devices.
 */

#include "DShowCam.h"
#include "Imager.h"
#include "ImagerMod.h"
#include "wininet.h"


//Flag to indicate transfer mode use for image file transfer. TRUE-HTTP transfer else FTP transfer
extern BOOL m_bHttpTransfer;


//Vaiable to hold FTP server password
//extern TCHAR			m_tcImagerPassword[MAX_PATH];
//variable to hold FTP user name 
//extern TCHAR			m_tcImagerUsername[MAX_PATH];

//Buffer to hold Navigation strings
//TCHAR m_IMGNav[2050];
//URL to navigate to for file transfer
TCHAR m_ImagerURL[2050];

//Navigation address passed by user
TCHAR m_NavAddress[2050];

//Not used - delete
HWND g_hParent=NULL;

//List of Image Capture devices available, only one device available as of now
//Color camera to be added in the future.
PListDevName g_ListDevName=NULL;

//#define STATIC_DSCAM

#ifdef STATIC_DSCAM
CDShowCam g_DSCam;
#endif

//CImagerMod instance which creates CImager instances and an interface between COre and CImager
extern CImagerMod *g_ImagerMod;
#ifdef LOG_TO_FILE
extern FILE *LogFile;
#endif
//Message Queue DATA structure
//were Used for color camera.
#define MAX_METALENGTH 30
//#define MAX_URL	2050

struct MetaTagQEntry
{
	TCHAR parameter[MAX_METALENGTH+1];
	TCHAR Value[MAX_URL+1];
};


////////////////////////////////////////////////////////////////////////
// Function:	CImager
// Description:	CImager constructor - Variable initialization done Here, 
//              Loads Image capture Library, Initializes Image Captue API function pointers.
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
CImager::CImager(HINSTANCE hInstance,HWND hParent,int iInstanceID, PBModule* pModule)
	:m_iInstanceID(iInstanceID)
{
#ifdef LOG_TO_FILE
	LogFile = 	fopen("CameraLog2.txt","w");
#endif
	DEBUGIT(LogFile,"Imager constructor entered \n");
	m_pModule = pModule;
	m_PreviewOn = FALSE;
	m_bImagerLoadFailed = true;
	m_bDisableDuringNavigate = true;  //  Default behaviour is to disable during navigate
	hModule = LoadLibraryEx(_T("ImgAPI32.dll"), NULL, 0);
	if (hModule != NULL) {
		m_bImagerLoadFailed = false;
		if (NULL == (Image_FindFirst = (IMAGE_FINDFIRSTPROC) GetProcAddress(hModule, _T("IMAGE_FindFirst")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_FindNext = (IMAGE_FINDNEXTPROC) GetProcAddress(hModule, _T("IMAGE_FindNext")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_FindClose = (IMAGE_FINDCLOSEPROC) GetProcAddress(hModule, _T("IMAGE_FindClose")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_Open = (IMAGE_OPENPROC) GetProcAddress(hModule, _T("IMAGE_Open")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_Close = (IMAGE_CLOSEPROC) GetProcAddress(hModule, _T("IMAGE_Close")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_StartAcquisition = (IMAGE_STARTACQUISITIONPROC) GetProcAddress(hModule, _T("IMAGE_StartAcquisition")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_StopAcquisition = (IMAGE_STOPACQUISITIONPROC) GetProcAddress(hModule, _T("IMAGE_StopAcquisition")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_StartViewfinder = (IMAGE_STARTVIEWFINDERPROC) GetProcAddress(hModule, _T("IMAGE_StartViewfinder")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_StopViewfinder = (IMAGE_STOPVIEWFINDERPROC) GetProcAddress(hModule, _T("IMAGE_StopViewfinder")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_GetImage = (IMAGE_GETIMAGEPROC) GetProcAddress(hModule, _T("IMAGE_GetImage")))) m_bImagerLoadFailed = true; 		
		if (NULL == (Image_SetCapCurrValue = (IMAGE_SETCAPCURRVALUEPROC) GetProcAddress(hModule, _T("IMAGE_SetCapCurrValue")))) m_bImagerLoadFailed = true;
		if (NULL == (Image_UnLockImager = (IMAGE_UNLOCKIMAGERPROC) GetProcAddress(hModule, _T("IMAGE_UnLockImager")))) m_bImagerLoadFailed = true;
	}
	m_bTransfer = FALSE;
	m_lpSzFilename = NULL;
	m_hImager = INVALID_HANDLE_VALUE;
	m_rcWinPos.left = GetSystemMetrics(SM_CXSCREEN)/2-50;
	m_rcWinPos.right = GetSystemMetrics(SM_CXSCREEN)/2+50;
	m_rcWinPos.top = GetSystemMetrics(SM_CYSCREEN)/2-50;
	m_rcWinPos.bottom = GetSystemMetrics(SM_CYSCREEN)/2+50;

	m_ImgNo = 1;
	//m_bHttpTransfer = TRUE;
	//memset(m_tcImagerPassword, 0, sizeof(TCHAR) * MAX_PATH); 
	//memset(m_tcImagerUsername, 0, sizeof(TCHAR) * MAX_PATH); 
	m_hInst=hInstance;
	m_hParentWnd=hParent;

	m_bImgLight = FALSE;//default state
	m_bImgAim = FALSE; //default state

	//Initialize Msg Queues
	m_MetamsgqOpt.dwSize		= sizeof(m_MetamsgqOpt);
	m_MetamsgqOpt.dwFlags		= MSGQUEUE_ALLOW_BROKEN;
	m_MetamsgqOpt.dwMaxMessages = 0;
	m_MetamsgqOpt.cbMaxMessage	= sizeof(MetaTagQEntry);
	m_MetamsgqOpt.bReadAccess	= TRUE;
	m_hMetaMsgQRead				= NULL;
	m_hMetaMsgQRead				= CreateMsgQueue(L"ImagerMetaMsgQ", &m_MetamsgqOpt);
	m_MetamsgqOpt.bReadAccess	= FALSE;
	m_hMetaMsgQWrite			= NULL;
	m_hMetaMsgQWrite			= CreateMsgQueue(L"ImagerMetaMsgQ", &m_MetamsgqOpt);
	m_iDesiredWidth				= -1;
	m_iDesiredHeight			= -1;
	
	//Initialize dev name to default dev
	wcscpy(m_szDevName,L"IMG1:");

	//Initialize it to ZERO to indicate no navigate string
	memset(m_EnumIMGNav,0,sizeof(m_EnumIMGNav));

	//Create DirectShow camera object
#ifdef STATIC_DSCAM
	m_pDSCam = &g_DSCam;
#else
	m_pDSCam = new CDShowCam(m_pModule);
#endif
	
	//Create list of available imager devices on terminal
	CreateListOfAvailableDevices();

#ifndef STATIC_DSCAM
		DEBUGIT(LogFile,"CImager::CImager deleting m_pDSCam \n");
		

	if (m_pDSCam) delete m_pDSCam;
	m_pDSCam = NULL;
	

#endif
	

	//Check that default dev exists otherwise select the first exisiting device
	ListDevName *pDevName = g_ListDevName;
	while (pDevName)
	{
		if (wcscmp(pDevName->DeviceName, m_szDevName) == 0)
			break;
		pDevName = pDevName->pNext;
	}
	if ((!pDevName) && (g_ListDevName))
	{
		wcsncpy(m_szDevName,g_ListDevName->DeviceName, 50);
	}
	m_szImagerCaptureURI = NULL;

	LPCTSTR pSettingValue = m_pModule->GetAppSettingPtr(m_iInstanceID, L"Imager\\DisableImagerDuringNavigation", L"Value");
	if (pSettingValue && pSettingValue[0] == L'0')
		m_bDisableDuringNavigate = false;
	else
		m_bDisableDuringNavigate = true;

	DEBUGIT(LogFile,"Imager created \n");
	
}



////////////////////////////////////////////////////////////////////////
// Function:	~CImager
// Description:	CImager Destructor - Viewfinder is stopped, 
//              Loads Image capture Library, Initializes Image Captue API function pointers.
//				Deinitializes imager and closes Image captue device handle
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
CImager::~CImager()
{	
	DEBUGIT(LogFile,"Imager destructor entered \n");
	
	if (m_PreviewOn)
	{	int result = StopViewer();
		if (result!=0)
		{
			DEBUGIT(LogFile,"StopViewer Failed with %d \n", result);
			
		}
	}
	if (m_bImagerLoadFailed && (m_hImager != INVALID_HANDLE_VALUE)) {
		Image_UnLockImager(m_hImager);
		Image_Close(m_hImager);
	}
	if(g_ListDevName!=NULL)
		LocalFree(g_ListDevName);

	if (hModule != NULL) FreeLibrary(hModule);
	delete [] m_lpSzFilename;
	delete[] m_szImagerCaptureURI;

#ifndef STATIC_DSCAM
	DEBUGIT(LogFile,"CImager::~CImager deleting m_pDSCam \n");
	
	if (m_pDSCam != NULL) delete m_pDSCam;
	m_pDSCam = NULL;
#endif
#ifdef LOG_TO_FILE	
	fclose(LogFile);
#endif
}


////////////////////////////////////////////////////////////////////////
// Function:	InitImager
// Description:	Finds first Imager device and opens handle to image capture device, 
//              image capture device handle is used to call supported Image capture APIs.
//				
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
DWORD CImager::InitImager()
{

	DWORD dwRes = E_IMG_SUCCESS;
	IMAGE_FINDINFO ImageFindInfo;
	DEBUGIT(LogFile,"Init imager entered \n");
	
	// Found the imager number requested so now open it (if it's an imager)
	if (wcsnicmp(m_szDevName, L"IMG", 3) != 0)
	{
		// if it's not an imager device we don't need to open it here
		m_hImager = INVALID_HANDLE_VALUE;
	}
	else if (m_bImagerLoadFailed)
		return 999;
	else if (m_szDevName[0] != L'\0')
	{
		memset(&ImageFindInfo, 0, sizeof(ImageFindInfo));
		SI_INIT(&ImageFindInfo);
		SI_SET_USED(&ImageFindInfo, tszRegistryBasePath);

		dwRes = Image_Open(m_szDevName /*ImageFindInfo.tszDeviceName*/, &m_hImager);

		if (dwRes != E_IMG_SUCCESS) {
			TCHAR tcErr[200];
			wsprintf(tcErr, L"Image_Open error %d", dwRes);
			
			return dwRes;
		}
	}

	SetName(DEFAULT_FILENAME);
	DEBUGIT(LogFile,"Init imager left \n");
	
	return dwRes;
	
}


	
////////////////////////////////////////////////////////////////////////
// Function:	StartViewer
// Description:	This function starts Viewfinder for opened image capture device.
//              image capture device handle is used to call supported Image capture APIs.
//				Sets JPEG as default image format. Sets the Light and Aim to default state FALSE
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
DWORD CImager::StartViewer()
{

	DWORD dwCapValue;
	DWORD dwRes; 
		DEBUGIT(LogFile,"start viewer entered \n");
	
	// Create the viewer window
	m_hWndViewer = CreateWindowEx(WS_EX_NOANIMATION|WS_EX_TOPMOST, L"static", NULL, WS_VISIBLE | WS_POPUP, 
				m_rcWinPos.left,
				m_rcWinPos.top,
				m_rcWinPos.right,
				m_rcWinPos.bottom, 
				m_hParentWnd, NULL, m_hInst, NULL);
				
	DEBUGIT(LogFile,"start viewer 1\n");
	
	if (wcsnicmp(m_szDevName, L"IMG", 3) == 0)
	{
		// If any of the functions failed to initialise return immediately
		if (m_bImagerLoadFailed || (m_hImager == INVALID_HANDLE_VALUE))
		{
			DestroyWindow(m_hWndViewer);
			DEBUGIT(LogFile,"m_bImagerLoadFailed || (m_hImager == INVALID_HANDLE_VALUE \n");
			
			return 999;
		}

		// Set the width of the target window for the video viewfinder
		dwCapValue = m_rcWinPos.right;// - m_rcWinPos.left + 1;
		if((dwRes = Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINWIDTH, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS))
		{
			DEBUGIT(LogFile,"Imager_SetCapCurrValue error %d", dwRes);
			
			DestroyWindow(m_hWndViewer);

			return 999;
		}
		
		// Set the height of the target window for the video viewfinder
		dwCapValue = m_rcWinPos.bottom;// - m_rcWinPos.top + 1;
		if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHEIGHT, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
		{
			DEBUGIT(LogFile,"Imager_SetCapCurrValue error %d", dwRes);
			
			DestroyWindow(m_hWndViewer);
			
			return 999;
		}
		BOOL bCrop = TRUE;
		if(Image_SetCapCurrValue(m_hImager, IMG_IMGCAP_MATCHVFCAPCROP, sizeof(BOOL), &bCrop) != E_IMG_SUCCESS)
		{
			DEBUGIT(LogFile,"Imager_SetCapCurrValue error %d", dwRes);
			
			DestroyWindow(m_hWndViewer);
			
			return 999;
		}
		
		// Specifies the window handle to be used to display the viewfinder
		dwCapValue = (DWORD)m_hWndViewer;
		if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHANDLE, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
		{
			DEBUGIT(LogFile,"Imager_SetCapCurrValue error %d", dwRes);
			
			DestroyWindow(m_hWndViewer);
			
			return 999;
		}

		dwCapValue = TWFF_JPEG; 
		if(Image_SetCapCurrValue(m_hImager, IMG_IMGCAP_FILEFORMAT, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
		{
			DEBUGIT(LogFile,"Imager_SetCapCurrValue error %d", dwRes);
			
			DestroyWindow(m_hWndViewer);
			
			return 999;
		}
		

		// Start acquiring images and Turn on the viewfinder
		if ((dwRes = Image_StartAcquisition(m_hImager)) != E_IMG_SUCCESS) {
			DEBUGIT(LogFile, "Imager_StartAcquisition error %d", dwRes);
			
			DestroyWindow(m_hWndViewer);
			
			return dwRes;
		}

		if ((dwRes = Image_StartViewfinder(m_hImager)) != E_IMG_SUCCESS) {
			DEBUGIT(LogFile,"Imager_StartViewfinder error %d", dwRes);
			
			DestroyWindow(m_hWndViewer);
			
			return dwRes;
		}
		
		
		Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_LAMPSTATE, sizeof(BOOL), &m_bImgLight);
		Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_AIMING, sizeof(BOOL), &m_bImgAim);
	}
	else
	{
		// Using a DirectShow Camera device
		CamConfig cfg;

#ifndef STATIC_DSCAM
		DEBUGIT(LogFile,"CImager::StartViewer() deleting m_pDSCam \n");
		
		if (m_pDSCam) delete m_pDSCam;
		m_pDSCam = new CDShowCam(m_pModule);
#endif
		DEBUGIT(LogFile,"start viewer 2\n");
		
		if(SUCCEEDED(m_pDSCam->InitFilterGrp()))
		{
			cfg.sCamID = m_szDevName;	// Pass that selected Camera Id to the "BuildFilterGraph(...)" function.
			cfg.rc = m_rcWinPos;
			// NB: rcWinPos right & bottom are wrong!! it's not being used as a rectangle..
			// it represents: top=top, left=left, bottom=height, right=width so some addition is
			// needed to get the actual rect bottom and right values
//			cfg.rc.bottom += cfg.rc.top;
//			cfg.rc.right += cfg.rc.left;
			cfg.rc.top = 0;
			cfg.rc.left = 0;
			cfg.hwndOwnerWnd = m_hWndViewer;
			cfg.bIsAudioEnb = FALSE;	// no audio capture
			cfg.bIsCapEnb = TRUE;		// enable capture
			cfg.bIsPrvEnb = TRUE;		// enable preview
			cfg.bIsStillEnb = TRUE;		// enable still capture
			DEBUGIT(LogFile,"start viewer 3\n");
			
			if(!m_pDSCam->BuildFilterGrp(&cfg))		
			{
				DEBUGIT(LogFile,"------FGraph Build FAIL------\n");
				
				if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);
			}
		}		
		else
		{
			DEBUGIT(LogFile,"------Init FGraph FAIL------\n");
			
			if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);
			return 1;
		}
		DEBUGIT(LogFile,"start viewer 4\n");
		
		if(!m_pDSCam->RunGrp())			
		{
			DEBUGIT(LogFile,"------FGraph Run FAIL------\n");
			
			if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);
			return 1;
		}

		HRESULT hr;

		CamPropTbl propTbl;
		propTbl.p = CameraControl_Flash;
		hr = m_pDSCam->Get_PropRng(CAM, &propTbl);
		if (hr != E_PROP_ID_UNSUPPORTED)
		{
			propTbl.plVal = m_bImgLight ? propTbl.plMax : propTbl.plMin;
			m_pDSCam->Set_PropVal(CAM, &propTbl);
		}
		else
		{
			DEBUGIT(LogFile,"hr == E_PROP_ID_UNSUPPORTED \n");
			
		}

		//  Set the resolution of the saved image
		if (!(m_iDesiredHeight == -1 && m_iDesiredWidth == -1))
		{
			PinType myPIN = S;
			ImageRes myRes;
			myRes.nHeight = m_iDesiredHeight;
			myRes.nWidth = m_iDesiredWidth;
			if (!m_pDSCam->Set_Resolution(&myRes, myPIN))
			{
				//  Something went wrong setting the desired Image resolution
				m_pModule->Log(PB_LOG_ERROR, L"Set Image Resolution failed", _T(__FUNCTION__), __LINE__);
			}
		}
	}

	m_PreviewOn = TRUE;
	DEBUGIT(LogFile,"start viewer exited \n");
	
	if (m_hParentWnd)
		SetFocus(m_hParentWnd);

	return E_IMG_SUCCESS;
}


////////////////////////////////////////////////////////////////////////
// Function:	StopViewer
// Description:	This function stop Viewfinder for opened image capture device.
//              
//				
// Author:	    Ramanagouda S Patil(RSP)
// Date:	    JULY 2009
////////////////////////////////////////////////////////////////////////
DWORD CImager::StopViewer()
{
	DWORD dwRes;
	
	DEBUGIT(LogFile,"CImager:: stop viewer entered \n");
	
	if (wcsnicmp(m_szDevName, L"IMG", 3) == 0)
	{
		// If any of the functions failed to initialise return immediately
		if (m_bImagerLoadFailed || (m_hImager == INVALID_HANDLE_VALUE)) {
			return 999;
		}

		// Stop the viewfinder first
		if (E_IMG_SUCCESS != (dwRes = Image_StopViewfinder(m_hImager))) {
			DEBUGIT(LogFile, "Imager_StopViewer error %d \n", dwRes);
			
	
			return dwRes;
		}

		// Stop the acquisition
		if (E_IMG_SUCCESS != (dwRes = Image_StopAcquisition(m_hImager))) {
			DEBUGIT(LogFile, "Imager_StopAcquisition error %d \n", dwRes);
			
	
			return dwRes;
		}

		// Destroy the window
		if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);

		
	}
	else
	{
		// Destroy the window
		if (IsWindow(m_hWndViewer)) DestroyWindow(m_hWndViewer);

		// destroy the DirectShow graph
		if (m_pDSCam) 
		{	BOOL hr = m_pDSCam->StopGrp();
			if (hr!=TRUE)
			{
				DEBUGIT(LogFile,  "StopViewer m_pDSCam->StopGrp()  failed with %d \n",hr);
				
				return 1;
			}
		}
		else 
		{
			DEBUGIT(LogFile,  " StopViewer m_pDSCam is null \n");
			
			return 1;
		}
		
		m_pDSCam->ReleaseGrp();
#ifndef STATIC_DSCAM
		DEBUGIT(LogFile,"CImager::StopViewer() deleting m_pDSCam \n");
		
		delete m_pDSCam;
		m_pDSCam = NULL;
#endif
	}
	m_PreviewOn = FALSE;
	
	DEBUGIT(LogFile,"stop viewer exited \n");
	
	return 0;
}


////////////////////////////////////////////////////////////////////////
// Function:	SetViewerPosition
// Description:	This sets the window size and position.  It then adjusts
//				the imager viewer window (inside the first window) and 
//				adjusts the cropping as required.
// Author:		Ramanagouda S Patil(RSP)
// Date:		JULY 2009
////////////////////////////////////////////////////////////////////////
DWORD CImager::SetViewerPosition(int x, int width, int y, int height)
{

	TCHAR tcErr[200];
	if (x != -9999) m_rcWinPos.left = x;
	if (width != -9999) m_rcWinPos.right = width;
	if (y != -9999) m_rcWinPos.top = y;
	if (height != -9999) m_rcWinPos.bottom = height;


	if (wcsnicmp(m_szDevName, L"IMG", 3) == 0)
	{
		if (IsWindow(m_hWndViewer)) {
			DWORD dwRes;
			MoveWindow(m_hWndViewer,
					m_rcWinPos.left,
					m_rcWinPos.top,
					m_rcWinPos.right,
					m_rcWinPos.bottom, 
					FALSE);

			// Set the width of the target window for the video viewfinder
			DWORD dwCapValue = m_rcWinPos.right;// - m_rcWinPos.left + 1;
			if((dwRes = Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINWIDTH, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS))
			{
				wsprintf(tcErr, L"Imager_SetCapCurrValue error %d", dwRes);
				
				return 999;
			}
		
			// Set the height of the target window for the video viewfinder
			dwCapValue = m_rcWinPos.bottom;// - m_rcWinPos.top + 1;
			if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHEIGHT, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
			{
				wsprintf(tcErr, L"Imager_SetCapCurrValue error %d", dwRes);
				
				return 999;
			}
			BOOL bCrop = TRUE;
			if(Image_SetCapCurrValue(m_hImager, IMG_IMGCAP_MATCHVFCAPCROP, sizeof(BOOL), &bCrop) != E_IMG_SUCCESS)
			{
				wsprintf(tcErr, L"Imager_SetCapCurrValue error %d", dwRes);
				
				return 999;
			}
			
			// Specifies the window handle to be used to display the viewfinder
			dwCapValue = (DWORD)m_hWndViewer;
			if(Image_SetCapCurrValue(m_hImager, IMG_VFCAP_WINHANDLE, sizeof(dwCapValue), &dwCapValue) != E_IMG_SUCCESS)
			{
				wsprintf(tcErr, L"Imager_SetCapCurrValue error %d", dwRes);
				
				return 999;
			}
		}
	}
	else if (m_pDSCam)
	{
		if (IsWindow(m_hWndViewer)) {
				MoveWindow(m_hWndViewer,
						m_rcWinPos.left,
						m_rcWinPos.top,
						m_rcWinPos.right,
						m_rcWinPos.bottom, 
						FALSE);

			m_pDSCam->ResizePreview(m_rcWinPos.right, m_rcWinPos.bottom);
		}
	}
	return 0;
}





////////////////////////////////////////////////////////////////////////
// Function:	SetOptions
// Description:	Sets the lamp and aim options on the imager
// Author:		Ramanagouda S Patil(RSP)
// Date:		JULY 2009
////////////////////////////////////////////////////////////////////////
DWORD CImager::SetOptions(int Lamp, int Aim)
{

	BOOL bChange = FALSE;
	if ((Lamp == 0) && (m_bImgLight)) {
		m_bImgLight = FALSE; 
		bChange = TRUE;
	}
	else if ((Lamp == 1) && (!m_bImgLight)) {
		m_bImgLight = TRUE;
		bChange = TRUE;
	}
	if ((Aim == 0) && (m_bImgAim)) {
		m_bImgAim = FALSE; 
		bChange = TRUE;
	}
	else if ((Aim == 1) && (!m_bImgAim)) {
		m_bImgAim = TRUE;
		bChange = TRUE;
	}
	if (bChange && m_hWndViewer) {
		if (wcsnicmp(m_szDevName, L"IMG", 3) == 0)
		{
			Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_LAMPSTATE, sizeof(BOOL), &m_bImgLight);
			Image_SetCapCurrValue(m_hImager, IMG_ACQCAP_AIMING, sizeof(BOOL), &m_bImgAim);
		}
		else if (m_pDSCam)
		{
			HRESULT hr;

			CamPropTbl propTbl;
			propTbl.p = CameraControl_Flash;
			hr = m_pDSCam->Get_PropRng(CAM, &propTbl);
			if (hr != E_PROP_ID_UNSUPPORTED)
			{
				propTbl.plVal = m_bImgLight ? propTbl.plMax : propTbl.plMin;
				hr = m_pDSCam->Set_PropVal(CAM, &propTbl);
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////
// Function:	DeInit
// Description:	Deinitializes imager.
// Author:		Ramanagouda S Patil(RSP)
// Date:		JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImager::DeInit()
{
	DEBUGIT(LogFile,"DeInit entered \n");
	
	if (m_PreviewOn)
	{	int result = StopViewer();
		if (result!=0)
		{
			DEBUGIT(LogFile,"StopViewer Failed with %d \n", result);
			
		}
	}
	if (/*m_bImagerLoadFailed && */(m_hImager != INVALID_HANDLE_VALUE)) {
		DEBUGIT(LogFile,"m_hImager is INVALID_HANDLE_VALUE \n");
		
		Image_UnLockImager(m_hImager);
		Image_Close(m_hImager);
		m_hImager = INVALID_HANDLE_VALUE;
	}
	return TRUE;
}





////////////////////////////////////////////////////////////////////////
// Function:	SendImage
// Description:	Saves the Image file to memory.  If a URL has been specified
//				it then sends the image to the URL using http.
// Author:		Ramanagouda S Patil(RSP)
// Date:		JULY 2009
// Rewritten:	Paul Henderson
// Date			August 2010	
// Added Image Capture as URI capabilities (DCC, June 2012)
////////////////////////////////////////////////////////////////////////
DWORD CImager::SaveImage()
{
	DWORD dwRes;

	//  If the user has defined the ImagerCaptureEvent then send the captured 
	//  image back to them as a Data URI
	bool bSaveBytes = false;
	BYTE* pImageAsBytes = NULL;
	int iLengthOfImage = 0;
	if (m_szImagerCaptureURI)
		bSaveBytes = true;

	if (wcsnicmp(m_szDevName, L"IMG", 3) == 0)
	{
		if ((dwRes = Image_GetImage(m_hImager, &g_dwImageBufSize, &g_pImageBuffer)) != E_IMG_SUCCESS) 
		{
			return ERR;
		}
	
		DWORD dwBytesWritten;
		HANDLE hFile = 
			CreateFile(m_lpSzFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile)
		{ 
			WriteFile(hFile,g_pImageBuffer,g_dwImageBufSize,&dwBytesWritten,NULL);
			if (bSaveBytes)
			{
				pImageAsBytes = (BYTE*)g_pImageBuffer;
				iLengthOfImage = dwBytesWritten;
			}
			CloseHandle(hFile);
		}
	}
	else if (m_pDSCam)
	{
		m_pDSCam->CaptureStill(m_lpSzFilename);
		if (bSaveBytes)
		{
			//  Rather than get bogged down the Direct Show again we'll just
			//  read the file back in from disk.
			HANDLE hFile = CreateFile(m_lpSzFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile)
			{
				DWORD dwFileSize = GetFileSize(hFile, NULL);
				if (dwFileSize > 0)
				{
					pImageAsBytes = new BYTE[dwFileSize];
					iLengthOfImage = dwFileSize;
					if (pImageAsBytes)
					{
						DWORD dwBytesRead = 0;
						do
						{
							if (!ReadFile(hFile, pImageAsBytes, dwFileSize, &dwBytesRead, NULL))
							{
								//  Some error has occured reading the file
								m_pModule->Log(PB_LOG_WARNING, 
								L"Unable to send image data as URI, could not read data", 
								_T(__FUNCTION__), __LINE__);
								delete[] pImageAsBytes;
								pImageAsBytes = NULL;
								break;
							}
						}
						while (dwBytesRead != 0);
					}
				}
				else
				{
					m_pModule->Log(PB_LOG_WARNING, 
					L"Unable to send image data as URI, size was unexpected", 
					_T(__FUNCTION__), __LINE__);
				}
				CloseHandle(hFile);
			}
			else
				m_pModule->Log(PB_LOG_WARNING, 
				L"Unable to send image data as URI, could not find captured image", 
				_T(__FUNCTION__), __LINE__);
		}
	}

	//  If the user has defined the ImagerCaptureEvent then send the captured 
	//  image back to them as a Data URI
	if (pImageAsBytes)
	{
		InvokeImagerCaptureEvent(pImageAsBytes, iLengthOfImage);
		if (m_pDSCam)
		{
			//  If using direct show we are responsible for freeing
			//  the memory associated with the byte array, else
			//  it is done by the EMDK
			delete[] pImageAsBytes;
		}
	}
	return 0;
}



////////////////////////////////////////////////////////////////////////
// Function:	CreateListOfAvailableDevices
// Description: Creates list of image capture devices and saves in global list g_ListDevName
//				
// Author:		Ramanagouda S Patil(RSP)
// Date:		JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImager::CreateListOfAvailableDevices()
{
	DWORD dwRes;

	// Find the Imager
	if (!m_bImagerLoadFailed)
	{
		IMAGE_FINDINFO ImageFindInfo;
		HANDLE FindHandle;
		
		memset(&ImageFindInfo, 0, sizeof(ImageFindInfo));
		SI_INIT(&ImageFindInfo);
		SI_SET_USED(&ImageFindInfo, tszRegistryBasePath);

		if (E_IMG_SUCCESS == (dwRes = Image_FindFirst(&ImageFindInfo, &FindHandle))) {

			do{
				if(!g_ListDevName)
				{
					g_ListDevName = (ListDevName*)LocalAlloc(LPTR,sizeof(ListDevName));//malloc(sizeof(ListDevName));
					if(g_ListDevName!=NULL)
					{
						g_ListDevName->pNext = NULL;
						wcscpy(g_ListDevName->DeviceName,ImageFindInfo.tszDeviceName);
						wcscpy(g_ListDevName->DeviceFriendlyName,ImageFindInfo.tszFriendlyName);

						if(!_memicmp(g_ListDevName->DeviceFriendlyName, L"Image Capture Driver for Color Camera", wcslen(L"Image Capture Driver for Color Camera")*sizeof(TCHAR)))
							g_ListDevName->bDeviceType = FALSE;
						else
							g_ListDevName->bDeviceType = TRUE;//Imager
					}
				}
				else
				{
					ListDevName *pRoot = g_ListDevName;
					ListDevName *pNode = (ListDevName*)LocalAlloc(LPTR,sizeof(ListDevName));;//malloc(sizeof(ListDevName));
					if(pNode !=NULL)
					{
						while(pRoot->pNext !=NULL)pRoot = pRoot->pNext;
						wcscpy(pNode->DeviceName,ImageFindInfo.tszDeviceName);
						wcscpy(pNode->DeviceFriendlyName,ImageFindInfo.tszFriendlyName);

						if(!_memicmp(g_ListDevName->DeviceFriendlyName, L"Image Capture Driver for Color Camera", wcslen(L"Image Capture Driver for Color Camera")*sizeof(TCHAR)))
							g_ListDevName->bDeviceType = FALSE;
						else
							g_ListDevName->bDeviceType = TRUE;//Camera

						pNode->bDeviceType = TRUE;//Imager
						pNode->pNext = NULL;
						pRoot->pNext = pNode;
					}
				}
			}
			while(	E_IMG_SUCCESS == Image_FindNext( &ImageFindInfo, FindHandle ));
			Image_FindClose(FindHandle);
		}
	}

	if (!m_pDSCam) return TRUE;

	// look for any DirectShow cameras and add them on the end
	wstring wsCamId;
	BOOL bCamFound = m_pDSCam->FindFirstCam(&wsCamId);

	//ListDevName* myPtr = NULL;

	//myPtr = (PListDevName)LocalAlloc(LPTR, (sizeof ListDevName)*2);
	//myPtr->pNext = NULL;

	//myPtr->bDeviceType=FALSE;
	//myPtr->DeviceName[0] = L'\0';
	//myPtr->DeviceFriendlyName[0] = L'\0';
	//g_ListDevName = myPtr;
	//return TRUE;
	//wcsncpy(g_ListDevName->DeviceName,/*id*/wsCamId.c_str(),MAX_LENGTH-1);
	//g_ListDevName->DeviceName[MAX_LENGTH] = L'\0';
	//_snwprintf(g_ListDevName->DeviceFriendlyName,MAX_LENGTH-1,L"Windows Camera Device");
	//g_ListDevName->bDeviceType = FALSE; // bDeviceType = colour camera


	while (bCamFound)
	{
		//WCHAR id[100];
		//id[99] = L'\0';
		//wcsncpy(id, wsCamId.c_str(), 99);
		//int idLen = wcslen(id);
		//// remove any trailing ":" 
		//if (id[idLen-1] == L':')
		//{
		//	id[idLen-1] = L'\0';
		//}
		if(!g_ListDevName)
		{
			g_ListDevName = (ListDevName*)LocalAlloc(LPTR,sizeof(ListDevName));
			if(g_ListDevName!=NULL)
			{
				g_ListDevName->pNext = NULL;
				wcsncpy(g_ListDevName->DeviceName,/*id*/wsCamId.c_str(),99);
				wsprintf(g_ListDevName->DeviceFriendlyName,L"Windows Camera Device");
				g_ListDevName->bDeviceType = FALSE; // bDeviceType = colour camera
			}
		}
		else
		{
			ListDevName *pRoot = g_ListDevName;
			ListDevName *pNode = (ListDevName*)LocalAlloc(LPTR,sizeof(ListDevName));
			if(pNode !=NULL)
			{
				while(pRoot->pNext !=NULL)pRoot = pRoot->pNext;
				wcsncpy(pNode->DeviceName,/*id*/wsCamId.c_str(),99);
				wsprintf(pNode->DeviceFriendlyName,L"Windows Camera Device");
				pNode->bDeviceType = FALSE; // Camera
				pNode->pNext = NULL;
				pRoot->pNext = pNode;
			}
		}
		bCamFound = m_pDSCam->FindNextCam(&wsCamId);
	}

	return (g_ListDevName != NULL);
;
}
////////////////////////////////////////////////////////////////////////
// Function:	EnumImagerNav
// Description: Saves Imager enummertion url 
//				
// Author:		Ramanagouda S Patil(RSP)
// Date:		JULY 2009
////////////////////////////////////////////////////////////////////////
BOOL CImager::EnumImagerNav(PBMetaStruct *pbMetaStructure, PPBSTRUCT /*pPBStructure  --unused*/)
{

	if((pbMetaStructure->lpValue!= NULL)||(wcscmp(pbMetaStructure->lpValue,L"")!=0))
		wcscpy(m_EnumIMGNav,pbMetaStructure->lpValue );

	return TRUE;
}


/**
* \author	Paul Henderson (PH, PTJK76)
* \date		August 2006 - Initial Creation, PH
*/
BOOL CImager::SetName(LPCTSTR lpSzFilename)
{
	int iLen;
	delete [] m_lpSzFilename;	

	//if the name was set to L"", then the default name will be selected
	if((iLen = wcslen(lpSzFilename))== 0){
		m_lpSzFilename = new TCHAR[wcslen(DEFAULT_FILENAME)+ 2];
		if(!m_lpSzFilename) return FALSE;
		wcscpy(m_lpSzFilename,L"\\");
		wcscat(m_lpSzFilename,DEFAULT_FILENAME);
		
		

	}
	else{	
		m_lpSzFilename = new TCHAR[iLen + 2];	
		if(m_lpSzFilename == NULL){
			//error
			return FALSE;
		}
		wcscpy(m_lpSzFilename,L"\\");
		wcscat(m_lpSzFilename,lpSzFilename);
		
	}
	
	return TRUE;
}

BOOL CImager::DirectShowShutterClick()
{
	BOOL result = FALSE;

	if (m_pDSCam)
		result = m_pDSCam->HandlesCaptureSound();
	return result;
}

BOOL CImager::SetDirectShowShutterSound(LPWSTR szSoundFile)
{
	BOOL result = FALSE;

	if (m_pDSCam)
		result = m_pDSCam->Set_CaptureSound(szSoundFile);
	return result;
}

BOOL CImager::SetDesiredWidth(int iDesiredWidth)
{
	BOOL bRetVal = TRUE;
	if (m_iDesiredWidth != iDesiredWidth)
	{
		m_iDesiredWidth = iDesiredWidth;
		if (m_PreviewOn)
		{
			PinType myPIN = S;
			ImageRes myRes;
			myRes.nHeight = m_iDesiredHeight;
			myRes.nWidth = m_iDesiredWidth;
			bRetVal = m_pDSCam->Set_Resolution(&myRes, myPIN);
			if (!bRetVal)
			{
				//  Something went wrong setting the desired Image resolution
				m_pModule->Log(PB_LOG_WARNING, L"Set Image Resolution failed", _T(__FUNCTION__), __LINE__);
			}
		}
	}
	return bRetVal;	
}

BOOL CImager::SetDesiredHeight(int iDesiredHeight)
{
	BOOL bRetVal = TRUE;
	if (m_iDesiredHeight != iDesiredHeight)
	{
		m_iDesiredHeight = iDesiredHeight;
		if (m_PreviewOn)
		{
			PinType myPIN = S;
			ImageRes myRes;
			myRes.nHeight = m_iDesiredHeight;
			myRes.nWidth = m_iDesiredWidth;
			bRetVal = m_pDSCam->Set_Resolution(&myRes, myPIN);
			if (!bRetVal)
			{
				//  Something went wrong setting the desired Image resolution
				m_pModule->Log(PB_LOG_WARNING, L"Set Image Resolution failed", _T(__FUNCTION__), __LINE__);
			}
		}
	}
	return bRetVal;	
}

BOOL CImager::SetImagerCaptureEvent(LPCWSTR szNavURI)
{
	//  store the passed navigation URI
	if (!szNavURI)
		return FALSE;
	if (m_szImagerCaptureURI)
		delete[] m_szImagerCaptureURI;

	m_szImagerCaptureURI = new WCHAR[(wcslen(szNavURI) * sizeof(WCHAR)) + 1];
	wcscpy(m_szImagerCaptureURI, szNavURI);
	return TRUE;
}

BOOL CImager::InvokeImagerCaptureEvent(BYTE* bData, int iLength)
{
	//  Called to invoke the ImagerCaptureEvent which contains a DataURI
	//  encoding of the Imager data.
	if (m_szImagerCaptureURI && m_szImagerCaptureURI[0])
	{
		//  Convert the Image to base 64, this representation is about 1/3
		//  larger than the binary input
		char* szEncodedString = new char[iLength * 2];
		memset(szEncodedString, 0, iLength * 2);
		EncodeToBase64(bData, iLength, szEncodedString);
		//  The DataURI is longer than the base 64 representation because of the header
		int iLengthOfReturnString = strlen(szEncodedString) + 30;
		WCHAR* szImagerAsDataURI = new WCHAR[iLengthOfReturnString];
		if (szImagerAsDataURI)
		{
			memset(szImagerAsDataURI, 0, iLengthOfReturnString * sizeof(WCHAR));
			wcscpy(szImagerAsDataURI, L"data:image/jpeg;base64,");
			mbstowcs(szImagerAsDataURI + wcslen(L"data:image/jpeg;base64,"),
				szEncodedString, iLengthOfReturnString);

			m_pModule->SendPBNavigate(tcImagerCaptureEventNames, m_iInstanceID, 
				m_szImagerCaptureURI, szImagerAsDataURI, NULL); 
		}
		delete[] szImagerAsDataURI;
		delete[] szEncodedString;
	}
	return TRUE;
}
